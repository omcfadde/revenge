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
 * \brief Radeon Analysis.
 *
 * \todo Document the packet format in detail; there isn't any existing
 * documentation.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "detect.h"
#include "main.h"
#include "ring.h"

static void analyze_indirect_buffer (int mem_ptr, unsigned long *mem_map);

/**
 * \brief Analyze a register write.
 */
static void
analyze_register (unsigned long key, unsigned long val, int mem_ptr,
		  unsigned long *mem_map)
{
  printf ("0x%04lx <- 0x%08lx\n", key, val);

  switch (key)
    {
    case RADEON_CP_IB_BASE:
      analyze_indirect_buffer (mem_ptr, mem_map);
      break;
    default:
      /* empty */
      break;
    }
}

/**
 * \brief Analyze a type 0 packet.
 *
 * A type 0 packet may write many consecutive registers; the register specified
 * in the packet header is the first register. The count specified in the packet
 * header is the number of consecutive registers to be written.
 */
static void
analyze_packet0 (unsigned long packet_type, unsigned long packet_cnt,
		 unsigned long packet_reg, int mem_ptr,
		 unsigned long *mem_map)
{
  int i;
  unsigned long mapped_reg, mapped_val;

  printf ("packet_type = %ld, packet_cnt = %ld, packet_reg = 0x%08lx\n",
	  packet_type, packet_cnt, packet_reg);

  for (i = 0; i < packet_cnt; i++)
    {
      /* the + 1 is to skip over the packet header */
      mapped_reg = packet_reg + (i << 2);
      mapped_val = mem_map[mem_ptr + i + 1];
      analyze_register (mapped_reg, mapped_val, mem_ptr, mem_map);
    }
}

/**
 * \brief Analyze a type 1 packet.
 *
 * A type 1 packet may write any two consecutive or non consecutive registers;
 * both registers are specified in the packet header. The packet header will be
 * followed by the data for the first and second registers.
 */
static void
analyze_packet1 (unsigned long packet_type, unsigned long packet_cnt,
		 unsigned long packet_rega, unsigned long packet_regb,
		 int mem_ptr, unsigned long *mem_map)
{
  unsigned long mapped_reg, mapped_val;

  printf
    ("packet_type = %ld, packet_cnt = %ld, packet_rega = 0x%08lx, packet_regb = 0x%08lx\n",
     packet_type, packet_cnt, packet_rega, packet_regb);

  /* the + 1 is to skip over the packet header */
  mapped_reg = packet_rega;
  mapped_val = mem_map[mem_ptr + packet_rega + 1];
  analyze_register (mapped_reg, mapped_val, mem_ptr, mem_map);

  /* the + 1 is to skip over the packet header */
  mapped_reg = packet_regb;
  mapped_val = mem_map[mem_ptr + packet_regb + 1];
  analyze_register (mapped_reg, mapped_val, mem_ptr, mem_map);
}

/**
 * \brief Analyze a type 2 packet.
 *
 * A type 2 packet is just a padding packet used for alignment; it doesn't
 * actually write any registers.
 */
static void
analyze_packet2 (unsigned long packet_type, unsigned long packet_cnt,
		 unsigned long packet_reg, int mem_ptr,
		 unsigned long *mem_map)
{
  printf ("packet_type = %ld, packet_cnt = %ld, packet_reg = 0x%08lx\n",
	  packet_type, packet_cnt, packet_reg);

  /* empty */
}

/**
 * \brief Analyze a type 3 packet.
 *
 * \todo Currently type 3 packets are not supported.
 */
static void
analyze_packet3 (unsigned long packet_type, unsigned long packet_cnt,
		 unsigned long packet_reg, int mem_ptr,
		 unsigned long *mem_map)
{
  printf ("packet_type = %ld, packet_cnt = %ld, packet_reg = 0x%08lx\n",
	  packet_type, packet_cnt, packet_reg);

#if 0
  assert (0);
#endif
}

/**
 * \brief Analyze the Radeon packets.
 *
 * \warning It is the responsibility of the packet analysis functions to ensure
 * parsing of all of the packet data; the main loop in this function simply
 * reads the packet header and skips over the data.
 */
static void
analyze_packets (unsigned long head, unsigned long tail,
		 unsigned long *mem_map)
{
  int i;
  unsigned long packet, packet_type, packet_cnt, packet_reg, packet_rega,
    packet_regb;

  /* the packet words and the packet header must be counted... */
  for (i = head; i < tail; i += packet_cnt + 1)
    {
      packet = mem_map[i];

      packet_type = (packet >> 30) & 0x3;
      packet_cnt = ((packet >> 16) & 0x3ff) + 1;
      packet_reg = ((packet >> 0) & 0x1fff) << 2;

      packet_rega = ((packet >> 0) & 0x7ff) << 2;
      packet_regb = ((packet >> 11) & 0x7ff) << 2;

      if (packet)
	{
	  switch (packet_type)
	    {
	    case 0x0:
	      analyze_packet0 (packet_type, packet_cnt, packet_reg, i,
			       mem_map);
	      break;
	    case 0x1:
	      analyze_packet1 (packet_type, packet_cnt, packet_rega,
			       packet_regb, i, mem_map);
	      break;
	    case 0x2:
	      analyze_packet2 (packet_type, packet_cnt, packet_reg, i,
			       mem_map);
	      break;
	    case 0x3:
	      analyze_packet3 (packet_type, packet_cnt, packet_reg, i,
			       mem_map);
	      break;
	    default:
	      assert (0);
	      break;
	    }
	}
    }
}

/**
 * \brief Analyze the Radeon indirect buffer.
 *
 * \todo Dynamically calculate the AGP address.
 *
 * \todo Support PCI-E.
 */
static void
analyze_indirect_buffer (int mem_ptr, unsigned long *mem_map)
{
  unsigned long *ib_mapped_addr;
  unsigned long ib_addr, ib_size;

  ib_addr = mem_map[mem_ptr + 1];
  ib_size = mem_map[mem_ptr + 2];

  ib_mapped_addr =
    (unsigned long *) ((char *) agp_mem_map + (ib_addr - agp_addr));

  if (option_verbose)
    {
      printf
	("indirect buffer! addr = 0x%08lx, mapped_addr = 0x%08lx size = %ld\n",
	 ib_addr, (unsigned long) ib_mapped_addr, ib_size);
    }

  analyze_packets (0, ib_size, ib_mapped_addr);

  if (option_verbose)
    {
      printf ("done!\n");
    }
}

/**
 * \brief Analyze the Radeon ring buffer.
 */
void
analyze_ring (void)
{
  if (option_verbose)
    {
      printf ("ring buffer! ring_head = 0x%08lx, ring_tail = 0x%08lx\n",
	      ring_head, ring_tail);
    }

  analyze_packets (ring_head, ring_tail, ring_mem_map);

  if (option_verbose)
    {
      printf ("done! ring_head = 0x%08lx, ring_tail = 0x%08lx", ring_head,
	      ring_tail);
    }
}
