#
# 4RING_OS License: MIT
#
# Makefile for users
#
# (C) Copyright 2021 Isa <isa@isoux.org>

include ../defines.mk

CFLAGS = -m32 -march=$(ARCH) -c -Wall -fomit-frame-pointer -finline-functions -nostdinc \
	-fasm-blocks -masm=intel -fasm -fno-builtin -I ../include -fdeclspec  -fpack-struct \
	-D$(BOOT_WAY)
	
LDFLAGS =

all: users.o

c.o:
	$(CC) $(CFLAGS) -O -o $*.o $<
	
OBJS =  main_task.o
	
users.o: $(OBJS) 
	$(LD) -T users_ld.scr -Map users.map -M $(OBJS) -o users.o > users.lnk
	objcopy -O binary -R .note -R .comment users.o ../sys_out/users
	#objdump -D -z --disassembler-options=intel-mnemonic users.o  > users.dum

clean:
	rm -f *.o -v tmp*

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile