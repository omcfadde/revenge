/*
 * $Id$
 * Copyright (C) 2007  Oliver McFadden <z3ro.geek@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <GL/gl.h>
#include <GL/glext.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <revenge_test.h>

typedef struct
{
  unsigned int id;
  char *name;
} data_store;

#define N_(name)	name, # name

void
gl_depth_mask (void)
{
  data_store depth_mask[] = {
    {N_(GL_FALSE)},
    {N_(GL_TRUE)},
  };

  int i;

  for (i = 0; i < sizeof (depth_mask) / sizeof (depth_mask[0]); i++)
    {
      test_prologue (depth_mask[i].name);
      glEnable (GL_DEPTH_TEST);
      glDepthMask (depth_mask[i].id);
      tri ();
      glDisable (GL_DEPTH_TEST);
      test_epilogue (true);
    }

  glDepthMask (GL_TRUE);
  glDisable (GL_DEPTH_TEST);
}
