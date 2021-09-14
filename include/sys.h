/*
 * 4RING_OS License: MIT
 *
 * sys.h
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _SYS_H
#define _SYS_H

#include <config.h>
#include <gdt.h>

#define MEM_LIMIT ((MEM_SIZE) / 0x1000) - 1 // 0x7ff

/* DEFINES OF RINGS */
#define RING0 0
#define	RING1 1
#define RING2 2
#define RING3 3

#define CORE RING0
#define	DEV  RING1
#define LIB  RING2
#define USR  RING3

#define CORE_SIZE 36*1024 	// 36Kb
#define DEVS_SIZE 36*1024 	// 36Kb
#define LIBS_SIZE 36*1024 	// 36Kb
#define RESERVE   0
#define USERS_SIZE 36*1024 //MEM_SIZE-GDT_SIZE-CORE_SIZE-DEVS_SIZE-LIBS_SIZE-RESERVE

#define GDT_SIZE	8*GDT_ENTRIES				// 36Kb
#define GDT_START	(MEM_SIZE) - (GDT_SIZE)		// 1F7000

#define CORE_START (GDT_START) - (CORE_SIZE)	// 1EE000
#define DEVS_START (CORE_START) - (DEVS_SIZE)	// 1E5000
#define LIBS_START (DEVS_START) - (LIBS_SIZE)	// 1DC000
#define USERS_START (LIBS_START) - (USERS_SIZE)	// 1D3000
//#define USERS_START 0x100000					// 1Mb

#define DEVS_STACK (CORE_START) - 4
#define LIBS_STACK (DEVS_START) - 4
#define USERS_STACK (LIBS_START) - 4

#define DEVS_LIMIT ((CORE_START) / 0x1000) - 1
#define LIBS_LIMIT ((DEVS_START) / 0x1000) - 1
#define USERS_LIMIT ((LIBS_START) / 0x1000) - 1

#endif /* _SYS_H */

