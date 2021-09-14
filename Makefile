#
# 4RING_OS License: MIT
#
# Makefile for system (iso or img)
#
# (C) Copyright 2021 Isa <isa@isoux.org>

include defines.mk

CFLAGS = -m32 -march=$(ARCH) -c -Wall -fomit-frame-pointer -finline-functions -nostdinc \
	-fasm-blocks -masm=intel -fasm -fno-builtin -I ../include -fdeclspec  -fpack-struct \
	
LDFLAGS =

ifeq ($(BOOT_WAY), GRUB2)
	ISO = make_iso
	#IMG = del_img
else
	IMG = make_img
	#ISO = del_iso
endif

.c.o:
	$(CC) $(CFLAGS) -nostdinc -Iinclude -c -o $*.o $<
	
SYS_OBJS = init/init.o init/main.o core/core.o devs/devs.o users/users.o core/gdt_mem.o

all: Image 

sys_out/sys_load: boot/load.o
	$(LD) -M -T ld_load.scr boot/load.o  -o sys_out/sys_load.o > sys_load.lnk
	objcopy -O binary -R .note -R .comment sys_out/sys_load.o sys_out/sys_load
	#objdump -D -z --disassembler-options=intel-mnemonic sys_out/sys_load.o > sys_load.dum

sys_out/system: $(SYS_OBJS)
	$(LD) -M -T ld_core.scr  $(SYS_OBJS) -o sys_out/core.o > core.lnk
	objcopy -O binary -R .note -R .comment sys_out/core.o sys_out/core
	#objdump -D -z --disassembler-options=intel-mnemonic sys_out/core.o > core.dum

Image: sys_out/system sys_out/sys_load $(ISO) $(IMG) #sys_out/system

del_iso:
	$(RM) $(SYS).iso
	
make_iso: sys_out/sys_load.o
	(make del_iso)
	$(MKDIR) $(GRUB_PATH)
	$(CP) $(LOAD) $(BOOT_PATH)/$(SYS)
	$(CP) $(CFG) $(GRUB_PATH)
	$(CP) $(GDT) $(BOOT_PATH)/$(GDT_RAM)
	$(CP) $(DEVS) $(BOOT_PATH)/$(DEVS_RAM)
	$(CP) $(CORE) $(BOOT_PATH)/$(CORE_RAM)
	$(CP) $(USRS) $(BOOT_PATH)/$(USRS_RAM)
	grub-file --is-x86-multiboot $(BOOT_PATH)/$(SYS)
	grub-mkrescue --compress=no -o $(SYS).iso $(ISO_PATH)
	
del_img:
	$(RM) $(IMG_FILES)

make_img:
	(make del_img)
	$(CP) $(LOAD) $(IMG_PATH)/$(SYS)
	$(CP) $(GDT) $(IMG_PATH)/$(GDT_RAM)
	$(CP) $(DEVS) $(IMG_PATH)/$(DEVS_RAM)
	$(CP) $(CORE) $(IMG_PATH)/$(CORE_RAM)
	$(CP) $(USRS) $(IMG_PATH)/$(USRS_RAM)

boot/load.o:
	(cd boot; make)
	
init/init.o:
	(cd init; make)
	
core/core.o:
	(cd core; make)
	
devs/devs.o:
	(cd devs; make)
	
#libs/libs.o:
	#(cd libs; make)
	
users/users.o:
	(cd users; make)
	
qemu_iso_grub2:
	#qemu-system-x86_64 -m 9M -cdrom 4ring_os.iso -machine pc-0.13
	qemu-system-i386 -m 9M -cdrom 4ring_os.iso -machine pc-0.13
		
bochs_iso_grub2:
	bochs

qemu_floppy:
	qemu-system-i386 -drive format=raw,file=/dev/sdX,index=0,if=floppy -m 9M -machine pc-0.13
	
write_floppy:
	dd if=tools/GRUB1/grub1.img of=/dev/sdX bs=512 # Be careful with the choice of device!!!
	
write_iso:
	dd if=4ring_os.iso of=/dev/sdX # Be careful with the choice of device!!!
		
clean:
	rm -f sys_out/* *.dum *.lnk 
	#rm -r iso/boot/grub
	#rm -f iso/boot/*

clean_all_objs:
	(cd boot; make clean)
	(cd core; make clean)
	(cd devs; make clean)
	(cd init; make clean)
	(cd users; make clean)
	(make clean)
	
dep:
	(cd boot; make dep)
	(cd core; make dep)
	(cd devs; make dep)
	(cd init; make dep)
