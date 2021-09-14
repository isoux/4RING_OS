/*
 * 4RING_OS License: MIT
 *
 * core_sched.c
 *
 * Main schedule functions at core (ring 0)
 *
 * The general requirements from the three less privileged rings
 * for task switching are processed here.
 * Each of the rings to which the task is further transferred, has
 * its own schedule procedures that assign priorities in their domain.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#include <typedef.h>
#include <gdt.h>
#include <gates.h>
#include <sched.h>
#include <sys.h>

#define REQ_BUF_SIZE 0xFF	// Request circular buffer size

/* Request circular buffer */
u_char req_buf[REQ_BUF_SIZE] = {0};

/* Table of indexed sub rings tasks selectors at gdt,
 * which contains the schedule procedures at less privileged rings
 */
u_short ring_sels[] = {	/* indexed rings selectors for schedules tasks */
	TSS_MAIN_TASK, //TSS_CORE_SEL
	TSS_DEVS_SCHED,
	TSS_LIBS_SCHED,
	TSS_MAIN_TASK //TSS_CORE_SEL //TSS_USERS_SCHED
};

/* fword argument (6 bytes)  for far call = lcall */
struct call_gate_args cg_args = {0,0,0};	/* call gate args */

/* Schedule structure with pointers for circular buffer */
struct sched_buf_ptrs sched_ptrs = {0,0};

/* Variable for Current ring executed task */
unsigned char cur_ring_task = 0;

/*
 * sc_sched_req = System call schedule request
 * This func is called from call gate descriptor from a less privileged
 * level and has a one input argument on stack.
 *
 * The argument can be (1.) a request to report a call from a less privileged
 * ring, or (2.) a request to delete a call (pointer at fifo circular buffer
 * that use sched function) from a lower ring, after the call has been
 * processed by the core scheduler.
 *
 * For now the func is in assembler code for critical speed reasons and
 * stack implementation when is called from call gate.
 */
__naked void sc_sched_req(void) {
__asm{
	  mov cx, ds					// save ds from the calling ring
	  xor eax,eax
	  mov eax, dword ptr[esp+0x8]	// get input arg from caller
	  mov edx, eax					// save arg at edx
	  mov ax, CORE_DATA
	  mov ds, ax					// set ds to core data structure
	  cmp edx, DECL_IRQ				// if arg is for register irq request
	jne clear						// else arg is for clear CLR_IRQ
	  mov bx,cx						// then get cpl
	  and bx,3
	  mov al, byte ptr [sched_ptrs.in_cur_p]
	  mov req_buf[0+eax*1], bl		// store cpl in circular buffer
	  mov ebp,[esp+0xc]				// get callers cur stack ptr
	  mov byte ptr es:[ebp+4],al	// store like out in_cur_p on callers stack
	  inc ax
	  cmp ax, REQ_BUF_SIZE			// if circular buffer overflow
	jne n1
	  mov ax,0						// then start again from 0
	n1:
	  mov byte ptr [sched_ptrs.in_cur_p], al // increment input cursor ptr
	  jmp short exit
	clear:							// CLR_IRQ
	  mov bl,0
	  mov byte ptr req_buf[0+edx*1],bl
	  mov al, byte ptr [sched_ptrs.out_cur_p]
	  inc al						// increment sched_ptrs.out_cur_p
	  cmp al, REQ_BUF_SIZE
	  jne n2
	  mov al,0
	  n2:
	  mov byte ptr [sched_ptrs.out_cur_p], al
	exit:
	  mov ds, cx					// restore ds from caled ring
	retf 4							// ret from call gate with out arg (4 byte)
  }
}

__inline_ static void long_jmp(void) {
  __asm{ ljmp [cg_args] }
}

/* Main system schedule called from timer interrupt 100 times in sec */
void sched(void) {

  u_char register out_ptr = sched_ptrs.out_cur_p;
  u_char register ring = req_buf[out_ptr];

  if (!((ring == RING0) && (cur_ring_task == RING0))) {
	  if (cur_ring_task != ring) {
		cur_ring_task = ring;
		cg_args.selector = ring_sels[ring];
		long_jmp();
		/* task is switched from FIFO request buffer to devs_sched_task */
	  }
  }
}
