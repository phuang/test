package org.chromium.alloy.adb;

public class Adb {
  /**
   * @param args
   */
  public static void main(String[] args) {
    AdbServer.start();
    AdbServer.shutdown();
  }
}
