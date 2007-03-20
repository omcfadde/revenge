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

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "ring.h"

#define REG_ADDR 0xe5000000	// lspci
#define REG_SIZE 0x7d4		// ???

#define AGP_ADDR 0xe0000000	// dmesg | grep 'AGP aperture'
#define AGP_SIZE 64*1024*1024	// arbitary (I think)

int mem_fd;
unsigned long *agp_mem_map, *mem_map, *ring_mem_map;
unsigned long ring_head, ring_size, ring_tail;

void
free_ring (void)
{
  if (munmap (agp_mem_map, AGP_SIZE) < 0)
    {
      assert (0);
    }

  if (munmap (mem_map, REG_SIZE) < 0)
    {
      assert (0);
    }

  close (mem_fd);
}

void
alloc_ring (void)
{
  unsigned long ring_offset;

  if ((mem_fd = open ("/dev/mem", O_RDWR)) < 0)
    {
      assert (0);
    }

  if ((mem_map =
       mmap (NULL, REG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
	     REG_ADDR)) < 0)
    {
      assert (0);
    }

  if ((agp_mem_map =
       mmap (NULL, AGP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
	     AGP_ADDR)) < 0)
    {
      assert (0);
    }

  ring_offset =
    mem_map[RADEON_CP_RB_BASE >> 2] -
    ((mem_map[RADEON_MC_AGP_LOCATION >> 2] & 0xffff) << 16);
  printf ("ring_offset = 0x%lx\n", ring_offset);

  ring_mem_map = (unsigned long *) ((char *) agp_mem_map + ring_offset);
  printf ("ring_mem_map = 0x%lx\n", (unsigned long) ring_mem_map);

  ring_size = (1 << ((mem_map[RADEON_CP_RB_CNTL >> 2] & 0xff) + 1));
  printf ("ring_size = 0x%lx\n", ring_size);
}

void
before_ring (void)
{
  ring_head = ring_tail = mem_map[RADEON_CP_RB_RPTR >> 2];
  printf ("ring_head = ring_tail = 0x%lx\n", ring_head);
}

void
after_ring (void)
{
  ring_tail = mem_map[RADEON_CP_RB_RPTR >> 2];
  printf ("ring_tail = 0x%lx\n", ring_tail);
}
