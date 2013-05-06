package org.chromium.alloy.adb;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.Deque;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map.Entry;

class Transport implements IOChannel {
  private static final boolean DEBUG = true;
	private SocketChannel mChannel = null;
  private int mSelectionOps = 0;
  private ByteBuffer mReadBuffer = null;
  private Deque<ByteBuffer> mOutputQue = new LinkedList<ByteBuffer>();
  private HashMap<Integer, AdbSocket> mSocketMap = new HashMap<Integer, AdbSocket>();

  private boolean mOnline = false;

  public Transport(SocketChannel channel) {
    mChannel = channel;
    mReadBuffer = ByteBuffer.allocate(AdbMessage.MAX_MESSAGE_SIZE * 32);
    mReadBuffer.order(ByteOrder.LITTLE_ENDIAN);
  }

  public void enableRead(boolean enable) {
    synchronized (this) {
      enableReadLocked(enable);
    }
  }

  private void enableReadLocked(boolean enable) {
    int Ops = mSelectionOps;
    if (enable) {
      Ops |= SelectionKey.OP_READ;
    } else {
      Ops &= ~SelectionKey.OP_READ;
    }
    updateSelectionLocked(Ops);
  }

  public void enableWrite(boolean enable) {
    System.out.println("enableWrite: enable=" + enable);
    synchronized (this) {
      enableWriteLocked(enable);
    }
  }

  private void enableWriteLocked(boolean enable) {
    int Ops = mSelectionOps;
    if (enable) {
      Ops |= SelectionKey.OP_WRITE;
    } else {
      Ops &= ~SelectionKey.OP_WRITE;
    }
    updateSelectionLocked(Ops);
  }

  private void updateSelectionLocked(int Ops) {
    if (Ops != mSelectionOps) {
      try {
        mChannel.register(AdbServer.server().selector(), Ops, this);
        AdbServer.server().selector().wakeup();
      } catch (ClosedChannelException e) {
        e.printStackTrace();
      }
      mSelectionOps = Ops;
    }
  }

  private void online() {
    mOnline = true;
  }

  private void offline() {
    mOnline = false;
  }

  private AdbSocket createLocalServiceSocket(final String name) {
    AdbSocket socket = null;
    if ("jdwp".equals(name)) {
      socket = new JDWPService();
    } else if ("track-jdwp".equals(name)) {
      socket = new TrackJDWPService();
    } else if (name.startsWith("shell:")) {
      try {
	      socket = new ShellService(name.substring(6));
      } catch (IOException e) {
	      e.printStackTrace();
      }
    } else if (name.startsWith("sync:")) {
      try {
	      socket = new SyncService();
      } catch (IOException e) {
	      e.printStackTrace();
      }
    }
    return socket;
  }

  private void handleSync(AdbMessage message) {
    if (message.arg0 != 0) {
      send(message);
    } else {
      offline();
      send(message);
    }
  }

  private void handleConnect(AdbMessage message) {
    online();
    StringBuilder builder = new StringBuilder();
    builder.append("device::");

    final String[] cnxn_props = {
    	"ro.product.name",
    	"ro.product.model",
    	"ro.product.device"
    };

    for (String prop : cnxn_props)
      builder.append(String.format("%s=%s;", prop, "alloy"));

    AdbMessage cm = new AdbMessage(
        AdbMessage.A_CNXN, AdbMessage.A_VERSION, AdbMessage.MAX_PAYLOAD, builder.toString());
    send(cm);
  }

  private void handleAuth(AdbMessage message) {}

  private void handleOkay(AdbMessage message) {
    if (mOnline) {
      AdbSocket socket = mSocketMap.get(message.arg1);
      if (socket != null) {
        if (socket.peer() == null) {
          socket.connectPeer(new AdbRemoteSocket(message.arg0, this));
        }
        socket.ready();
      }
    }
  }

  private void handleOpen(AdbMessage message) {
    if (mOnline) {
      // Trim the zero at end of the message.
      int length = message.data.length;
      if (message.data[length - 1] == 0)
        length -= 1;
      String name = new String(message.data, 0, length);
      AdbSocket socket = createLocalServiceSocket(name);
      AdbRemoteSocket remote = new AdbRemoteSocket(message.arg0, this);
      if (socket == null) {
        remote.close();
      } else {
        mSocketMap.put(socket.id(), socket);
        socket.connectPeer(remote);
        socket.peer().ready();
        socket.ready();
      }
    }
  }

  private void handleClose(AdbMessage message) {
    if (mOnline) {
      AdbSocket socket = mSocketMap.remove(message.arg1);
      if (socket != null) socket.close();
    }
  }

  private void handleWrite(AdbMessage message) {
    if (mOnline) {
      AdbSocket socket = mSocketMap.get(message.arg1);
      if (socket != null) {
        if (socket.enqueue(message) == 0)
          socket.peer().ready();
      }
    }
  }

  public void send(AdbMessage message) {
    ByteBuffer buffer = message.toByteBuffer();
    buffer.flip();
    synchronized (this) {
      mOutputQue.addLast(buffer);
      enableWriteLocked(true);
    }
  }

  @Override
  public boolean onReadable() {
  	try {
      if (mChannel.read(mReadBuffer) < 0)
      	return false;
      mReadBuffer.flip();
      while (true) {
        AdbMessage message = null;
        message = AdbMessage.read(mReadBuffer);
        if (message == null) {
          mReadBuffer.compact();
          return true;
        }
        if (DEBUG)
        	System.err.println("message = " + message);
        switch (message.command) {
          case AdbMessage.A_SYNC:
            handleSync(message);
            break;
          case AdbMessage.A_CNXN:
            handleConnect(message);
            break;
          case AdbMessage.A_AUTH:
            handleAuth(message);
            break;
          case AdbMessage.A_OKAY:
            handleOkay(message);
            break;
          case AdbMessage.A_OPEN:
            handleOpen(message);
            break;
          case AdbMessage.A_CLSE:
            handleClose(message);
            break;
          case AdbMessage.A_WRTE:
            handleWrite(message);
            break;
          default:
          	if (DEBUG) {
          		System.err.println(String.format(
          				"Unknown message: command is 0x%08x", message.command));
          	}
          	return false;
        }
      }
  	} catch (IOException e) {
  		e.printStackTrace();
  		return false;
  	}
  }

	@Override
	public boolean onWritable() {
		try {
			synchronized (this) {
				if (!mOutputQue.isEmpty()) {
					ByteBuffer buffer = mOutputQue.getFirst();
					mChannel.write(buffer);
					if (!buffer.hasRemaining())
						mOutputQue.removeFirst();
				}
				if (mOutputQue.isEmpty())
					enableWriteLocked(false);
			}
			return true;
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
	}

	@Override
  public boolean onAcceptable() { return false; }

	@Override
  public void onClose() {
		Iterator<Entry<Integer, AdbSocket>> iterator = 
				mSocketMap.entrySet().iterator();
		while (iterator.hasNext()) {
			iterator.next().getValue().close();
		}
		mSocketMap.clear();
  }
}
