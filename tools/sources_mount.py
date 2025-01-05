#!/usr/bin/python3

import argparse
import subprocess
import getpass

DISK_PATH = 'F:\\WSL\\chrome_ohos.vhdx'

# Check if the disk is mounted
def is_mounted():
  command = [
    'mount',
  ]
  result = subprocess.run(command, capture_output=True, text=True)

  if result.returncode != 0:
    print("Command failed with return code", result.returncode)
    print(result.stderr)
    return False

  return '/mnt/wsl/sources' in result.stdout


# Mount the disk
def mount():
  if is_mounted():
    print("Already mounted")
    return

  command = [
    'wsl.exe',
    '--mount',
    '--vhd', DISK_PATH,
    '--partition', '1',
    '--name', 'sources',
    '--type', 'btrfs'
  ]
  result = subprocess.run(command, capture_output=True, text=True)

  if result.returncode != 0:
    print("Command failed with return code", result.returncode)
    print(result.stderr)
    command = [
      'wsl.exe',
      '--unmount',
      DISK_PATH
    ]
    result = subprocess.run(command, capture_output=True, text=True)
    return

  password = getpass.getpass("Enter your sudo password: ")
  command = [
    'sudo',
    '-S',  # Read the password from standard input
    'mount',
    '--bind',
    '/mnt/wsl/sources',
    '/home/penghuang/sources'
  ]
  result = subprocess.run(command, input=password + '\n', capture_output=True, text=True)

  if result.returncode != 0:
    print("Bind mount failed with return code", result.returncode)
    print(result.stderr)


# Unmount the disk
def unmount():
  command = [
    'wsl.exe',
    '--unmount',
    DISK_PATH
  ]
  result = subprocess.run(command, capture_output=True, text=True)

  if result.returncode != 0:
    print("Command failed with return code", result.returncode)
    print(result.stderr)

  command = [
    'sudo',
    'umount',
    '/home/penghuang/sources'
  ]
  result = subprocess.run(command, capture_output=True, text=True)

  if result.returncode != 0:
    print("Command failed with return code", result.returncode)
    print(result.stderr)


# Parse the command line arguments
def main():
  parser = argparse.ArgumentParser()
  subparsers = parser.add_subparsers(dest='command')

  # Create the parser for the "mount" command
  parser_mount = subparsers.add_parser('mount')

  # Create the parser for the "unmount" command
  parser_unmount = subparsers.add_parser('unmount')

  args = parser.parse_args()
  if args.command == 'mount':
    mount()
  elif args.command == 'unmount':
    unmount()
  else:
    parser.print_help()


if __name__ == '__main__':
  main()