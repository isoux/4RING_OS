/*
 * 4RING_OS License: MIT
 *
 * 4libc/printf.c
 *
 * Part of 4libc static 4libc.a for 4RING_OS
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 *
 */

#include <libc_calls.h>

int printf(const char *fmt, ...)
{
  int i;
    i = __printf(&fmt);
    return i;
}


