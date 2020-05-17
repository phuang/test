#!/usr/bin/bash

PROG=`basename $0`
echo $PROG
DIRS="	
  android_webview
  cc
  base
  gpu
  components/viz
  content/browser/gpu
  third_party/skia
  third_party/vulkan_headers
  ui/base
  ui/gfx
  ui/gl
  ui/ozone
  sandbox
  services/service_manager/sandbox
"

CHROMIUM_DIR=`dirname $PWD`

for f in $DIRS; do
  if [ $PROG = "mount" ]; then
    mkdir -p $f
	  sudo mount --rbind $CHROMIUM_DIR/src/$f $f
  fi
  if [ $PROG = "umount" ]; then
	  sudo umount $f
  fi
done

