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
	ui/base
	ui/gfx
	ui/gl
	ui/ozone
"


for f in $DIRS; do
if [ $PROG = "mount" ]; then
	mkdir -p $f
	sudo mount --rbind $HOME/sources/chromium/src/$f $f
fi
if [ $PROG = "umount" ]; then
	sudo umount $f
fi
done

