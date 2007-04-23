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
#include "main.h"

static int analyze_indirect_buffer (int mem_ptr, unsigned long *mem_map);

/**
 * \brief Analyze a register write.
 */
static int
analyze_register (unsigned long key, unsigned long val, int mem_ptr,
		  unsigned long *mem_map)
{
  int proc;

  printf ("reg 0x%04lx <- 0x%08lx\n", key, val);

  switch (key)
    {
    case RADEON_CP_IB_BASE:
      proc = analyze_indirect_buffer (mem_ptr, mem_map);
      break;
    default:
      proc = 1;
      break;
    }

  return proc;
}

/**
 * \brief Analyze a type 0 packet.
 *
 * \note A type 0 packet may write many consecutive registers; the register
 * specified in the packet header is the first register. The count specified in
 * the packet header is the number of consecutive registers to be written.
 */
static void
analyze_packet0 (unsigned long packet_cnt, unsigned long packet_reg,
		 int mem_ptr, unsigned long *mem_map)
{
  int i, proc;
  unsigned long mapped_reg, mapped_val;

  for (i = 0; i < packet_cnt; i += proc)
    {
      /* the + 1 is to skip over the packet header */
      mapped_reg = packet_reg + (i * 4);
      mapped_val = mem_map[mem_ptr + i + 1];
      proc = analyze_register (mapped_reg, mapped_val, mem_ptr, mem_map);
    }
}

/**
 * \brief Analyze a type 1 packet.
 *
 * \note A type 1 packet may write any two consecutive or non consecutive
 * registers; both registers are specified in the packet header. The packet
 * header will be followed by the data for the first and second registers.
 *
 * \note I believe type 1 packets are deprecated, however there is some existing
 * code in other projects for handling them...
 *
 * \todo Currently type 1 packets are not supported.
 */
static void
analyze_packet1 (unsigned long packet_cnt, unsigned long packet_reg,
		 int mem_ptr, unsigned long *mem_map)
{
  assert (0);
}

/**
 * \brief Analyze a type 2 packet.
 *
 * \note A type 2 packet is just a padding packet used for alignment; it doesn't
 * actually write any registers.
 */
static void
analyze_packet2 (unsigned long packet_cnt, unsigned long packet_reg,
		 int mem_ptr, unsigned long *mem_map)
{
  /* empty */
}

/**
 * \brief Analyze a type 3 packet.
 *
 * \todo Currently type 3 packets are not supported.
 */
static void
analyze_packet3 (unsigned long packet_cnt, unsigned long packet_reg,
		 int mem_ptr, unsigned long *mem_map)
{
  assert (0);
}

/**
 * \brief Analyze a indirect buffer.
 *
 * \todo This function is incomplete; it doesn't dump the indirect buffer
 * contents.
 *
 * \todo Dynamically calculate the AGP address.
 *
 * \todo Support PCI-E.
 */
static int
analyze_indirect_buffer (int mem_ptr, unsigned long *mem_map)
{
  int i;
  unsigned long *ib_mapped_addr;
  unsigned long ib_addr, ib_size;
  unsigned long packet_type, packet_cnt, packet_reg;

  ib_addr = mem_map[mem_ptr + 1];
  ib_size = mem_map[mem_ptr + 2];

  ib_mapped_addr =
    (unsigned long *) ((char *) agp_mem_map + (ib_addr - AGP_ADDR));

  if (option_verbose)
    {
      printf
	("indirect buffer! addr = 0x%08lx, mapped_addr = 0x%08lx size = %ld\n",
	 ib_addr, (unsigned long) ib_mapped_addr, ib_size);
    }

  /* the packet words and the packet header must be counted... */
  for (i = 0; i < ib_size; i += packet_cnt + 1)
    {
      /* ??? */
      packet_type = (ib_mapped_addr[i] >> 30) & 0x3;
      packet_cnt = (ib_mapped_addr[i] >> 16) & /* 0x3fff */ 0x3ff;
      packet_reg = (ib_mapped_addr[i] >> 0) & /* 0xffff */ 0x1fff;

      /* a count of 0 actually means a count of 1... */
      packet_cnt = packet_cnt + 1;

      /* ??? */
      packet_reg = packet_reg << 2;

      printf ("packet_type = %ld, packet_cnt = %ld, packet_reg = 0x%08lx\n",
	      packet_type, packet_cnt, packet_reg);

      switch (packet_type)
	{
	case 0x0:
	  analyze_packet0 (packet_cnt, packet_reg, i, ib_mapped_addr);
	  break;
	case 0x1:
	  analyze_packet1 (packet_cnt, packet_reg, i, ib_mapped_addr);
	  break;
	case 0x2:
	  analyze_packet2 (packet_cnt, packet_reg, i, ib_mapped_addr);
	  break;
	case 0x3:
	  analyze_packet3 (packet_cnt, packet_reg, i, ib_mapped_addr);
	  break;
	default:
	  assert (0);
	  break;
	}
    }

  if (option_verbose)
    {
      printf ("done!\n");
    }

  return 2;
}

/**
 * \brief Analyze the Radeon ring buffer.
 *
 * \warning It is the responsibility of the packet analysis functions to ensure
 * parsing of all of the packet data; the main loop in this function simply
 * reads the packet header and skips over the data.
 */
void
analyze_ring (void)
{
  int i;
  unsigned long packet_type, packet_cnt, packet_reg;

  if (option_verbose)
    {
      printf ("ring buffer! ring_head = 0x%08lx, ring_tail = 0x%08lx\n",
	      ring_head, ring_tail);
    }

  /* the packet words and the packet header must be counted... */
  for (i = ring_head; i < ring_tail; i += packet_cnt + 1, i &= ring_size - 1)
    {
      /* ??? */
      packet_type = (ring_mem_map[i] >> 30) & 0x3;
      packet_cnt = (ring_mem_map[i] >> 16) & 0x3fff;
      packet_reg = (ring_mem_map[i] >> 0) & 0xffff;

      /* a count of 0 actually means a count of 1... */
      packet_cnt = packet_cnt + 1;

      /* ??? */
      packet_reg = packet_reg << 2;

      printf ("packet_type = %ld, packet_cnt = %ld, packet_reg = 0x%08lx\n",
	      packet_type, packet_cnt, packet_reg);

      switch (packet_type)
	{
	case 0x0:
	  analyze_packet0 (packet_cnt, packet_reg, i, ring_mem_map);
	  break;
	case 0x1:
	  analyze_packet1 (packet_cnt, packet_reg, i, ring_mem_map);
	  break;
	case 0x2:
	  analyze_packet2 (packet_cnt, packet_reg, i, ring_mem_map);
	  break;
	case 0x3:
	  analyze_packet3 (packet_cnt, packet_reg, i, ring_mem_map);
	  break;
	default:
	  assert (0);
	  break;
	}
    }

  if (option_verbose)
    {
      printf ("done! ring_head = 0x%08lx, ring_tail = 0x%08lx, i = 0x%08x\n",
	      ring_head, ring_tail, i);
    }
}