#!/usr/bin/bash
image=$1
fs=$2
sudo cryptsetup luksOpen $image $image && \
sudo mount /dev/mapper/$image $fs && \
(cd fs; bash --rcfile .bashrc)
sudo umount $fs
sudo cryptsetup luksClose $image

