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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "analyze.h"
#include "detect.h"
#include "main.h"
#include "ring.h"

static unsigned int ib_addr = 0, ib_size = 0;

static void dump_ib (unsigned int ib_addr, unsigned int ib_size);

static void
dump_reg (unsigned int key, unsigned int val)
{
  switch (key)
    {
    case RADEON_CP_IB_BASE:
      ib_addr = val;
      break;
    case RADEON_CP_IB_BUFSZ:
      ib_size = val;
      dump_ib (ib_addr, ib_size);
      ib_addr = ib_size = 0;
      break;
    default:
      analyze_reg (key, val);
      break;
    }
}

static int
dump_packet0 (unsigned int packet_cnt, unsigned int packet_reg,
	      unsigned int *mem_map)
{
  int i;
  unsigned int mapped_reg, mapped_val;

  for (i = 0; i <= packet_cnt; i++)
    {
      mapped_reg = packet_reg + (i << 2);
      mapped_val = mem_map[i + 1];
      dump_reg (mapped_reg, mapped_val);
    }

  return (packet_cnt + 1) + 1;
}

static int
dump_packet2 (unsigned int packet_cnt)
{
  return packet_cnt + 1;
}

static int
dump_packet3 (unsigned int packet_cnt)
{
  return -1;
}

static void
dump_packets (unsigned int head, unsigned int tail, unsigned int *mem_map)
{
  int i;
  unsigned int packet_type, packet_cnt, packet_reg;
  unsigned int proc;

  for (i = head; i < tail; i += proc, i &= ring_size - 1)
    {
      packet_type = (mem_map[i] >> 30) & 0x3;
      packet_cnt = (mem_map[i] >> 16) & 0x3fff;
      packet_reg = ((mem_map[i] >> 0) & 0x1fff) << 2;

      switch (packet_type)
	{
	case 0x0:
	  proc = dump_packet0 (packet_cnt, packet_reg, &mem_map[i]);
	  break;
	case 0x2:
	  proc = dump_packet2 (packet_cnt);
	  break;
	case 0x3:
	  proc = dump_packet3 (packet_cnt);
	  break;
	default:
	  assert (0);
	  break;
	}

      if (proc == -1)
	{
	  /* HACK */
	  return;
	}
    }

  assert (i == tail);
}

static void
dump_ib (unsigned int ib_addr, unsigned int ib_size)
{
  unsigned int *ib_mapped_addr;

  ib_mapped_addr =
    (unsigned int *) ((char *) agp_mem_map + (ib_addr - agp_addr));

  dump_packets (0, ib_size, ib_mapped_addr);
}

void
dump (void)
{
  analyze_begin ();

  dump_packets (ring_head, ring_tail, ring_mem_map);

  analyze_end ();
}
