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

static void
analyze_ring_packet0 (unsigned long *packet_type, unsigned long *packet_cnt,
		      unsigned long *packet_reg)
{
  switch (*packet_reg)
    {
    case RADEON_CP_IB_BASE:
      printf ("ib!\n");
      break;
    default:
      /* empty */
      break;
    }
}

static void
analyze_ring_packet1 (unsigned long *packet_type, unsigned long *packet_cnt,
		      unsigned long *packet_reg)
{
}

static void
analyze_ring_packet2 (unsigned long *packet_type, unsigned long *packet_cnt,
		      unsigned long *packet_reg)
{
}

static void
analyze_ring_packet3 (unsigned long *packet_type, unsigned long *packet_cnt,
		      unsigned long *packet_reg)
{
}

void
analyze_ring (void)
{
  int i;
  unsigned long packet_type, packet_cnt, packet_reg;

  for (i = ring_head; i < ring_tail; i++, i &= ring_size - 1)
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

      /* ??? */
      packet_reg = packet_reg << 2;

      printf
	("packet_type = 0x%08lx packet_cnt = 0x%08lx packet_reg = 0x%08lx\n",
	 packet_type, packet_cnt, packet_reg);

      switch (packet_type)
	{
	case REVENGE_CP_PACKET_TYPE0:
	  analyze_ring_packet0 (&packet_type, &packet_cnt, &packet_reg);
	  break;
	case REVENGE_CP_PACKET_TYPE1:
	  analyze_ring_packet1 (&packet_type, &packet_cnt, &packet_reg);
	  break;
	case REVENGE_CP_PACKET_TYPE2:
	  analyze_ring_packet2 (&packet_type, &packet_cnt, &packet_reg);
	  break;
	case REVENGE_CP_PACKET_TYPE3:
	  analyze_ring_packet3 (&packet_type, &packet_cnt, &packet_reg);
	  break;
	default:
	  assert (0);
	  break;
	}
    }
}
