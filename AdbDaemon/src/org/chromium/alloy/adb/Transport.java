package org.chromium.alloy.adb;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.Deque;
import java.util.HashMap;
import java.util.LinkedList;

class Transport implements IOChannel {
  private SocketChannel mChannel = null;
  private int mSelectionOps = 0;
  private SelectionKey mSelectionKey = null;
  private Deque<ByteBuffer> mOutputQue = new LinkedList<ByteBuffer>();
  private HashMap<Integer, AdbSocket> mSocketMap = new HashMap<Integer, AdbSocket>();

  private boolean mOnline = false;

  public Transport(SocketChannel channel) {
    mChannel = channel;
  }

  public void enableRead(boolean enable) {
    System.out.println("enableRead: enable=" + enable);
    int Ops = mSelectionOps;
    if (enable) {
      Ops |= SelectionKey.OP_READ;
    } else {
      Ops &= ~SelectionKey.OP_READ;
    }

    updateSelection(Ops);
  }

  public void enableWrite(boolean enable) {
    System.out.println("enableWrite: enable=" + enable);
    int Ops = mSelectionOps;
    if (enable) {
      Ops |= SelectionKey.OP_WRITE;
    } else {
      Ops &= ~SelectionKey.OP_WRITE;
    }
    updateSelection(Ops);
  }

  private void updateSelection(int Ops) {
    if (Ops != mSelectionOps) {
      try {
        mSelectionKey = mChannel.register(
            AdbServer.server().selector(), Ops, this);
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

  private void createLocalServiceSocket(final String name) {

  }

  private void handleSync(AdbMessage message) {
    if (message.arg0 == 0)
      offline();
    send(message);
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

    for (String prop: cnxn_props)
      builder.append(String.format("%s=%s;", prop, "alloy"));

    AdbMessage cm = new AdbMessage(AdbMessage.A_CNXN, AdbMessage.A_VERSION,
        AdbMessage.MAX_PAYLOAD, builder.toString());
    send(cm);
  }

  private void handleAuth(AdbMessage message) {

  }

  private void handleOpen(AdbMessage message) {
    if (mOnline) {
     String name = new String(message.data);
     System.out.println(String.format("OPEN %s", name));
     createLocalServiceSocket(name);
    }
  }

  private void handleClose(AdbMessage message) {

  }

  private void handleWrite(AdbMessage message) {

  }

  public void send(AdbMessage message) {
    ByteBuffer buffer  = message.toByteBuffer();
    buffer.flip();
    mOutputQue.addLast(buffer);
    enableWrite(true);
  }

  @Override
  public void onAcceptable() throws IOException {
  }

  @Override
  public void onReadable() throws IOException {
    ByteBuffer buffer = ByteBuffer.allocate(AdbMessage.MAX_PAYLOAD + 24);
    int result = mChannel.read(buffer);
    System.out.println("onReadable: result=" + result);
    buffer.flip();
    while(buffer.hasRemaining()) {
      AdbMessage message = new AdbMessage(buffer);
      System.out.println("dest = " + message);
      switch (message.command) {
        case AdbMessage.A_SYNC: handleSync(message); break;
        case AdbMessage.A_CNXN: handleConnect(message); break;
        case AdbMessage.A_AUTH: handleAuth(message); break;
        case AdbMessage.A_OPEN: handleOpen(message); break;
        case AdbMessage.A_CLSE: handleClose(message); break;
        case AdbMessage.A_WRTE: handleWrite(message); break;
        default:
          System.err.println(String.format("Unknown message command is 0x%08x", message.command));
          break;
      }
    }
  }

  @Override
  public void onWritable() throws IOException {
    System.out.println("onWriteable: size=" + mOutputQue.size());
    if (!mOutputQue.isEmpty()) {
      ByteBuffer buffer = mOutputQue.getFirst();
      mChannel.write(buffer);
      if (!buffer.hasRemaining())
        mOutputQue.removeFirst();
    }
    if (mOutputQue.isEmpty())
      enableWrite(false);
  }
}
