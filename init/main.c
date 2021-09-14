/*
 * 4RING_OS License: MIT
 *
 * main.c
 *
 * Here is turning on paging and preparing to jump at main funct.
 * Main func prepares all initializations on all rings and entering
 * on main_core_task which is currently at ring 1 ring (devs)...
 *
 * setup_paging function is taken from Linux 0.01
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 *
 */

#include <typedef.h>
#include <gdt.h>

extern void Devs_init(long *);
extern long * p_gdt;

int main(void);
void enter_main_core_task(void);
//void enter_user(void);
void setup_paging(void);

#define PAGE_SIZE		4096
#define PG_TAB_ENTRY	1024

/* Follows pointers for paging directorium (1) and page tables(2)
 * After the initialization the first 0x3000 bytes
 * of code will be overwritten by these page tables
 */
int * pg_dir0 = (int *) 0x00000000;//Ptr of page dir starts from addr 0x00
int * pg_tab0 = (int *) 0x00001000;//Ptr of page table for first 4Mb(4096 bytes)
int * pg_tab1 = (int *) 0x00002000;//Ptr of page table for another 4Mb = 8Mb
int * pg_tab2 = (int *) 0x00003000;//Ptr of page table for another 4Mb

//long * _gdt1 = (long *) &gdt[0];

__naked void after_pg_tables() {
__asm{
	push 0
	push 0
	push 0
	lea eax, l1
	push eax
	lea eax, main
	push eax
	jmp setup_paging
	l1:
	jmp l1
	}
}

int main(void) {
	
	Devs_init(p_gdt);
	// Libs_init(p_gdt);
	// Users_init(p_gdt);
	enter_main_core_task();
	// enter_user();
}

/* Entering the task which selector is in GDT, With long jump asm instruction.
 * Currently task is in ring 1 (devs) with turned on interrupt flag and IO
 * privileged lever at ring 1 to be able to taking interrupts from keyboard
 * and so on, since all devices are at ring 1.
 */
__naked void enter_main_core_task(void) {
__asm{
   ljmp TSS_MAIN_TASK, 0 // core_task at tasks.c
  }
}

__naked void setup_paging(void) {
__asm{
	mov ecx, PG_TAB_ENTRY*3
	xor eax, eax
	xor edi, edi
	cld
	rep stosd
	mov ebx, [pg_dir0]
	mov edx, [pg_tab0]
	add edx, 7
	mov dword ptr [ebx], edx
	mov edx, [pg_tab1]
	add edx, 7
	mov dword ptr [ebx+4], edx
	mov edi,	[pg_tab1]
	add edi, 4092
	mov eax, 0x7ff007
	std
	l1:
	stosd
	sub eax, PAGE_SIZE
	jnl l1
	xor eax, eax
	mov cr3, eax
	mov eax, cr0
	or eax, 0x80000000
	mov cr0,eax
	ret
	}
}


