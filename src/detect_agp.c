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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "detect_agp.h"

unsigned int agp_addr, agp_len;

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
