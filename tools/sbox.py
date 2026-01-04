#!/usr/bin/env python3
"""enter.py - safer Python replacement for enter.sh

Usage:
    sudo ./enter.py path/to/image.luks

Behavior mirrors the original shell script:
  - opens LUKS device with cryptsetup
  - mounts it under ./fs (or a user-specified mount point)
  - chdirs into the mount and launches an interactive bash using .bashrc if present
  - on exit (or error) unmounts and closes the luks device

Note: This script must be run as root because it calls cryptsetup and mount.
"""

from __future__ import annotations
import argparse
import os
import shutil
import signal
import subprocess
import sys
from typing import Optional

def run_as_root(cmd, *, check=True):
    if os.geteuid() != 0:
        cmd = ["sudo"] + cmd
    return run(cmd, check=check)

def run(cmd, *, check=True):
    print(f"+ {' '.join(cmd)}")
    result = subprocess.run(cmd)
    if check and result.returncode != 0:
        raise subprocess.CalledProcessError(result.returncode, cmd)
    return result.returncode


class EnterError(Exception):
    pass


def resolve_mapper_name(image: str) -> str:
    # Use basename of image (without dirs) as mapper name by default
    return os.path.splitext(os.path.basename(image))[0] or 'luks_device'


def open_luks(image: str, mapper: str):
    run_as_root(["cryptsetup", "luksOpen", image, mapper])


def close_luks(mapper: str):
    run_as_root(["cryptsetup", "luksClose", mapper], check=False)


def mount_mapper(mapper: str, mount_point: str):
    os.makedirs(mount_point, exist_ok=True)
    device = f"/dev/mapper/{mapper}"
    run_as_root(["mount", device, mount_point])


def unmount(mount_point: str):
    run_as_root(["umount", mount_point], check=False)


def launch_shell(mount_point: str):
    # Use .bashrc inside mount_point if present, otherwise start interactive bash
    rcfile = ".bashrc"
    # Change into mount point and run bash interactively
    oldcwd = os.getcwd()
    try:
        os.chdir(mount_point)
        if os.path.exists(rcfile):
            run(["bash", "--rcfile", ".bashrc"])  # will return when exited
        else:
            run(["bash"])  # fallback
    finally:
        os.chdir(oldcwd)


def parse_args(argv: Optional[list[str]] = None):
    p = argparse.ArgumentParser(description="Open a LUKS image, mount it, enter a shell, then clean up.")
    p.add_argument("image", help="path to the LUKS image or device")
    p.add_argument("-m", "--mapper", help="name for /dev/mapper/<name> (defaults to basename of image)")
    p.add_argument("--mount-point", default="./fs", help="mount point to use (default: ./fs)")
    return p.parse_args(argv)


def main(argv: Optional[list[str]] = None):
    args = parse_args(argv)

    mapper = args.mapper or resolve_mapper_name(args.image)
    mount_point = args.mount_point

    # Graceful cleanup on signals
    stop = {"received": False}

    def _signal_handler(signum, frame):
        print(f"Received signal {signum}; will clean up and exit.")
        stop["received"] = True

    signal.signal(signal.SIGINT, _signal_handler)
    signal.signal(signal.SIGTERM, _signal_handler)

    mounted = False
    opened = False

    try:
        open_luks(args.image, mapper)
        opened = True

        mount_mapper(mapper, mount_point)
        mounted = True

        if stop["received"]:
            print("Exiting before shell due to signal")
            return 1

        launch_shell(mount_point)

        return 0

    except subprocess.CalledProcessError as e:
        print(f"Command failed: {e}")
        return e.returncode if isinstance(e.returncode, int) else 1
    except EnterError as e:
        print(f"ERROR: {e}")
        return 2
    except Exception as e:
        print(f"Unexpected error: {e}")
        return 1
    finally:
        # Attempt cleanup even if earlier steps failed; ignore errors but report them
        if mounted:
            try:
                print("Unmounting mount point...")
                unmount(mount_point)
            except Exception as e:
                print(f"Warning: failed to unmount {mount_point}: {e}")
        if opened:
            try:
                print("Closing LUKS mapper...")
                close_luks(mapper)
            except Exception as e:
                print(f"Warning: failed to close LUKS mapper {mapper}: {e}")


if __name__ == "__main__":
    sys.exit(main())
