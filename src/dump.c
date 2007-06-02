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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "analyze.h"
#include "detect.h"
#include "main.h"
#include "ring.h"

#define DEBUG

static unsigned int ib_addr = 0, ib_size = 0;

static void dump_ib (unsigned int ib_addr, unsigned int ib_size);

static void
dump_reg (unsigned int key, unsigned int val)
{
  assert (key);

#ifdef DEBUG
  printf ("%s: key = 0x%04x val = 0x%08x\n", __func__, key, val);
#endif

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
      //analyze_reg (key, val);
      break;
    }
}

static int
dump_packet0 (unsigned int packet_type, unsigned int packet_cnt,
	      unsigned int packet_bit15, unsigned int packet_reg,
	      unsigned int *mem_map)
{
  int i;
  unsigned int mapped_reg;

#ifdef DEBUG
  printf ("%s: type = %d cnt = %d bit15 = %d reg = 0x%04x\n", __func__,
	  packet_type, packet_cnt, packet_bit15, packet_reg);
#endif

  for (i = 0; i < packet_cnt + 1; i++)
    {
      if (packet_bit15)
	{
	  mapped_reg = packet_reg;
	}
      else
	{
	  mapped_reg = packet_reg + (i << 2);
	}
      dump_reg (mapped_reg, mem_map[i]);
    }

  return packet_cnt + 1;
}

static int
dump_packet2 (unsigned int packet_type, unsigned int packet_cnt,
	      unsigned int packet_bit15, unsigned int packet_reg,
	      unsigned int *mem_map)
{
#ifdef DEBUG
  printf ("%s: type = %d cnt = %d bit15 = %d reg = 0x%04x\n", __func__,
	  packet_type, packet_cnt, packet_bit15, packet_reg);
#endif

  return 0;
}

static int
dump_packet3_noop (unsigned int packet_type, unsigned int packet_cnt,
		   unsigned int packet_opcode, unsigned int *mem_map)
{
  int i;
  unsigned int proc;

  proc = packet_cnt + 1;

  for (i = 0; i < proc; i++)
    {
#ifdef DEBUG
      printf ("%s: 0x%08x\n", __func__, mem_map[i]);
#endif
      assert (mem_map[i] == 0x0);
    }

  return proc;
}

static int
dump_packet3_draw_immediate (unsigned int packet_type,
			     unsigned int packet_cnt,
			     unsigned int packet_opcode,
			     unsigned int *mem_map)
{
  int i;
  unsigned int proc;

  proc = (packet_cnt + 2) * 4;

  for (i = 0; i < proc; i++)
    {
#ifdef DEBUG
      printf ("%s: 0x%08x\n", __func__, mem_map[i]);
#endif
    }

  return proc;
}

static int
dump_packet3 (unsigned int packet_type, unsigned int packet_cnt,
	      unsigned int packet_opcode, unsigned int *mem_map)
{
  int i;
  unsigned int proc;

#ifdef DEBUG
  printf ("%s: type = %d cnt = %d opcode = 0x%02x\n", __func__,
	  packet_type, packet_cnt, packet_opcode);
#endif

  switch (packet_opcode)
    {
    case RADEON_CP_NOP:
      proc =
	dump_packet3_noop (packet_type, packet_cnt, packet_opcode, mem_map);
      break;
    case RADEON_CP_3D_DRAW_IMMD_2:
      proc =
	dump_packet3_draw_immediate (packet_type, packet_cnt, packet_opcode,
				     mem_map);
      break;
    default:
      assert (0);
      break;
    }

  return proc;
}

static void
dump_packet (unsigned int head, unsigned int tail, unsigned int *mem_map)
{
  int i;
  unsigned int packet_type, packet_cnt, packet_bit15, packet_reg,
    packet_opcode;
  unsigned int proc;

  /*
   * (gdb) p/t 0x3 << 30
   * $1 = 11000000000000000000000000000000
   * (gdb) p/t 0x3 << 30 | 0x3fff << 16
   * $2 = 11111111111111110000000000000000
   * (gdb) p/t 0x3 << 30 | 0x3fff << 16 | 1 << 15
   * $3 = 11111111111111111000000000000000
   * (gdb) p/t 0x3 << 30 | 0x3fff << 16 | 1 << 15 | 0x1fff
   * $4 = 11111111111111111001111111111111
   */

  for (i = head; i < tail; i += proc + 1, i &= ring_size - 1)
    {
      packet_type = (mem_map[i] >> 30) & 0x3;
      packet_cnt = (mem_map[i] >> 16) & 0x3fff;
      packet_bit15 = (mem_map[i] >> 15) & 0x1;
      packet_reg = ((mem_map[i] >> 0) & 0x1fff) << 2;

      packet_opcode = (mem_map[i] >> 8) & 0xff;

      assert (mem_map[i]);

      switch (packet_type)
	{
	case 0x0:
	  proc =
	    dump_packet0 (packet_type, packet_cnt, packet_bit15, packet_reg,
			  &mem_map[i + 1]);
	  break;
	case 0x2:
	  proc =
	    dump_packet2 (packet_type, packet_cnt, packet_bit15, packet_reg,
			  &mem_map[i + 1]);
	  break;
	case 0x3:
	  proc =
	    dump_packet3 (packet_type, packet_cnt, packet_opcode,
			  &mem_map[i + 1]);
	  break;
	default:
	  assert (0);
	  break;
	}

      assert (i + proc + 1 <= tail);
    }
}

static void
dump_ib (unsigned int ib_addr, unsigned int ib_size)
{
  unsigned int *mapped_ib_addr;

  mapped_ib_addr =
    (unsigned int *) ((char *) agp_mem_map + (ib_addr - agp_addr));

  dump_packet (0, ib_size, mapped_ib_addr);
}

void
dump (void)
{
  analyze_begin ();

  dump_packet (ring_head, ring_tail, ring_mem_map);

  analyze_end ();
}
