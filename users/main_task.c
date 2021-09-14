/*
 * 4RING_OS License: MIT
 *
 * main_task.c
 *
 * For now main task are statically initialized at memory
 * space of users (ring 3).
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt.h>
#include <ldt.h>
#include <task.h>
#include <sys.h>

void main_task(void);

int cur_task = 0;

l_long ldt_core[2] =	{
	0x00c0fa00000007ff,	// Sel. 0x07 TASK_CODE limit=2047 (8Mb) base 0 gran=1
	0x00c0f200000007ff	// Sel. 0x0f TASK_DATA; 8Mb
};

long stack_tss_users[68];		// 4 * long size more for align
long stack_tss_users_krn[68];	// Stack for ring 0
long stack_tss_users_devs[68];	// Stack for ring 1
long stack_tss_users_libs[68];	// Stack for ring 2

struct stack_ptr p_stack_core = { & stack_tss_users[64]};
struct stack_ptr p_stack_core_krn = { & stack_tss_users_krn[64]};
struct stack_ptr p_stack_core_devs = { & stack_tss_users_devs[64]};
struct stack_ptr p_stack_core_libs = { & stack_tss_users_libs[64]};

struct tss_struct tss_users = {
	0,				// back link
	0,				// p_stack_core_krn.end_stack
	CORE_DATA,		// ss0
	0,DEVS_DATA,	// esp1, ss1
	0,LIBS_DATA,	// esp2, ss2
	0,				// cr3
	0,				// ptr of main_task,
	0x1200,			// eflags interupt enable IO = 1
	0,0,0,0,		// eax, ebx, ecx, edx
	0,				// ptr of p_stack_core.end_stack = esp
	0,0,0,			// ebp, esi, edi
	TASK_DATA, TASK_CODE, TASK_DATA, TASK_DATA, TASK_DATA, TASK_DATA,
					// es, cs, ss, ds, fs, gs
	LDT_MAIN_TASK,	// selector of ldt table
	0x8000000		// trace bitmap
};

void set_task_stacks(void) {

	long * p0 = (long *) & main_task;

	tss_users.task_stack = p_stack_core.end_stack;
	tss_users.core_stack = p_stack_core_krn.end_stack;
	tss_users.ring1_stack = p_stack_core_devs.end_stack;
	tss_users.ring2_stack = p_stack_core_libs.end_stack;
	tss_users.task = p0;
}

__naked void main_task(void) {
  __asm{
	mov eax, TASK_DATA
	mov ds, ax
	l1:
	jmp l1
  }
}

