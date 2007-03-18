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
#include <string.h>

#include "maps.h"

int mapnum = 0;
struct map_t maps[BUFSIZ];

static void
free_map (struct map_t *map)
{
  if (map->dev)
    {
      free (map->dev);
    }

  if (map->before)
    {
      free (map->before);
    }

  if (map->after)
    {
      free (map->after);
    }
}

void
free_maps (void)
{
  int i;

  for (i = 0; i < mapnum; i++)
    {
      free_map (&maps[i]);
    }
}

static int
parse_map (char *str, struct map_t *map)
{
  char dev[256] = { 0 };
  char prot[4];
  int ret;
  unsigned char major, minor;
  unsigned long vm_start, vm_end, phys, inode;

  ret = sscanf (str, "%lx-%lx %4c %lx %2hhx:%2hhx %lu %253s",
		&vm_start, &vm_end, prot, &phys, &major, &minor, &inode, dev);

  if (ret < 7)
    {
      return 1;
    }

  map->start = (unsigned int *) vm_start;

  map->phys = phys;
  map->size = vm_end - vm_start;
  map->before = NULL;
  map->after = NULL;
  map->dev = strdup (dev);

  return 0;
}

static int
good_map (struct map_t *map, int mapnum)
{
  if (strncmp (map->dev, "/dev/dri/card", 13) != 0)
    {
      return 1;
    }

  return 0;
}

void
alloc_maps (void)
{
  FILE *fd;
  char buf[BUFSIZ];

  fd = fopen ("/proc/self/maps", "r");
  while (fgets (buf, BUFSIZ, fd))
    {
      if (parse_map (buf, &maps[mapnum]))
	{
	  continue;
	}

      // if it's too big then it's probably video ram.
      if (maps[mapnum].size > 32 * 1024 * 1024)
	{
	  free_map (&maps[mapnum]);
	  continue;
	}

      // I need to detect the register aperture better; right now it's just
      // based on size. see renouveau for aperture detection code. :)
      if (maps[mapnum].size == 64 * 1024)
	{
	  free_map (&maps[mapnum]);
	  continue;
	}

      if (good_map (&maps[mapnum], mapnum))
	{
	  free_map (&maps[mapnum]);
	  continue;
	}

      if (!(maps[mapnum].before = malloc (maps[mapnum].size)))
	{
	  free_map (&maps[mapnum]);
	  continue;
	}

      if (!(maps[mapnum].after = malloc (maps[mapnum].size)))
	{
	  free_map (&maps[mapnum]);
	  continue;
	}

      // TODO: print the map

      mapnum++;
    }

  fclose (fd);
}

void
before_maps (void)
{
  int i;

  for (i = 0; i < mapnum; i++)
    {
      memcpy (maps[i].before, maps[i].start, maps[i].size);
    }
}

void
after_maps (void)
{
  int i;

  for (i = 0; i < mapnum; i++)
    {
      memcpy (maps[i].after, maps[i].start, maps[i].size);
    }
}
