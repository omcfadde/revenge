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

#include "detect.h"
#include "main.h"
#include "memory.h"
#include "register.h"

static unsigned int ib_addr = 0, ib_size = 0;
static unsigned int rb_addr = 0, rb_head = 0, rb_size = 0, rb_tail = 0;

static void dump_ib (unsigned int ib_addr, unsigned int ib_size);

static void
dump_reg (unsigned int key, unsigned int val)
{
  printf ("%s: key = 0x%04x val = 0x%08x\n", __func__, key, val);

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
      /* empty */
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
  unsigned int proc;

  printf ("%s: type = %d cnt = %d bit15 = %d reg = 0x%04x\n", __func__,
	  packet_type, packet_cnt, packet_bit15, packet_reg);

  proc = packet_cnt + 1;

  for (i = 0; i < proc; i++)
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

  return proc;
}

static int
dump_packet2 (unsigned int packet_type, unsigned int packet_cnt,
	      unsigned int packet_bit15, unsigned int packet_reg,
	      unsigned int *mem_map)
{
  printf ("%s: type = %d cnt = %d bit15 = %d reg = 0x%04x\n", __func__,
	  packet_type, packet_cnt, packet_bit15, packet_reg);

  return 0;
}

static int
dump_packet3 (unsigned int packet_type, unsigned int packet_cnt,
	      unsigned int packet_opcode, unsigned int *mem_map)
{
  int i;
  unsigned int proc;

  printf ("%s: type = %d cnt = %d opcode = 0x%02x\n", __func__, packet_type,
	  packet_cnt, packet_opcode);

  proc = packet_cnt + 1;

  for (i = 0; i < proc; i++)
    {
      printf ("%s: 0x%08x\n", __func__, mem_map[i]);
    }

  return proc;
}

static void
dump_packets (unsigned int head, unsigned int tail, unsigned int *mem_map)
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

  for (i = head; i < tail; i += proc + 1)
    {
      assert (mem_map[i]);

      packet_type = (mem_map[i] >> 30) & 0x3;
      packet_cnt = (mem_map[i] >> 16) & 0x3fff;
      packet_bit15 = (mem_map[i] >> 15) & 0x1;
      packet_reg = ((mem_map[i] >> 0) & 0x1fff) << 2;

      packet_opcode = (mem_map[i] >> 8) & 0xff;

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

  assert (i == tail);
}

static void
dump_ib (unsigned int ib_addr, unsigned int ib_size)
{
  unsigned int *ib_mem_map;

  if (!option_disable_ib)
    {
      printf ("%s: ib_addr = 0x%08x\n", __func__, ib_addr);
      printf ("%s: ib_size = 0x%08x\n", __func__, ib_size);

      ib_mem_map = memory_read (ib_addr, ib_size * 4);
      dump_packets (0, ib_size, ib_mem_map);
      free (ib_mem_map);
    }
}

void
dump_rb_pre (void)
{
  rb_addr = register_read (RADEON_CP_RB_BASE);
  rb_head = register_read (RADEON_CP_RB_RPTR);
  rb_size = (1 << ((register_read (RADEON_CP_RB_CNTL) & 0xff) + 1));

  printf ("%s: rb_addr = 0x%08x\n", __func__, rb_addr);
  printf ("%s: rb_head = 0x%08x\n", __func__, rb_head);
  printf ("%s: rb_size = 0x%08x\n", __func__, rb_size);
}

void
dump_rb_post (void)
{
  unsigned int *rb_mem_map;

  rb_tail = register_read (RADEON_CP_RB_RPTR);

  printf ("%s: rb_tail = 0x%08x (%d)\n", __func__, rb_tail,
	  rb_tail - rb_head);

  rb_mem_map = memory_read (rb_addr, rb_size * 4);
  dump_packets (rb_head, rb_tail, rb_mem_map);
  free (rb_mem_map);
}
