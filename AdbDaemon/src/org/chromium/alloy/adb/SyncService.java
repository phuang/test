package org.chromium.alloy.adb;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

class SyncService extends AdbThreadSocket {
  private static final int SYNC_DATA_MAX = 64 * 1024;
  private ByteBuffer mBuffer = null;

  private static class Msg {
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

    private static String idToString(int id) {
      switch (id) {
        case ID_STAT: return "STAT";
        case ID_LIST: return "LIST";
        case ID_ULNK: return "ULNK";
        case ID_SEND: return "SEND";
        case ID_RECV: return "RECV";
        case ID_DENT: return "DENT";
        case ID_DONE: return "DONE";
        case ID_DATA: return "DATA";
        case ID_OKAY: return "OKAY";
        case ID_FAIL: return "FAIL";
        case ID_QUIT: return "QUIT";
        default: return "UNKN";
      }
    }

    static class Req {
      int id = 0;
      int nameLen = 0;
      String name = null;
      
      @Override
      public String toString() {
      	return String.format("%s: %d %s", Msg.idToString(id), nameLen, name);
      }
    }

    static class Stat {
      int id = Msg.ID_STAT;
      int mode = 0;
      int size = 0;
      int time = 0;
      
      @Override
      public String toString() {
      	return String.format("STAT: 0%04o %ld %ld",
      			mode, ((long)size) & 0xffffffff, ((long)time) & 0xffffffff);
      }
    }

    @SuppressWarnings("unused")
    static class Dent {
      int id = 0;
      int mode = 0;
      int size = 0;
      int time = 0;
      int nameLen = 0;
    }

    static class Data {
      int id = 0;
      int size = 0;
      byte[] data = null;

      @Override
      public String toString() {
      	return String.format("%s: %d", Msg.idToString(id), size);
      }
    }

    static class Status {
      int id = 0;
      int msgLen = 0;

      @Override
      public String toString() {
      	return String.format("%s: %d", Msg.idToString(id), msgLen);
      }
    }
  }

  public SyncService() throws IOException {
  	super("AdbSync");
    mBuffer = ByteBuffer.allocate(SYNC_DATA_MAX * 2);
    mBuffer.order(ByteOrder.LITTLE_ENDIAN);
    mBuffer.flip();
  }

  private void waitData(int len) throws InterruptedException, IOException {
    if (len > SYNC_DATA_MAX)
      throw new IOException("wait data len is oversize");

    if (mBuffer.remaining() >= len)
      return;

    mBuffer.compact();
    do {
    	input().read(mBuffer);
    } while (mBuffer.position() < len);
    mBuffer.flip();
  }

  private byte[] readBytes(int len) throws InterruptedException, IOException {
    waitData(len);
    byte[] bytes = new byte[len];
    mBuffer.get(bytes);
    return bytes;
  }

  private String readString(int len) throws InterruptedException, IOException {
    return new String(readBytes(len));
  }

  private Msg.Req readReq() throws Exception {
    waitData(8);
    Msg.Req req = new Msg.Req();
    req.id = mBuffer.getInt();
    req.nameLen = mBuffer.getInt();
    if (req.nameLen > 1024) {
      throw new AdbSyncException("invalid namelen");
    }
    req.name = readString(req.nameLen);
    return req;
  }

  private Msg.Data readData() throws Exception {
    waitData(8);
    Msg.Data data = new Msg.Data();
    data.id = mBuffer.getInt();
    data.size = mBuffer.getInt();
    if (data.id == Msg.ID_DONE)
      return data;
    if (data.id == Msg.ID_DATA) {
      if (data.size > SYNC_DATA_MAX)
        throw new AdbSyncException("oversize data message");
      data.data = readBytes(data.size);
      return data;
    }

    throw new AdbSyncException("invalid data message");
  }

  private void write(ByteBuffer buffer) throws IOException {
  	while (buffer.hasRemaining()) {
  		output().write(buffer);
  	}
  }
  
  private void writeStat(Msg.Stat stat) throws IOException {
    ByteBuffer buffer = ByteBuffer.allocate(16);
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    buffer.putInt(stat.id);
    buffer.putInt(stat.mode);
    buffer.putInt(stat.size);
    buffer.putInt(stat.time);
    buffer.flip();
    write(buffer);
  }

	private void writeStatus(Msg.Status status) throws IOException {
    ByteBuffer buffer = ByteBuffer.allocate(8);
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    buffer.putInt(status.id);
    buffer.putInt(status.msgLen);
    buffer.flip();
    write(buffer);
  }

  private void writeData(Msg.Data data) throws IOException {
    assert (data.size <= data.data.length);
    ByteBuffer buffer = ByteBuffer.allocate(8 + data.size);
    buffer.order(ByteOrder.LITTLE_ENDIAN);
    buffer.putInt(data.id);
    buffer.putInt(data.size);
    buffer.put(data.data, 0, data.size);
    buffer.flip();
    write(buffer);
  }

  private void handleStat(Msg.Req req) throws IOException {
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
  	// TODO(penghuang): support list.
  }

  private void handleSend(Msg.Req req) throws Exception {
    String[] tmp = req.name.split(",", 2);
    String path = tmp[0];
    int mode = 0777;
    if (tmp.length == 2) {
      mode = Integer.parseInt(tmp[1]);
      mode &= 0777;
    }

    // Copy user permission bits to "group" and "other" permissions.
    handleSendFile(path, mode);
  }

  private void handleSendFile(String path, int mode) throws Exception {
    File file = new File(path);
    long timestamp = 0;

    if (file.exists() || !file.createNewFile())
      throw new AdbSyncException(String.format("create file %s failed", path));

    file.setExecutable((mode & 0111) != 0);
    file.setWritable((mode & 0222) != 0);
    file.setReadable((mode & 0444) != 0);

    FileOutputStream fstream = new FileOutputStream(file);
    while (true) {
      Msg.Data data = readData();
      if (data.id == Msg.ID_DONE) {
        timestamp = data.size & 0xffffffff;
        break;
      }
      try {
        if (fstream != null)
          fstream.write(data.data);
      } catch (IOException e) {
        try {
          fstream.close();
        } catch (IOException ce) {}
        fstream = null;
        file.delete();
      }
    }

    if (fstream != null) {
      try {
        fstream.close();
      } catch (IOException e) {}
      file.setLastModified(timestamp);
      Msg.Status status = new Msg.Status();
      status.id = Msg.ID_OKAY;
      writeStatus(status);
    }
  }

  private void handleRecv(Msg.Req req) {
  	// TODO(penghuang): support recv.
  }

  @Override
  protected void loop() {
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
    } catch (AdbSyncException ase) {
      Msg.Data msg = new Msg.Data();
      msg.id = Msg.ID_FAIL;
      msg.data = ase.getMessage().getBytes();
      msg.size = msg.data.length;
      try {
	      writeData(msg);
      } catch (IOException ie) {
	      ie.printStackTrace();
      }
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}