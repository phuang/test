#!/usr/bin/bash

QEMU=/home/penghuang/qemu/qemu-system-x86_64.exe

# CPU="-cpu EPYC-Milan-v1"
# CPU="-cpu max"
CPU="-cpu EPYC-Rome-v2"

IMAGE=d:\\VM\\Fedora\\Fedora.qcow2

# VGA="-device virtio-gpu-pci"
# VGA="-vga qxl"
VGA="-device virtio-vga"

NETWORK="-device virtio-net,netdev=vmnic -netdev user,id=vmnic"

# AUDIO="-device AC97"

POINTER="-usbdevice tablet"
# POINTER="-device virtio-tablet"

# KEYBOARD="-device virtio-keyboard"

${QEMU} \
  -display gtk,show-cursor=on,grab-on-hover=on,gl=off,zoom-to-fit=off \
  --accel whpx \
  ${CPU} \
  -smp 10 \
  -m 10G \
  -k en-us \
  -drive file=${IMAGE},if=virtio \
  ${VGA} \
  ${NETWORK} \
  ${AUDIO} \
  ${POINTER} \
  ${KEYBOARD} \
  ${NULL}
