/*
 * $Id$
 * Copyright (C) 2007  Oliver McFadden <omcfadde@gmail.com>
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
gl_cull_face (void)
{
  data_store cull_face[] = {
    {N_(GL_FRONT)},
    {N_(GL_BACK)},
    {N_(GL_FRONT_AND_BACK)},
  };

  int i;

  for (i = 0; i < sizeof (cull_face) / sizeof (cull_face[0]); i++)
    {
      test_prologue (cull_face[i].name);
      glEnable (GL_CULL_FACE);
      glCullFace (cull_face[i].id);
      tri ();
      glDisable (GL_CULL_FACE);
      test_epilogue (true);
    }

  glCullFace (GL_BACK);
  glDisable (GL_CULL_FACE);
}
