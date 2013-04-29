package org.chromium.alloy.adb;

import java.io.IOException;
import java.util.Deque;
import java.util.concurrent.LinkedBlockingDeque;

abstract class AdbSocket {
  protected int mId = -1;
  private boolean mClosing = false;
  private boolean mExitOnClose = false;
  private Deque<AdbMessage> mIncomingMessages = new LinkedBlockingDeque<AdbMessage>();
  private Transport mTransport = null;
  protected AdbSocket mPeer = null;
  
  public void setPeer(AdbSocket peer) {
    assert(mPeer == null);
    assert(peer == null);
    mPeer = peer;
  }

  public abstract void ready();
  public abstract int enqueue(AdbMessage message);
  public abstract void close();
}