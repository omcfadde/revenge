/*
 * $Id$
 * Copyright (C) 2007  Maciej Cencora <m.cencora@gmail.com>
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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include <revenge_detect.h>
#include <revenge_main.h>

#define round_up(x, y) (((x) + (y) - 1) & ~((y) - 1))
#define round_down(x, y) ((x) & ~((y) - 1))

void *
memory_read_agp (unsigned int addr, unsigned int size)
{
  void *tmp;

  if (!(tmp = (void *) malloc (size)))
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  memcpy (tmp, agp_mem_map + ((addr - agp_addr) >> 2), size);

  return tmp;
}

static unsigned int
memory_gart_to_phys (unsigned int addr)
{
  int num;
  unsigned int phys_addr;

  num = (addr - pcigart_start) / ATI_PCIGART_PAGE_SIZE;

  assert ((pcigart_mem_map[num] & 0xc) == 0xc);

  switch (option_interface)
    {
    case INTERFACE_IGP:
    case INTERFACE_RS690:
      phys_addr = pcigart_mem_map[num] & ~0xc;
      break;
    case INTERFACE_PCI:
      phys_addr = pcigart_mem_map[num];
      break;
    case INTERFACE_PCI_E:
      phys_addr = (pcigart_mem_map[num] & ~0xc) << 8;
      break;
    default:
      assert (0);
      break;
    }

  phys_addr = phys_addr & ~(ATI_PCIGART_PAGE_SIZE - 1);

  if (option_debug && option_verbose)
    {
      printf ("%s: addr = 0x%08x phys_addr = 0x%08x (%d)\n", __func__,
	      addr, phys_addr, num);
    }

  return phys_addr;
}

static void *
memory_read_pcigart (unsigned int addr, unsigned int size)
{
  unsigned int addr_mod;
  unsigned int buf_size;
  unsigned int start_page_addr, end_page_addr;
  void *dest;
  void *mem_map, *mem_map_ptr;
  void *page_mem_map;

  addr_mod = addr % ATI_PCIGART_PAGE_SIZE;
  buf_size = round_up (addr_mod + size, ATI_PCIGART_PAGE_SIZE);

  start_page_addr = round_down (addr, ATI_PCIGART_PAGE_SIZE);
  end_page_addr = start_page_addr + buf_size;

  if (!(mem_map = (void *) malloc (buf_size)))
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  if (option_debug && option_verbose)
    {
      printf
	("%s: addr = 0x%08x size = 0x%08x start_page_addr = 0x%08x end_page_addr = 0x%08x\n",
	 __func__, addr, size, start_page_addr, end_page_addr);
    }

  for (mem_map_ptr = mem_map; start_page_addr < end_page_addr;
       start_page_addr += ATI_PCIGART_PAGE_SIZE, mem_map_ptr +=
       ATI_PCIGART_PAGE_SIZE)
    {
      if ((page_mem_map =
	   mmap (NULL, ATI_PCIGART_PAGE_SIZE, PROT_READ | PROT_WRITE,
		 MAP_SHARED, mem_fd,
		 memory_gart_to_phys (start_page_addr))) == MAP_FAILED)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}

      memcpy (mem_map_ptr, page_mem_map, ATI_PCIGART_PAGE_SIZE);

      if (munmap (page_mem_map, ATI_PCIGART_PAGE_SIZE) < 0)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}
    }

  if (!(dest = (void *) malloc (size)))
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }
  memcpy (dest, mem_map + addr_mod, size);
  free (mem_map);

  return dest;
}

void *
memory_read (unsigned int addr, unsigned int size)
{
  void *mem_map;

  switch (option_interface)
    {
    case INTERFACE_AGP:
      mem_map = memory_read_agp (addr, size);
      break;
    case INTERFACE_IGP:
    case INTERFACE_PCI:
    case INTERFACE_PCI_E:
    case INTERFACE_RS690:
      mem_map = memory_read_pcigart (addr, size);
      break;
    default:
      assert (0);
      break;
    }

  return mem_map;
}
