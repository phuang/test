package org.chromium.alloy.adb;

import java.io.IOException;

interface IOChannel {
  /**
   * @throws IOException
   */
  public boolean onAcceptable();

  /**
   * @throws IOException
   */
  public boolean onReadable();

  /**
   * @return TODO
   */
  public boolean onWritable();

  public void onClose();
}
