package org.chromium.alloy.adb.shell;

import java.nio.channels.Pipe.SinkChannel;
import java.nio.channels.Pipe.SourceChannel;
import java.util.Map;

public class Command {
  private String mCwd = null;
  private String[] mArgs = null;
  private Map<String, String> mEnvs = null;
  private SourceChannel mInput = null;
  private SinkChannel mOutput = null;
  
  protected Command(String[] args, String cwd, Map<String, String>envs,
      SourceChannel input, SinkChannel output) {
    mCwd = cwd;
    mArgs = args;
    mEnvs = envs;
    mInput = input;
    mOutput = output;
  }
  
  protected SourceChannel input() {
    return mInput;
  }
  
  protected SinkChannel output() {
    return mOutput;
  }
  
  protected void write(String data) {  
  }
  
  protected final void execute() {
  }
}
