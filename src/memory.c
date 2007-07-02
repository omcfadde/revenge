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
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "detect.h"
#include "main.h"

static unsigned int *
memory_read_agp (unsigned int addr, unsigned int size)
{
  unsigned int *tmp;

  tmp = (unsigned int *) malloc (size);
  memcpy (tmp, (unsigned int *) ((char *) agp_mem_map + (addr - agp_addr)),
	  size);

  return tmp;
}

static unsigned int
gart_to_phys (unsigned int addr)
{
  int num;
  unsigned int phys_addr;

  num = (addr - pcigart_start) / ATI_PCIGART_PAGE_SIZE;

  switch (option_interface)
    {
    case IF_IGP:
      phys_addr = (pcigart_mem_map[num] & ~0xc);
      break;
    default:
      phys_addr = (pcigart_mem_map[num] & ~0xc) << 8;
      break;
    }

  return phys_addr;
}

static unsigned int *
memory_read_pcigart (unsigned int addr, unsigned int size)
{
  int i;
  unsigned int *page_mem_map;
  unsigned int *tmp;
  unsigned int page_addr, page_phys_addr;

  size = (size + (ATI_PCIGART_PAGE_SIZE - 1)) & ~(ATI_PCIGART_PAGE_SIZE - 1);

  assert ((addr % ATI_PCIGART_PAGE_SIZE) == 0);
  assert ((size % ATI_PCIGART_PAGE_SIZE) == 0);

  tmp = (unsigned int *) malloc (size);

  for (i = 0; i < size; i += ATI_PCIGART_PAGE_SIZE)
    {
      page_addr = addr + i;
      page_phys_addr = gart_to_phys (page_addr);

      if ((page_mem_map =
	   mmap (NULL, ATI_PCIGART_PAGE_SIZE, PROT_READ | PROT_WRITE,
		 MAP_SHARED, mem_fd, page_phys_addr)) == MAP_FAILED)
	{
	  assert (0);
	}

      memcpy ((unsigned int *) ((char *) tmp + i), page_mem_map,
	      ATI_PCIGART_PAGE_SIZE);

      if (munmap (page_mem_map, ATI_PCIGART_PAGE_SIZE) < 0)
	{
	  assert (0);
	}
    }

  return tmp;
}

unsigned int *
memory_read (unsigned int addr, unsigned int size)
{
  unsigned int *mem_map;

  switch (option_interface)
    {
    case IF_AGP:
      mem_map = memory_read_agp (addr, size);
      break;
    default:
      mem_map = memory_read_pcigart (addr, size);
      break;
    }

  return mem_map;
}
