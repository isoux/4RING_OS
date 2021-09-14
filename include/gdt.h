/*
 * 4RING_OS License: MIT
 *
 * gdt.h
 *
 * Global Descriptor Table definitions.
 * It is planned that all 8192 descriptors will be used,
 * due to many future TASKS and CALL GATES.
 * For now is 4608 gdt entries.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _GDT_H
#define _GDT_H

#include <typedef.h>

extern long long gdt[];

#define GDT_ENTRIES	4608	// number of entries (max is 8192)

/// GDT Descriptors Selectors
#define DPL_RING0	0
#define DPL_RING1	1
#define DPL_RING2	2
#define DPL_RING3	3

#define CORE_BASE 0x00
#define CORE_CODE 0x08		// Ring 0 code desc
#define CORE_DATA 0x10		// Ring 0 data desc

#define RING1_CODE	0x18	// Ring 1 code decs
#define RING1_DATA	0x20	// Ring 1 data decs

#define RING2_CODE	0x28	// Ring 2 code decs
#define RING2_DATA	0x30	// Ring 2 data decs

#define RING3_CODE	0x38	// Ring 3 code decs
#define RING3_DATA	0x40	// Ring 3 data decs

/* Reserved */
/*
#define X 0x48
#define X 0x50
*/

/* RING 1 TASKS */
#define TSS_DEVS_IRQ 0x58
#define LDT_DEVS_IRQ 0x60
#define TSS_DEVS_SCHED 0x68
#define LDT_DEVS_SCHED 0x70
/* RING 2 TASKS */
#define TSS_LIBS_IRQ 0x78
#define LDT_LIBS_IRQ 0x80
#define TSS_LIBS_SCHED 0x88
#define LDT_LIBS_SCHED 0x90
/* RING 3 TASKS */
#define TSS_MAIN_TASK 0x98
#define LDT_MAIN_TASK 0xA0
#define TSS_USERS_IRQ 0xA8
#define LDT_USERS_IRQ 0xB0

/* Reserved */
/*
#define X 0xB8
#define X 0xC0
#define X 0xC8
#define X 0xD0
#define X 0xD8
#define X 0xE0
#define X 0xE8
#define X 0xF0
*/

/* Data access trough segment registers */
#define DEVS_ACCES_DATA 0xF8	// For GS at RING 0 from call_gate

/* Call gates */
#define CG_TX_IRQ	0x100

/// End GDT Descriptors Selectors

#define DEVS_CODE	RING1_CODE + DPL_RING1 // DEVS sel. Ring1 code + 1 (dpl)
#define DEVS_DATA	RING1_DATA + DPL_RING1 // DEVS sel. Ring1 data + 1 (dpl)
#define LIBS_CODE	RING2_CODE + DPL_RING2 // LIBS sel. Ring2 code + 2 (dpl)
#define LIBS_DATA	RING2_DATA + DPL_RING2 // LIBS sel. Ring2 data + 2 (dpl)
#define USERS_CODE	RING3_CODE + DPL_RING3 // USERS sel. Ring3 code + 3 (dpl)
#define USERS_DATA	RING3_DATA + DPL_RING3 // USERS sel. Ring3 data + 3 (dpl)

#define TSS_GDT(selector) ((tss_des_struct *) & gdt[selector >> 3])
#define TSS_BUSY(selector) ((TSS_GDT(selector)->type))

#endif /* _GDT_H */
