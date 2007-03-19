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

#define RADEON_MC_AGP_LOCATION		0x014c
#define RADEON_CP_RB_BASE		0x0700
#define RADEON_CP_RB_CNTL		0x0704
#define RADEON_CP_RB_RPTR		0x0710
#define RADEON_CP_IB_BASE		0x0738
#define RADEON_CP_PACKET0		0x00000000
#define CP_PACKET0( reg, n )						\
	(RADEON_CP_PACKET0 | ((n) << 16) | ((reg) >> 2))

#define RADEON_CP_PACKET_MASK		0xC0000000
#define RADEON_CP_PACKET_COUNT_MASK	0x3fff0000
#define RADEON_CP_PACKET0_REG_MASK	0x000007ff
#define RADEON_CP_PACKET1_REG0_MASK	0x000007ff
#define RADEON_CP_PACKET1_REG1_MASK	0x003ff800

#define REG_ADDR 0xe5000000	// lspci
#define REG_SIZE 0x7d4		// ???

#define AGP_ADDR 0xe0000000	// dmesg | grep 'AGP aperture'
#define AGP_SIZE 64*1024*1024	// arbitary (I think)

static int mem_fd;
static unsigned long *agp_mem_map, *mem_map, *ring_mem_map;
static unsigned long ring_head, ring_size, ring_tail;

void
free_ring (void)
{
  close (mem_fd);

  if (munmap (mem_map, REG_SIZE) < 0)
    {
      assert (0);
    }

  if (munmap (agp_mem_map, AGP_SIZE) < 0)
    {
      assert (0);
    }
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
  fprintf (stderr, "ring_offset = 0x%lx\n", ring_offset);

  ring_mem_map = (unsigned long *) ((char *) agp_mem_map + ring_offset);
  fprintf (stderr, "ring_mem_map = 0x%lx\n", (unsigned long) ring_mem_map);

  ring_size = (1 << ((mem_map[RADEON_CP_RB_CNTL >> 2] & 0xff) + 1));
  fprintf (stderr, "ring_size = 0x%lx\n", ring_size);
}

void
before_ring (void)
{
  ring_head = ring_tail = mem_map[RADEON_CP_RB_RPTR >> 2];
  fprintf (stderr, "ring_head = ring_tail = 0x%lx\n", ring_head);
}

void
after_ring (void)
{
  int i;
  unsigned long packet_type, packet_cnt, packet_reg;

  ring_tail = mem_map[RADEON_CP_RB_RPTR >> 2];
  fprintf (stderr, "ring_tail = 0x%lx\n", ring_tail);

  for (i = ring_head; i < ring_tail; i++, i &= ring_size - 1)
    {
      // TODO
      packet_type = ring_mem_map[i];
      packet_cnt = ring_mem_map[i];
      packet_reg = ring_mem_map[i];
      switch (packet_type)
	{
	case CP_PACKET0 (RADEON_CP_IB_BASE, 1):
	  fprintf (stderr, "ib!\n");
	  break;
	default:
	  fprintf (stderr, "0x%08lx\n", ring_mem_map[i]);
	  break;
	}
    }
}
