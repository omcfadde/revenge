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

#include <stdio.h>
#include <stdlib.h>

#include "ring.h"

static void
analyze_ring_ib (void)
{
  fprintf (stderr, "ib!\n");
}

void
analyze_ring (void)
{
  int i;
  unsigned long packet_type, packet_cnt, packet_reg;

  for (i = ring_head; i < ring_tail; i++, i &= ring_size - 1)
    {
      switch (ring_mem_map[i])
	{
	case CP_PACKET0 (RADEON_CP_IB_BASE, 1):
	  analyze_ring_ib ();
	  break;
	default:
	  fprintf (stderr, "0x%08lx\n", ring_mem_map[i]);
	  break;
	}
    }
}

