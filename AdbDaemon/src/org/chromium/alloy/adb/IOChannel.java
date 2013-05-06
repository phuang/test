package org.chromium.alloy.adb;

import java.io.IOException;

interface IOChannel {
  public boolean onAcceptable();

  public boolean onReadable();

  public boolean onWritable();

  public void onClose();
}
