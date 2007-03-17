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

static int
analyze_map_changed (struct map_t *map)
{
  int i, j;

  for (i = 0, j = 0; i < map->size / 4; i++)
    {
      if (map->before[i] != map->after[i])
	{
	  j++;
	}
    }

  return j;
}

static void
analyze_map (struct map_t *map)
{
  int i;

  for (i = 0; i < map->size / 4; i++)
    {
      if (map->before[i] == map->after[i])
	{
	  continue;
	}
      fprintf (stderr, "0x%08x: 0x%08x -> 0x%08x\n", i,
	       map->before[i], map->after[i]);
    }
}

void
analyze_maps (void)
{
  int i;
  int map_changed;
  struct map_t *map;

  for (i = 0; i < mapnum; i++)
    {
      map = &maps[i];

      map_changed = analyze_map_changed (map);

      fprintf (stderr, "Map %d/%d has %d/%ld changed dwords.\n", i, mapnum,
	       map_changed, map->size / 4);

      if (map_changed)
	{
	  analyze_map (map);
	}
    }
}
