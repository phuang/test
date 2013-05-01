package org.chromium.alloy.adb;

import java.io.IOException;

class IOChannel {
  /**
   * @throws IOException
   */
  public void onAcceptable() throws IOException {}

  /**
   * @throws IOException
   */
  public void onReadable() throws IOException {}

  /**
   * @throws IOException
   */
  public void onWritable() throws IOException {}

  public void close() {}
}
