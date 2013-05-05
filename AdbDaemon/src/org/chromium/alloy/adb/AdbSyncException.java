package org.chromium.alloy.adb;

class AdbSyncException extends Exception {
  private static final long serialVersionUID = -3601132696244713904L;

	public AdbSyncException(String message) {
    super(message);
  }
}