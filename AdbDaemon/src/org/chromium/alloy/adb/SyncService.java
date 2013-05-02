package org.chromium.alloy.adb;

import java.io.File;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.concurrent.BlockingDeque;
import java.util.concurrent.LinkedBlockingDeque;


class SyncService extends AdbSocket implements Runnable {
  private static final int MESSAGE_QUE_SIZE = 32;
  private static final int SYNC_DATA_MAX = 64 * 1024;
  private BlockingDeque<AdbMessage> mMessageQue = new LinkedBlockingDeque<AdbMessage>(MESSAGE_QUE_SIZE);
  private boolean mBlocking = false;
  private ByteBuffer mBuffer = null;
  private Thread mThread = null;
  
  static class Msg {
    static final int ID_STAT = 0x54415453;
    static final int ID_LIST = 0x5453494c;
    static final int ID_ULNK = 0x4b4e4c55;
    static final int ID_SEND = 0x444e4553;
    static final int ID_RECV = 0x56434552;
    static final int ID_DENT = 0x544e4544;
    static final int ID_DONE = 0x454e4f44;
    static final int ID_DATA = 0x41544144;
    static final int ID_OKAY = 0x59414b4f;
    static final int ID_FAIL = 0x4c494146;
    static final int ID_QUIT = 0x54495551;

    static class Req {
      int id;
      int nameLen;
      String name;
    }
    
    static class Stat {
      int id = Msg.ID_STAT;
      int mode = 0;
      int size = 0;
      int time = 0;
    }
    
    static class Dent {
      int id;
      int mode;
      int size;
      int time;
      int nameLen;
    }
    
    static class Data {
      int id;
      int size;
    }
    
    static class Status {
      int id;
      int msgLen;
    }
  }
  
  public SyncService() {
    mBuffer = ByteBuffer.allocate(SYNC_DATA_MAX * 2);
    mBuffer.flip();
    mBuffer.order(ByteOrder.LITTLE_ENDIAN);
  }

  private void waitData(int len) throws InterruptedException {
    if (mBuffer.remaining() >= len)
      return;
    AdbMessage message = mMessageQue.takeFirst();
    synchronized (this) {
      if (mBlocking) {
        mPeer.ready();
        mBlocking = false;
      }
    }
    mBuffer.compact();
    mBuffer.put(message.data);
    while (true) {
      message = mMessageQue.peekFirst();
      if (message == null || mBuffer.remaining() < message.dataLength)
        break;
      mBuffer.put(message.data, 0, message.dataLength);
      mMessageQue.removeFirst();
    }
    mBuffer.flip();
  }
  
  private Msg.Req readReq() throws Exception {
    waitData(8);
    Msg.Req req = new Msg.Req();
    req.id = mBuffer.getInt();
    req.nameLen = mBuffer.getInt();
    if (req.nameLen > 1024) {
      throw new Exception("invalid namelen");
    }
    req.name = readString(req.nameLen);
    return req;
  }
  
  private void writeStat(Msg.Stat stat) {
    byte[] bytes = new byte[16];
    ByteBuffer buffer = ByteBuffer.wrap(bytes);
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    buffer.putInt(stat.id);
    buffer.putInt(stat.mode);
    buffer.putInt(stat.size);
    buffer.putInt(stat.time);
    mPeer.enqueue(new AdbMessage(0, 0, 0, bytes));
  }

  private String readString(int len) throws InterruptedException {
    waitData(len);
    byte[] name = new byte[len];
    mBuffer.get(name);
    return new String(name);
  }

  @Override
  public int enqueue(AdbMessage message) {
    boolean result = mMessageQue.offerLast(message);
    assert (result);
    if (mMessageQue.remainingCapacity() == 0) {
      // We can not accept more message, return 1;
      synchronized (this) {
        assert (!mBlocking);
        mBlocking = true;
      }
      return 1;
    }
    return 0;
  }
  
  @Override
  public void ready() {
    mThread = new Thread(this, "AdbSync");
    mThread.start();
  }
  
  @Override
  public void close() {
    try {
      mThread.interrupt();
      mThread.join();
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
    super.close();
  }
  
  private void handleStat(Msg.Req req) {
    File file = new File (req.name);
    Msg.Stat msg = new Msg.Stat();
    if (file.exists()) {
      if (file.canRead())
        msg.mode |= 0444;
      if (file.canWrite())
        msg.mode |= 0222;
      if (file.canExecute())
        msg.mode |= 0111;
      msg.size = (int)file.length();
      msg.time = (int)file.lastModified();
    }
    writeStat(msg);
  }
  
  private void handleList(Msg.Req req) {
    
  }

  private void handleSend(Msg.Req req) {
    
  }
  
  private void handleRecv(Msg.Req req) {
    
  }
  
  @Override
  public void run() {
    try {
      while (true) {
        Msg.Req req = readReq();
        switch (req.id) {
          case Msg.ID_STAT: handleStat(req); break;
          case Msg.ID_LIST: handleList(req); break;
          case Msg.ID_SEND: handleSend(req); break;
          case Msg.ID_RECV: handleRecv(req); break;
          case Msg.ID_QUIT: return;
          default:
            throw new Exception("unknown command");
        }
      }
    } catch (Exception e) {
      e.printStackTrace();
    } finally {
      if (mPeer != null)
        mPeer.close();
    }
  }
}
