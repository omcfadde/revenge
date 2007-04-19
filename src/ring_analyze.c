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

/**
 * \file
 *
 * \brief Analyze the Radeon ring buffer.
 *
 * \todo Document the packet format in detail; there isn't any existing
 * documentation
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ring.h"

/*
 * Radeon Packets
 *
 * 11000000000000000000000000000000 Type
 * 00111111111111110000000000000000 Count
 *
 * ???
 */

#define REVENGE_CP_PACKET_TYPE_SHIFT 30
#define REVENGE_CP_PACKET_TYPE_MASK 0x3

#define REVENGE_CP_PACKET_TYPE0 0x0
#define REVENGE_CP_PACKET_TYPE1 0x1
#define REVENGE_CP_PACKET_TYPE2 0x2
#define REVENGE_CP_PACKET_TYPE3 0x3

#define REVENGE_CP_PACKET_CNT_SHIFT 16
#define REVENGE_CP_PACKET_CNT_MASK 0x3fff

/* not sure about these... */
#define REVENGE_CP_PACKET_REG_SHIFT 0
#define REVENGE_CP_PACKET_REG_MASK 0xffff

/**
 * \brief Analyze a indirect buffer.
 *
 * \todo This function is incomplete; it doesn't dump the indirect buffer
 * contents.
 */
static int
analyze_ring_packet0_ib (int ring_ptr, unsigned long packet_type,
			 unsigned long packet_cnt, unsigned long packet_reg)
{
  unsigned long ib_addr, ib_size;

  ib_addr = ring_mem_map[ring_ptr + 1];
  ib_size = ring_mem_map[ring_ptr + 2];

  printf ("indirect buffer! addr = 0x%08lx, size = %ld\n", ib_addr, ib_size);

  /* TODO: dump the indirect buffer. */

  return 2;
}

/**
 * \brief Analyze a type 0 packet.
 */
static void
analyze_ring_packet0 (int ring_ptr, unsigned long packet_type,
		      unsigned long packet_cnt, unsigned long packet_reg)
{
  int i, proc;
  unsigned long reg;

  for (i = 0; i < packet_cnt; i += proc)
    {
      reg = packet_reg + (i * 4);

      /* the + 1 is to skip over the packet header */
      printf ("reg 0x%04lx <- 0x%08lx\n", reg,
	      ring_mem_map[ring_ptr + i + 1]);

      switch (reg)
	{
	case RADEON_CP_IB_BASE:
	  proc =
	    analyze_ring_packet0_ib (ring_ptr, packet_type, packet_cnt, reg);
	  break;
	default:
	  proc = 1;
	  break;
	}
    }
}

/**
 * \brief Analyze a type 1 packet.
 */
static void
analyze_ring_packet1 (int ring_ptr, unsigned long packet_type,
		      unsigned long packet_cnt, unsigned long packet_reg)
{
}

/**
 * \brief Analyze a type 2 packet.
 */
static void
analyze_ring_packet2 (int ring_ptr, unsigned long packet_type,
		      unsigned long packet_cnt, unsigned long packet_reg)
{
}

/**
 * \brief Analyze a type 3 packet.
 */
static void
analyze_ring_packet3 (int ring_ptr, unsigned long packet_type,
		      unsigned long packet_cnt, unsigned long packet_reg)
{
}

/**
 * \brief Analyze the Radeon ring buffer.
 */
void
analyze_ring (void)
{
  int i;
  unsigned long packet_type, packet_cnt, packet_reg;

  printf ("ring buffer! ring_head = %08lx, ring_tail = %08lx\n", ring_head,
	  ring_tail);

  /* the packet words and the packet header must be counted... */
  for (i = ring_head; i < ring_tail; i += packet_cnt + 1, i &= ring_size - 1)
    {
      /* ??? */
      packet_type =
	(ring_mem_map[i] >> REVENGE_CP_PACKET_TYPE_SHIFT) &
	REVENGE_CP_PACKET_TYPE_MASK;
      packet_cnt =
	(ring_mem_map[i] >> REVENGE_CP_PACKET_CNT_SHIFT) &
	REVENGE_CP_PACKET_CNT_MASK;
      packet_reg =
	(ring_mem_map[i] >> REVENGE_CP_PACKET_REG_SHIFT) &
	REVENGE_CP_PACKET_REG_MASK;

      /* a count of 0 actually means a count of 1... */
      packet_cnt = packet_cnt + 1;

      /* ??? */
      packet_reg = packet_reg << 2;

      printf ("packet_type = %ld, packet_cnt = %ld, packet_reg = 0x%08lx\n",
	      packet_type, packet_cnt, packet_reg);

      switch (packet_type)
	{
	case REVENGE_CP_PACKET_TYPE0:
	  analyze_ring_packet0 (i, packet_type, packet_cnt, packet_reg);
	  break;
	case REVENGE_CP_PACKET_TYPE1:
	  analyze_ring_packet1 (i, packet_type, packet_cnt, packet_reg);
	  break;
	case REVENGE_CP_PACKET_TYPE2:
	  analyze_ring_packet2 (i, packet_type, packet_cnt, packet_reg);
	  break;
	case REVENGE_CP_PACKET_TYPE3:
	  analyze_ring_packet3 (i, packet_type, packet_cnt, packet_reg);
	  break;
	default:
	  assert (0);
	  break;
	}

      printf ("\n");
    }

  printf ("done! ring_head = %08lx, ring_tail = %08lx, i = %08x\n", ring_head,
	  ring_tail, i);
}
