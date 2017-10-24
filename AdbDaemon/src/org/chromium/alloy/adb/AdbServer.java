package org.chromium.alloy.adb;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

public class AdbServer implements IOChannel, Runnable {
  private static final int DEFAULT_ADB_PORT = 5555;

  private static AdbServer sServer = null;

  private static Thread sServerThread = null;

  private ServerSocketChannel mChannel = null;

  private Selector mSelector = null;

  private void init() throws IOException {
    assert (mChannel == null);

    mChannel = ServerSocketChannel.open();

    int port = DEFAULT_ADB_PORT;
    int count = 32;
    do {
      // Find a free port.
      count--;
      SocketAddress endpoint = new InetSocketAddress("127.0.0.1", port);
      try {
        mChannel.socket().bind(endpoint);
      } catch (IOException e) {
        port += 2;
        continue;
      }
      break;
    } while (count > 0);

    if (count == 0) {
      throw new IOException("Can not find a free port.");
    }
    assert (mChannel.socket().isBound());
    try {
      SocketChannel smartsocket = SocketChannel.open(new InetSocketAddress("127.0.0.1", 5037));
      String serverName = String.format("host:emulator:%d", port);
      String message = String.format("%04x%s", serverName.length(), serverName);
      ByteBuffer buffer = ByteBuffer.allocate(message.length());
      buffer.put(message.getBytes());
      buffer.flip();
      do {
        smartsocket.write(buffer);
      } while (buffer.hasRemaining());
      buffer = ByteBuffer.allocate(128);
      int result = smartsocket.read(buffer);
      buffer.flip();
      System.out.println("smartsocket: result=" + result + " " + new String(buffer.array()));

    } catch (IOException e) {
      e.printStackTrace();
    }

    mSelector = Selector.open();
    mChannel.configureBlocking(false);
    SelectionKey key = mChannel.register(mSelector, SelectionKey.OP_ACCEPT);
    key.attach(this);
  }

  private void loop() throws IOException {
    while (true) {
      int readyChannels = mSelector.select();
      if (readyChannels == 0)
        continue;
      Set<SelectionKey> selectedKeys = mSelector.selectedKeys();
      Iterator<SelectionKey> keyIterator = selectedKeys.iterator();
      while (keyIterator.hasNext()) {
        SelectionKey key = keyIterator.next();
        IOChannel iochannel = (IOChannel) key.attachment();
        boolean result = false;
        if (key.isAcceptable()) {
          result = iochannel.onAcceptable();
        } else if (key.isReadable()) {
          result = iochannel.onReadable();
        } else if (key.isWritable()) {
          result = iochannel.onWritable();
        }
        if (!result) {
          iochannel.onClose();
          key.cancel();
        }
        keyIterator.remove();
      }
    }
  }

  public Selector selector() {
    return mSelector;
  }

  @Override
  public boolean onAcceptable() {
    try {
      SocketChannel channel = mChannel.accept();
      channel.configureBlocking(false);
      Transport socket = new Transport(channel);
      socket.enableRead(true);
      System.out.println("Got an incoming connection, s = " + channel);
      return true;
    } catch (IOException e) {
      e.printStackTrace();
      return false;
    }
  }

  @Override
  public void run() {
    try {
      init();
      loop();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }

  public static AdbServer server() {
    return sServer;
  }

  public static void start() {
    assert (sServer == null);
    sServer = new AdbServer();
    sServerThread = new Thread(sServer, "Adb");
    sServerThread.start();
  }

  public static void shutdown() {
    try {
      sServerThread.join();
      sServerThread = null;
      sServer = null;
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
  }

  @Override
  public boolean onReadable() {
    return false;
  }

  @Override
  public boolean onWritable() {
    return false;
  }

  @Override
  public void onClose() {}
}
