#
# 4RING_OS License: MIT
#
# Makefile for core
#
# (C) Copyright 2021 Isa <isa@isoux.org>

include ../defines.mk

CFLAGS = -m32 -march=$(ARCH) -c -Wall -fomit-frame-pointer -finline-functions -nostdinc \
	-fasm-blocks -masm=intel -fasm -fno-builtin -I ../include -fdeclspec  -fpack-struct
	
LDFLAGS = 

all:  gdt.elf core.o

c.o:
	$(CC) $(CFLAGS) -O \
	-o $*.o $<
	
OBJS = gdt.o idt.o timer.o interrupts.o core_sched.o # tasks.o
	
core.o: $(OBJS)
	$(LD) -r -o core.o $(OBJS)
	#objdump -D -z --disassembler-options=intel-mnemonic core.o  > core.dmp
	
gdt.elf: gdt_mem.o
	$(LD) -T ld_gdt.scr -Map gdt_mem.map -M gdt_mem.o -o gdt_mem.elf > gdt_mem.lnk
	objcopy -O binary -R .note -R .comment gdt_mem.elf gdt_mem.bin
	
clean:
	rm -f *.o *.lnk *.elf *.bin *.dmp -v tmp*

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do $(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile

# Dependencies

core_sched.o: core_sched.c ../include/typedef.h ../include/gdt.h \
  ../include/gates.h ../include/sched.h ../include/sys.h \
  ../include/config.h
gdt.o: gdt.c ../include/typedef.h ../include/gdt.h
gdt_mem.o: gdt_mem.c ../include/typedef.h ../include/gdt.h \
  ../include/sys.h ../include/config.h
idt.o: idt.c ../include/typedef.h ../include/gdt.h ../include/idt.h \
  ../include/gates.h
interrupts.o: interrupts.c ../include/typedef.h ../include/idt.h \
  ../include/gates.h ../include/gdt.h ../include/io.h \
  ../include/gas_regs.h ../include/devs/hard_int.h
tasks.o: tasks.c ../include/typedef.h ../include/gdt.h ../include/ldt.h \
  ../include/task.h ../include/sys.h ../include/config.h
timer.o: timer.c ../include/typedef.h ../include/gdt.h ../include/idt.h \
  ../include/gates.h
