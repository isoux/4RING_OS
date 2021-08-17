#
# 4RING_OS License: MIT
#
# Common make definitions
#
# (C) Copyright 2021 Isa <isa@isoux.org>

CC = clang
LD = ld -m elf_i386
ARCH = i386
CPP	= clang -E -nostdinc -I ../include

CP := cp
RM := rm -rf
MKDIR := mkdir -pv

LOAD = sys_out/sys_load.o
GDT = core/gdt_mem.bin
GDT_RAM = gdt_
DEVS_RAM = devs
CORE_RAM = core
SYS = 4ring_os
IMG_FILES = img/*
CFG = boot/grub.cfg
DEVS = sys_out/devs
CORE = sys_out/core
IMG_PATH := img
ISO_PATH := iso
BOOT_PATH := $(ISO_PATH)/boot
GRUB_PATH := $(BOOT_PATH)/grub

# GRUB or GRUB2 : One of these must be defined!
export BOOT_WAY = GRUB

