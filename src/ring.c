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

#include <stdio.h>
#include <stdlib.h>

#include "detect.h"
#include "main.h"
#include "ring.h"

unsigned int ring_head, ring_size, ring_tail;

void
alloc_ring (void)
{
  unsigned int ring_offset;

  ring_offset =
    mem_map[RADEON_CP_RB_BASE >> 2] -
    ((mem_map[RADEON_MC_AGP_LOCATION >> 2] & 0xffff) << 16);

  ring_mem_map = (unsigned int *) ((char *) agp_mem_map + ring_offset);

  ring_size = (1 << ((mem_map[RADEON_CP_RB_CNTL >> 2] & 0xff) + 1));

  ring_head = ring_tail = mem_map[RADEON_CP_RB_RPTR >> 2];

#ifdef DEBUG
  printf
    ("ring_offset = 0x%08x ring_size = 0x%08x ring_head = ring_tail = 0x%08x\n",
     ring_offset, ring_size, ring_head);
#endif
}
