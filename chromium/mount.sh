#!/usr/bin/bash

PROG=`basename $0`
echo $PROG
DIRS="	
	gpu
	base
	ui/gl
	ui/ozone
	components/viz
	third_party/skia
"


for f in $DIRS; do
if [ $PROG = "mount" ]; then
	mkdir -p $f
	sudo mount --bind $HOME/sources/chromium/src/$f $f
fi
if [ $PROG = "umount" ]; then
	sudo umount $f
fi
done

