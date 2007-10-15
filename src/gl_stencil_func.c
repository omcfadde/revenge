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

#include <revenge_test.h>

typedef struct
{
  unsigned int id;
  char *name;
} data_store;

#define N_(name)	name, # name

void
gl_stencil_func (void)
{
  data_store stencil_func[] = {
    {N_(GL_NEVER)},
    {N_(GL_LESS)},
    {N_(GL_LEQUAL)},
    {N_(GL_GREATER)},
    {N_(GL_GEQUAL)},
    {N_(GL_EQUAL)},
    {N_(GL_NOTEQUAL)},
    {N_(GL_ALWAYS)},
  };

  int i;

  for (i = 0; i < sizeof (stencil_func) / sizeof (stencil_func[0]); i++)
    {
      test_prologue (stencil_func[i].name);
      glEnable (GL_STENCIL_TEST);
      glStencilFunc (stencil_func[i].id, 0, ~0);
      glStencilMask (~0);
      glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
      tri ();
      glDisable (GL_STENCIL_TEST);
      test_epilogue (true);
    }

  glStencilFunc (GL_ALWAYS, 0, ~0);
  glDisable (GL_STENCIL_TEST);
}
