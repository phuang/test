package org.chromium.alloy.adb;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;
import java.util.Vector;


class ShellService extends AdbThreadSocket {
  private String mCurrentDir = "/";
  private HashMap<String, String> mEnvs = new HashMap<String, String>();
  private Vector<String[]> mCommands = null;
  private int mLastResult = 0;

  public ShellService(final String commands) throws IOException {
  	super("AdbShell");
    mEnvs.put("EXTERNAL_STORAGE", "/mnt/sdcard");
    mEnvs.put("ANDROID_DATA", "/data");
    mEnvs.put("ANDROID_ROOT", "/system");
    mCommands = commands.isEmpty() ? null : parseCommands(commands);
  }

  private String getEnv(final String name) {
    String value = mEnvs.get(name);
    return value != null ? value : "";
  }

  private Vector<String[]> parseCommands(String commands) throws IOException {
    Vector<String[]> cmds = new Vector<String[]>();
    Vector<String> cmd = new Vector<String>();
    char[] chars = commands.toCharArray();
    StringBuilder sb = new StringBuilder();
    char quote = 0;
    for (int i = 0; i < chars.length; ++i) {
      char c = chars[i];
      switch(c) {
        case '"':
        case '\'':
          if (quote == 0) {
            quote = c;
          } else if (quote == c) {
            quote = 0;
          } else {
            sb.append(c);
          }
          break;
        case '\\':
          if (quote == 0) {
            sb.append(chars[++i]);
          } else if (quote == '\'') {
            sb.append('\\');
          } else if (quote == '"') {
            char next = chars[i+1];
            if (next == '"' || next == '$' || next == '\\') {
              sb.append(next);
              i++;
            } else {
              sb.append('\\');
            }
          }
          break;
        case ' ':
          if (quote == 0) {
            String s = sb.toString();
            if (!s.isEmpty())
              cmd.add(s);
            sb = new StringBuilder();
          } else {
            sb.append(c);
          }
          break;
        case ';':
          if (quote == 0) {
            String s = sb.toString();
            if (!s.isEmpty())
              cmd.add(s);
            sb = new StringBuilder();
            if (cmd.size() != 0) {
              cmds.add(cmd.toArray(new String[0]));
              cmd.clear();
            }
          } else {
            sb.append(c);
          }
          break;
        case '$':
          if (quote == '\'' || (i + 1) == chars.length) {
            sb.append('$');
          } else {
            char nextChar = chars[i + 1];
            if (nextChar == '?') {
              sb.append(String.format("%d", mLastResult));
            } else if (nextChar == '$') {
              sb.append("1");
            } else if ((nextChar >= 'a' && nextChar <= 'z') ||
                       (nextChar >= 'A' && nextChar <= 'Z')) {
              int begin = i + 1;
              int j = i + 1;
              char cc;
              do {
                j++;
                if (j == chars.length)
                  break;
                cc = chars[j];
              } while ((cc >= 'a' && cc <= 'z') ||
                       (cc >='A' && cc <= 'Z') ||
                       (cc >='0' && cc <='9') || cc == '_');
              sb.append(getEnv(new String(chars, begin, j - begin)));
              i = j - 1;
            } else {
              sb.append('$');
            }
          }
          break;
        default:
          sb.append(c);
          break;
      }
    }

    if (quote != 0) {
      print(String.format("unexpected EOF while looking for matching `%c'\n", quote));
      cmds.clear();
      return cmds;
    }

    String s = sb.toString();
    if (!s.isEmpty())
      cmd.add(s);
    if (!cmd.isEmpty())
      cmds.add(cmd.toArray(new String[0]));
    return cmds;
  }

  private void cdMain(String[] args) throws IOException {
    String newPath = null;
    if (args.length >= 2) {
      newPath = args[1];
      if (!newPath.startsWith("/")) {
        newPath = mCurrentDir + "/" + newPath;
      }
    }
    if (newPath == null)
      newPath = getEnv("HOME");
    if (newPath.isEmpty()) {
      print("cd: no home directory (HOME not set)\n");
      return;
    }

    File directory = new File(newPath);
    if (!directory.exists()) {
      print(String.format("cd: %s: No such file or directory\n", newPath));
      return;
    }

    if (!directory.isDirectory()) {
      print(String.format("cd: %s: Not a directory\n", newPath));
      return;
    }

    mCurrentDir = directory.getAbsolutePath();
    print("\n");
  }

  private void echoMain(String[] args) throws IOException {
    StringBuffer sb = new StringBuffer();
    if (args.length >= 2) {
      sb.append(args[1]);
      for (int i = 2; i < args.length; i++) {
        sb.append(" ");
        sb.append(args[i]);
      }
    }
    sb.append("\n");
    print(sb.toString());
  }

  private void exportMain(String[] args) throws IOException {
    if (args.length == 1) {
      Iterator<Entry<String,String>> iterator = mEnvs.entrySet().iterator();
      while (iterator.hasNext()) {
        Entry<String, String> entry = iterator.next();
        print(String.format("export -x %s=\"%s\"\n",
            entry.getKey(), entry.getValue()));
      }
      return;
    } else {
      for (int i = 1; i < args.length; ++i) {
        String[] s = args[i].split("=", 2);
        if (s.length == 2) {
          mEnvs.put(s[0], s[1]);
        } else {
          if (!mEnvs.containsKey(s[0]))
            mEnvs.put(s[0], "");
        }
      }
    }
  }

  private void getpropMain(String[] args) throws IOException {
    print("\n");
  }

  private void logcatMain(String[] args) throws IOException {
    print("\n");
  }

  private void lsMain(String[] args) throws IOException {
    String path = mCurrentDir;
    File directory = new File(path);
    for (String file: directory.list()) {
      print(file + "\n");
    }
  }

  private void pmMain(String[] args) throws IOException {
    print("shell: pm: command not found\n");
  }

  private void pwdMain(String[] args) throws IOException {
    print(mCurrentDir + "\n");
  }

  private void rmMain(String[] args) throws IOException {
    for (int i = 1; i < args.length; ++i) {
      File file = new File(args[i]);
      if (!file.exists()) {
        print(String.format("rm failed for %s, No such file or directory\n", args[i]));
        return;
      }
      if (file.isDirectory()) {
        print(String.format("rm failed for %s, Is a directory\n", args[i]));
        return;
      }
      if (!file.delete()) {
        print(String.format("rm failed for %s\n", args[i]));
      }
    }
  }

  private void execute(String[] args) throws IOException {
    if ("cd".equals(args[0])) {
      cdMain(args);
    } else if ("echo".equals(args[0])) {
      echoMain(args);
    } else if ("export".equals(args[0])) {
      exportMain(args);
    } else if ("getprop".equals(args[0])) {
      getpropMain(args);
    } else if ("logcat".equals(args[0])) {
      logcatMain(args);
    } else if ("ls".equals(args[0])) {
      lsMain(args);
    } else if ("pm".equals(args[0])) {
      pmMain(args);
    } else if ("pwd".equals(args[0])) {
      pwdMain(args);
    } else if ("rm".equals(args[0])) {
      rmMain(args);
    } else {
      print(String.format("shell: %s: command not found\n", args[0]));
    }
  }

  private void print(final String data) throws IOException {
    ByteBuffer buffer = ByteBuffer.wrap(data.getBytes());
  	while (buffer.hasRemaining())
  		output().write(buffer);
  }

	@Override
	protected void loop() {
		try {
	    Iterator<String[]> iterator = mCommands.iterator();
	    while (iterator.hasNext()) {
	      String[] args = iterator.next();
	      if ("exec".equals(args[0])) {
	        execute(Arrays.copyOfRange(args, 1, args.length));
	        break;
	      } else {
	        execute(args);
	      }
	    }
		} catch (IOException e) {
			e.printStackTrace();
		}
  }
}