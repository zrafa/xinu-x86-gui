#!/bin/bash

# create a msdos partition table on the device 
sudo parted -s /dev/sdb mklabel msdos

# create the EFI partition
sudo parted -s /dev/sdb mkpart primary 1MiB 551MiB

# activate the esp and boot flags
sudo parted -s /dev/sdb set 1 esp on
sudo parted -s /dev/sdb set 1 boot on

# format it with a fat32 filesystem
sudo mkfs.fat -F32 /dev/sdb1

# use all the remaining space on the device as data partition
sudo parted -s /dev/sdb mkpart primary 551MiB 100%

# format it with a fext4 filesystem
sudo mkfs.ext4 /dev/sdb2

# Create the mountpoints
sudo mkdir /tmp/{efi,data}

# Mount the EFI partition
sudo mount /dev/sdb1 /tmp/efi

# Mount the data partition
sudo mount /dev/sdb2 /tmp/data

# Installing legacy grub2
sudo grub-install \
  --target=i386-pc \
  --recheck \
  --boot-directory="/tmp/data/boot" /dev/sdb

# Installing grub for efi
sudo grub-install \
  --target=x86_64-efi \
  --recheck \
  --removable \
  --efi-directory="/tmp/efi" \
  --boot-directory="/tmp/data/boot" /dev/sdb

# Just to avoid the warnings at boot time
sudo mkdir /tmp/efi/boot
sudo mkdir /tmp/efi/.disk
sudo touch /tmp/efi/.disk/info

# Copy kernel
sudo cp -r xinu.elf /tmp/data/

sudo cp grub.cfg /tmp/data/boot/grub/

# Umount partitions
sudo umount /tmp/efi
sudo umount /tmp/data

# Remove mount points
sudo rm -rf /tmp/efi
sudo rm -rf /tmp/data

