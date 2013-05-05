package org.chromium.alloy.adb;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.Pipe;
import java.nio.channels.SelectionKey;

abstract class AdbThreadSocket extends AdbSocket implements IOChannel {
	private Thread mThread;
	private Pipe mInput;
	private Pipe mOutput;
	private ByteBuffer mInputBuffer = null;
	private ByteBuffer mOutputBuffer = null;
	private boolean mInitialized = false;
	private boolean mReady = false;
	
	protected AdbThreadSocket(String name) throws IOException {
		mInput = Pipe.open();
		mInput.sink().configureBlocking(false);
		mOutput = Pipe.open();
		mOutput.source().configureBlocking(false);
		mOutputBuffer = ByteBuffer.allocate(AdbMessage.MAX_PAYLOAD);
		
		mThread = new Thread(new Runnable() {
			@Override
      public void run() {
				loop();
				try {
	        output().close();
        } catch (IOException e) {
	        e.printStackTrace();
        }
				try {
	        input().close();
        } catch (IOException e) {
	        e.printStackTrace();
        }
      }
		}, name);
	}
	
	protected AdbThreadSocket() throws IOException {
		this("AdbService");
	}
	
	protected abstract void loop();
	
	protected Pipe.SourceChannel input() {
		return mInput.source();
	}
	
	protected Pipe.SinkChannel output() {
		return mOutput.sink();
	}

  public void ready() {
  	if (mInitialized == false) {
  		mThread.start();
  		mInitialized = true;
  	}
  	
  	if (!mThread.isAlive()) {
  		close();
  		return;
  	}
  	
  	try {
	    mOutput.source().register(AdbServer.server().selector(),
	    		SelectionKey.OP_READ, this);
	    mReady = true;
    } catch (ClosedChannelException e) {
	    e.printStackTrace();
	    close();
    }
  }

  @Override
  public int enqueue(AdbMessage message) {
  	if (mInputBuffer != null)
  		return -1;
  	mInputBuffer = ByteBuffer.wrap(message.data, 0, message.dataLength);
  	try {
	    mInput.sink().write(mInputBuffer);
    } catch (IOException e) {
	    e.printStackTrace();
	    mInputBuffer = null;
	    return -1;
    }
  	if (mInputBuffer.hasRemaining()) {
  		try {
	      mInput.sink().register(AdbServer.server().selector(),
	      		SelectionKey.OP_WRITE, this);
      } catch (ClosedChannelException e) {
      	mInputBuffer = null;
      	return -1;
      }
  		return 1;
  	}
  	mInputBuffer = null;
  	return 0;
  }

  public void close() {
  	if (mThread != null && mThread.isAlive()) {
  		mThread.interrupt();
  		try {
	      mThread.join();
      } catch (InterruptedException e) {
	      e.printStackTrace();
      }
  	}
  	super.close();
  }

  public void disconnect() {
  }

	@Override
  public boolean onAcceptable() {
		return false;
  }

	@Override
  public boolean onReadable() {
		if (!mReady) {
			try {
		    mOutput.source().register(AdbServer.server().selector(),
		    		0, this);
	    } catch (ClosedChannelException e) {
		    e.printStackTrace();
		    return false;
	    }
		}
		mOutputBuffer.rewind().clear();
		int result = -1;
    try {
	    result = mOutput.source().read(mOutputBuffer);
    } catch (IOException e) {
	    e.printStackTrace();
    }
		if (result < 0)
			return false;
		mOutputBuffer.flip();
		AdbMessage message = new AdbMessage();
		message.data = new byte[result];
		mOutputBuffer.get(message.data);
		mPeer.enqueue(message);
		mReady = false;
		return true;
	}

	@Override
  public boolean onWritable() {
		try {
			if (mInputBuffer != null) {
				mInput.sink().write(mInputBuffer);
				if (!mInputBuffer.hasRemaining())
					mInputBuffer = null;
			}
			if (mInputBuffer == null) {
				mInput.sink().register(AdbServer.server().selector(),
				    SelectionKey.OP_WRITE, this);
			}
			return true;
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
	}

	@Override
  public void onClose() {
		close();
	}
}