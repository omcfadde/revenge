/*
 * $Id$
 * Copyright (C) 2007  Oliver McFadden <z3ro.geek@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <asm/ioctl.h>
#include <asm/unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "ioctl.h"
#include "ioctl_blob.h"
#include "ioctl_drm.h"
#include "ioctl_radeon.h"
#include "main.h"

#ifndef MAX_ERRNO
#define MAX_ERRNO	4095
#endif

#if defined (__i386__)

/*
 * user-visible error numbers are in the range -1 - -MAX_ERRNO: see
 * <asm-i386/errno.h>
 */
#define __syscall_return(type, res) \
do { \
	if ((unsigned long)(res) >= (unsigned long)(-MAX_ERRNO)) { \
		errno = -(res); \
		res = -1; \
	} \
	return (type) (res); \
} while (0)

/* XXX - _foo needs to be __foo, while __NR_bar could be _NR_bar. */
#define _syscall0(type,name) \
type name(void) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (__NR_##name)); \
__syscall_return(type,__res); \
}

#define _syscall1(type,name,type1,arg1) \
type name(type1 arg1) \
{ \
long __res; \
__asm__ volatile ("push %%ebx ; movl %2,%%ebx ; int $0x80 ; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"ri" ((long)(arg1)) : "memory"); \
__syscall_return(type,__res); \
}

#define _syscall2(type,name,type1,arg1,type2,arg2) \
type name(type1 arg1,type2 arg2) \
{ \
long __res; \
__asm__ volatile ("push %%ebx ; movl %2,%%ebx ; int $0x80 ; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"ri" ((long)(arg1)),"c" ((long)(arg2)) \
	: "memory"); \
__syscall_return(type,__res); \
}

#define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
long __res; \
__asm__ volatile ("push %%ebx ; movl %2,%%ebx ; int $0x80 ; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"ri" ((long)(arg1)),"c" ((long)(arg2)), \
		  "d" ((long)(arg3)) : "memory"); \
__syscall_return(type,__res); \
}

#define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
type name (type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
{ \
long __res; \
__asm__ volatile ("push %%ebx ; movl %2,%%ebx ; int $0x80 ; pop %%ebx" \
	: "=a" (__res) \
	: "0" (__NR_##name),"ri" ((long)(arg1)),"c" ((long)(arg2)), \
	  "d" ((long)(arg3)),"S" ((long)(arg4)) : "memory"); \
__syscall_return(type,__res); \
}

#define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
	  type5,arg5) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5) \
{ \
long __res; \
__asm__ volatile ("push %%ebx ; movl %2,%%ebx ; movl %1,%%eax ; " \
                  "int $0x80 ; pop %%ebx" \
	: "=a" (__res) \
	: "i" (__NR_##name),"ri" ((long)(arg1)),"c" ((long)(arg2)), \
	  "d" ((long)(arg3)),"S" ((long)(arg4)),"D" ((long)(arg5)) \
	: "memory"); \
__syscall_return(type,__res); \
}

#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
	  type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
long __res; \
  struct { long __a1; long __a6; } __s = { (long)arg1, (long)arg6 }; \
__asm__ volatile ("push %%ebp ; push %%ebx ; movl 4(%2),%%ebp ; " \
                  "movl 0(%2),%%ebx ; movl %1,%%eax ; int $0x80 ; " \
                  "pop %%ebx ;  pop %%ebp" \
	: "=a" (__res) \
	: "i" (__NR_##name),"0" ((long)(&__s)),"c" ((long)(arg2)), \
	  "d" ((long)(arg3)),"S" ((long)(arg4)),"D" ((long)(arg5)) \
	: "memory"); \
__syscall_return(type,__res); \
}

#elif defined (__x86_64__)

/* user-visible error numbers are in the range -1 - -MAX_ERRNO */

#define __syscall_clobber "r11","rcx","memory"

#define __syscall_return(type, res) \
do { \
	if ((unsigned long)(res) >= (unsigned long)(-MAX_ERRNO)) { \
		errno = -(res); \
		res = -1; \
	} \
	return (type) (res); \
} while (0)

#define __syscall "syscall"

#define _syscall0(type,name) \
type name(void) \
{ \
long __res; \
__asm__ volatile (__syscall \
	: "=a" (__res) \
	: "0" (__NR_##name) : __syscall_clobber ); \
__syscall_return(type,__res); \
}

#define _syscall1(type,name,type1,arg1) \
type name(type1 arg1) \
{ \
long __res; \
__asm__ volatile (__syscall \
	: "=a" (__res) \
	: "0" (__NR_##name),"D" ((long)(arg1)) : __syscall_clobber ); \
__syscall_return(type,__res); \
}

#define _syscall2(type,name,type1,arg1,type2,arg2) \
type name(type1 arg1,type2 arg2) \
{ \
long __res; \
__asm__ volatile (__syscall \
	: "=a" (__res) \
	: "0" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2)) : __syscall_clobber ); \
__syscall_return(type,__res); \
}

#define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
long __res; \
__asm__ volatile (__syscall \
	: "=a" (__res) \
	: "0" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2)), \
		  "d" ((long)(arg3)) : __syscall_clobber); \
__syscall_return(type,__res); \
}

#define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
type name (type1 arg1, type2 arg2, type3 arg3, type4 arg4) \
{ \
long __res; \
__asm__ volatile ("movq %5,%%r10 ;" __syscall \
	: "=a" (__res) \
	: "0" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2)), \
	  "d" ((long)(arg3)),"g" ((long)(arg4)) : __syscall_clobber,"r10" ); \
__syscall_return(type,__res); \
}

#define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
	  type5,arg5) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5) \
{ \
long __res; \
__asm__ volatile ("movq %5,%%r10 ; movq %6,%%r8 ; " __syscall \
	: "=a" (__res) \
	: "0" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2)), \
	  "d" ((long)(arg3)),"g" ((long)(arg4)),"g" ((long)(arg5)) : \
	__syscall_clobber,"r8","r10" ); \
__syscall_return(type,__res); \
}

#define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4, \
	  type5,arg5,type6,arg6) \
type name (type1 arg1,type2 arg2,type3 arg3,type4 arg4,type5 arg5,type6 arg6) \
{ \
long __res; \
__asm__ volatile ("movq %5,%%r10 ; movq %6,%%r8 ; movq %7,%%r9 ; " __syscall \
	: "=a" (__res) \
	: "0" (__NR_##name),"D" ((long)(arg1)),"S" ((long)(arg2)), \
	  "d" ((long)(arg3)), "g" ((long)(arg4)), "g" ((long)(arg5)), \
	  "g" ((long)(arg6)) : \
	__syscall_clobber,"r8","r10","r9" ); \
__syscall_return(type,__res); \
}

#endif

/* asm/unistd.h */
#define __NR_sys_ioctl __NR_ioctl

_syscall3 (int, sys_ioctl, int, fd, int, request, void *, ptr);

// both fglrx and out driver use drm, so always check those. only check
// radeon_ioctl's when running our driver, and only check blob_ioctl's when
// running fglrx.
static struct ioctl_t *
find_ioctl (int ioctl_dir, int ioctl_type, int ioctl_nr, int ioctl_size)
{
  struct ioctl_t *ioctl;

  for (ioctl = drm_ioctls; ioctl->name; ioctl++)
    {
      if (ioctl_dir == ioctl->dir && ioctl_type == ioctl->type
	  && ioctl_nr == ioctl->nr && ioctl_size == ioctl->size)
	{
	  return ioctl;
	}
    }

  if (option_blob)
    {
      for (ioctl = blob_ioctls; ioctl->name; ioctl++)
	{
	  if (ioctl_dir == ioctl->dir && ioctl_type == ioctl->type
	      && ioctl_nr == ioctl->nr && ioctl_size == ioctl->size)
	    {
	      return ioctl;
	    }
	}
    }
  else
    {
      for (ioctl = radeon_ioctls; ioctl->name; ioctl++)
	{
	  if (ioctl_dir == ioctl->dir && ioctl_type == ioctl->type
	      && ioctl_nr == ioctl->nr && ioctl_size == ioctl->size)
	    {
	      return ioctl;
	    }
	}
    }

  return NULL;
}

static char *
pretty_ioctl_dir (int ioctl_dir)
{
  switch (ioctl_dir)
    {
    case _IOC_READ | _IOC_WRITE:
      return "_IOC_READ | _IOC_WRITE";
      break;
    case _IOC_WRITE:
      return "_IOC_WRITE";
      break;
    case _IOC_READ:
      return "_IOC_READ";
      break;
    case _IOC_NONE:
    default:
      return "_IOC_NONE";
      break;
    }
}

// print the ioctl's data in hex. a lot of the code is just to format it nicely
// (hexdump style)
static void
pretty_generic_ioctl (struct ioctl_t *ioctl, int *ioctl_ptr)
{
  int i;

  if (ioctl->size > 0)
    {
      for (i = 0; i < ioctl->size / 4; i++)
	{
	  if (i % 8 == 0 && i > 0)
	    {
	      fprintf (stderr, "\n");
	    }
	  fprintf (stderr, "%08x", ioctl_ptr[i]);
	  if ((i + 1) % 8 != 0 && i + 1 < ioctl->size / 4)
	    {
	      fprintf (stderr, " ");
	    }
	}
      fprintf (stderr, "\n");
    }
}

static void
pretty_ioctl (int ioctl_dir, int ioctl_type, int ioctl_nr,
	      int ioctl_size, int *ioctl_ptr)
{
  struct ioctl_t *ioctl;

  if ((ioctl = find_ioctl (ioctl_dir, ioctl_type, ioctl_nr, ioctl_size)))
    {
      fprintf (stderr, "%s (%s) %d %d %d\n", ioctl->name,
	       pretty_ioctl_dir (ioctl->dir), ioctl->type, ioctl->nr,
	       ioctl->size);

      if (ioctl->func)
	{
	  ioctl->func (ioctl, ioctl_ptr);
	}
      else
	{
	  pretty_generic_ioctl (ioctl, ioctl_ptr);
	}
    }
  else
    {
      // TODO: do something better than just giving up, for example, check if
      // the ioctl is within the device specific range and make a big warning
      // about this, as it should probably be added to the blob's ioctls.
      fprintf (stderr, "warning: unknown ioctl %d!\n", ioctl_nr);
    }
}

int
ioctl (int fd, int request, void *ptr)
{
  int *ioctl_ptr;
  int ioctl_dir, ioctl_type, ioctl_nr, ioctl_size;
  int res;

  /* asm/ioctl.h */
  ioctl_dir = _IOC_DIR (request);
  ioctl_type = _IOC_TYPE (request);
  ioctl_nr = _IOC_NR (request);
  ioctl_size = _IOC_SIZE (request);

  ioctl_ptr = ptr;

  if (option_ioctl)
    {
      pretty_ioctl (ioctl_dir, ioctl_type, ioctl_nr, ioctl_size, ioctl_ptr);
    }

  res = sys_ioctl (fd, request, ptr);

  return res;
}
