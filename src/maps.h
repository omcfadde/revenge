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

#ifndef __MAPS_H__
#define __MAPS_H__

struct map_t
{
  unsigned int *start;
  unsigned long size;
  unsigned long phys;

  unsigned int *before;
  unsigned int *after;

  char *dev;
  int flags;
};

extern int mapnum;
extern struct map_t maps[BUFSIZ];
void alloc_maps (void);
void free_maps (void);
void before (void);
void after (void);

#endif
