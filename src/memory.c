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

static unsigned int *
memory_read_agp (unsigned int addr, unsigned int size)
{
  return (unsigned int *) ((char *) agp_mem_map + (addr - agp_addr));
}

static unsigned int *
memory_read_pcie (unsigned int addr, unsigned int size)
{
  unsigned int page_addr = 0;
  unsigned int gart_entry_num = 0;

  gart_entry_num = (addr - pcigart_start) / ATI_PCIGART_PAGE_SIZE;
  page_addr = (pcigart_mem_map[gart_entry_num] & ~0xc) << 8;
  printf ("0x%08x\n", page_addr);

  assert (0);
  return NULL;
}

unsigned int *
memory_read (unsigned int addr, unsigned int size)
{
  if (option_agp)
    {
      return memory_read_agp (addr, size);
    }
  else
    {
      return memory_read_pcie (addr, size);
    }
}
