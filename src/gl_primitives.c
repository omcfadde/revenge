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

#include <GL/gl.h>
#include <GL/glext.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <test.h>

typedef struct
{
  unsigned int id;
  char *name;
} data_store;

#define N_(name)	name, # name

void
gl_primitives (void)
{
  typedef struct
  {
    int num_points;
    data_store prim;
  } prim_t;

  prim_t primitives[] = {
    {1, {N_(GL_POINTS)}},
    {2, {N_(GL_LINES)}},
    {3, {N_(GL_LINE_LOOP)}},
    {3, {N_(GL_LINE_STRIP)}},
    {3, {N_(GL_TRIANGLES)}},
    {4, {N_(GL_TRIANGLE_STRIP)}},
    {4, {N_(GL_TRIANGLE_FAN)}},
    {4, {N_(GL_QUADS)}},
    {6, {N_(GL_QUAD_STRIP)}},
    {5, {N_(GL_POLYGON)}},
  };

  int i, j;

  for (i = 0; i < sizeof (primitives) / sizeof (primitives[0]); i++)
    {
      test_prologue (primitives[i].prim.name);
      glBegin (primitives[i].prim.id);
      for (j = 0; j < primitives[i].num_points; j++)
	{
	  glVertex3i (1 + j * 3, 2 + j * 3, 3 + j * 3);
	}
      glEnd ();
      test_epilogue (true);
    }
}
