#!/usr/bin/python3

import subprocess
import sys
import os.path
import re

SYMBOLIZER = "/home/penghuang/sources/chrome-prebuilts-ohos/llvm/bin/llvm-symbolizer"
LIBRARY = "/home/penghuang/sources/chromium/src/out/Release/lib.unstripped/libcontent_shell_content_view.so"
BASENAMES = "/home/penghuang/sources/chromium/src/out/Release/../../"

def main():
  args = [
    SYMBOLIZER,
    "-e",
    LIBRARY,
    '--pretty-print',
    '--basenames',
  ]
  process = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=sys.stdout, stderr=sys.stderr)
  pattern = r"(?<=pc )([0-9a-fA-F]+)"
  for line in sys.stdin.readlines():
    match = re.search(pattern, line)
    if match:
      address = match.group(1)
      process.stdin.write(("0x%s\n" % address).encode("utf-8"))
  process.stdin.flush()
  process.stdin.close()
  process.wait()

if __name__ == "__main__":
  sys.exit(main())
