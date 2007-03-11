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

#include "maps.h"

void
analyze_maps (void)
{
  int i, j;

  // print info, and dump the changed regs etc. see renouveau.
  for (i = 0; i < mapnum; i++)
    {
      fprintf (stderr, "map %d:\n", i);
      for (j = 0; j < maps[i].size / 4; j++)
	{
	  if (maps[i].before[j] == maps[i].after[j])
	    {
	      continue;
	    }
	  fprintf (stderr, "changed reg 0x%08x from 0x%08x to 0x%08x\n", j,
		   maps[i].before[j], maps[i].after[j]);
	}

      fprintf (stderr, "\n");
    }
}
