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
#include <ctype.h>
#include <pci/pci.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "register.h"

unsigned int agp_addr = 0, agp_len = 0;
unsigned int pcigart_addr = 0, pcigart_len = 0;
unsigned int pcigart_start = 0, pcigart_end = 0;
unsigned int reg_addr = 0, reg_len = 0;

static int
is_agp_iomem (int level, char *name)
{
  if (strcmp (name, "0000:00:00.0") == 0 || strcmp (name, "GART") == 0)
    {
      return 1;
    }

  return 0;
}

void
detect_agp_aperture (void)
{
  char buf[BUFSIZ];
  FILE *file;

  if (!(file = fopen ("/proc/iomem", "r")))
    {
      assert (0);
    }

  while (fgets (buf, BUFSIZ, file))
    {
      int level = 0;
      char *p = buf;
      while (*p && isspace (*p))
	{
	  level++;
	  p++;
	}

      unsigned int start, end;
      int n, ret;
      ret = sscanf (p, "%x-%x : %n", &start, &end, &n);
      if (ret < 2)
	{
	  continue;
	}

      p += n;
      n = strlen (p);
      if (p[n - 1] == '\n')
	{
	  p[n - 1] = '\0';
	}

      if (is_agp_iomem (level, p))
	{
	  agp_addr = start;
	  agp_len = end - start + 1;
	  printf ("AGP Aperture 0x%08x (0x%08x)\n", agp_addr, agp_len);
	  fclose (file);
	  return;
	}
    }

  fclose (file);

  assert (0);
}

void
detect_pcigart_aperture (void)
{
  pcigart_addr = register_read_pcie (RADEON_PCIE_TX_GART_BASE);
  pcigart_len = /* 1024 * 32 */ 32768;
  printf ("PCI GART Aperture 0x%08x (0x%08x)\n", pcigart_addr, pcigart_len);

  pcigart_start = register_read_pcie (RADEON_PCIE_TX_GART_START_LO);
  pcigart_end = register_read_pcie (RADEON_PCIE_TX_GART_END_LO);
  printf ("PCI GART Start/End 0x%08x 0x%08x\n", pcigart_start, pcigart_end);
}

void
detect_igpgart_aperture (void)
{
  unsigned int agp_base;
  pcigart_addr = register_read_igp (RADEON_IGPGART_BASE_ADDR);
  pcigart_len = /* 1024 * 32 */ 32768;
  printf ("IGP GART Aperture 0x%08x (0x%08x)\n", pcigart_addr, pcigart_len);

  agp_base = register_read (RADEON_MC_AGP_LOCATION);
  pcigart_start = (agp_base & 0xffff) << 16;
  pcigart_end = (agp_base & 0xffff0000);
  printf ("IGP GART Start/End 0x%08x 0x%08x\n", pcigart_start, pcigart_end);
}

static unsigned int
get_conf_long (unsigned char *config, unsigned int pos)
{
  return config[pos] | (config[pos + 1] << 8) | (config[pos + 2] << 16) |
    (config[pos + 3] << 24);
}

void
detect_reg_aperture (void)
{
  char filter_slot[] = "00.0", filter_id[] = "1002:";
  int i;
  pciaddr_t addr, len;
  struct pci_access *pacc;
  struct pci_dev *pdev;
  struct pci_filter filter;
  unsigned char *pci_config;
  unsigned int flag;

  pacc = pci_alloc ();
  pci_init (pacc);
  pci_filter_init (pacc, &filter);
  pci_filter_parse_slot (&filter, filter_slot);
  pci_filter_parse_id (&filter, filter_id);
  pci_scan_bus (pacc);

  for (pdev = pacc->devices; pdev != NULL; pdev = pdev->next)
    {
      if (pci_filter_match (&filter, pdev))
	{
	  pci_config = (unsigned char *) malloc (64);
	  if (pci_read_block (pdev, 0, pci_config, 64))
	    {
	      pci_setup_cache (pdev, pci_config, 64);
	      pci_fill_info (pdev,
			     PCI_FILL_IDENT | PCI_FILL_CLASS | PCI_FILL_IRQ |
			     PCI_FILL_BASES | PCI_FILL_ROM_BASE |
			     PCI_FILL_SIZES);
	      for (i = 0; i < 6; i++)
		{
		  flag =
		    get_conf_long (pci_config, PCI_BASE_ADDRESS_0 + (4 * i));

		  if (!(flag & PCI_BASE_ADDRESS_SPACE_IO))
		    {
		      addr = pdev->base_addr[i] & PCI_ADDR_MEM_MASK;
		      len = pdev->size[i];

		      if (!(flag & PCI_BASE_ADDRESS_MEM_PREFETCH)
			  && len == 65536)
			{
			  reg_addr = (unsigned int) addr;
			  reg_len = (unsigned int) len;
			  printf ("Register Aperture 0x%08x (0x%08x)\n",
				  reg_addr, reg_len);
			}
		    }
		}
	    }
	  free (pci_config);
	}
    }

  pci_cleanup (pacc);
}
