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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <revenge_detect.h>
#include <revenge_main.h>

void
dump_device_id (void)
{
  FILE *file;

  if (!(file = fopen ("device_id.txt", "w")))
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  fprintf (file, "%04x\n", reg_device_id);

  fclose (file);
}

void
dump_device_name (void)
{
  FILE *file;

  if (!(file = fopen ("device_name.txt", "w")))
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  fprintf (file, "%s\n", reg_device_name);

  fclose (file);
}

void
dump_fglrx_version (void)
{
  system
    ("fglrxinfo 2> /dev/null | sed -n 's/.*(\\([^)]*\\)).*/\\1/p' > fglrx_version.txt");
}

void
dump_fglrxinfo (void)
{
  system ("fglrxinfo -v 2> /dev/null > fglrxinfo.txt");
}

void
dump_glxinfo (void)
{
  system ("glxinfo -v 2> /dev/null > glxinfo.txt");
}

void
dump_interface (void)
{
  FILE *file;

  if (!(file = fopen ("interface.txt", "w")))
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  switch (option_interface)
    {
    case IF_PCIE:
      fprintf (file, "PCI-E\n");
      break;
    case IF_AGP:
      fprintf (file, "AGP\n");
      break;
    case IF_IGP:
      fprintf (file, "IGP\n");
      break;
    default:
      assert (0);
      break;
    }

  fclose (file);
}

void
dump_lspci (void)
{
  system ("lspci -v 2> /dev/null > lspci.txt");
}
