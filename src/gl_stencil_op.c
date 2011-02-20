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

data_store stencil_op[] = {
  {N_(GL_KEEP)},
  {N_(GL_ZERO)},
  {N_(GL_REPLACE)},
  {N_(GL_INCR)},
  {N_(GL_DECR)},
  {N_(GL_INVERT)},
};

void
gl_stencil_op_fail (void)
{
  int i;

  for (i = 0; i < sizeof (stencil_op) / sizeof (stencil_op[0]); i++)
    {
      test_prologue (stencil_op[i].name);
      glEnable (GL_STENCIL_TEST);
      glStencilFunc (GL_ALWAYS, 0, ~0);
      glStencilMask (~0);
      glStencilOp (stencil_op[i].id, GL_KEEP, GL_KEEP);
      tri ();
      glDisable (GL_STENCIL_TEST);
      test_epilogue (true);
    }

  glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
  glDisable (GL_STENCIL_TEST);
}

void
gl_stencil_op_zfail (void)
{
  int i;

  for (i = 0; i < sizeof (stencil_op) / sizeof (stencil_op[0]); i++)
    {
      test_prologue (stencil_op[i].name);
      glEnable (GL_STENCIL_TEST);
      glStencilFunc (GL_ALWAYS, 0, ~0);
      glStencilMask (~0);
      glStencilOp (GL_KEEP, stencil_op[i].id, GL_KEEP);
      tri ();
      glDisable (GL_STENCIL_TEST);
      test_epilogue (true);
    }

  glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
  glDisable (GL_STENCIL_TEST);
}

void
gl_stencil_op_zpass (void)
{
  int i;

  for (i = 0; i < sizeof (stencil_op) / sizeof (stencil_op[0]); i++)
    {
      test_prologue (stencil_op[i].name);
      glEnable (GL_STENCIL_TEST);
      glStencilFunc (GL_ALWAYS, 0, ~0);
      glStencilMask (~0);
      glStencilOp (GL_KEEP, GL_KEEP, stencil_op[i].id);
      tri ();
      glDisable (GL_STENCIL_TEST);
      test_epilogue (true);
    }

  glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
  glDisable (GL_STENCIL_TEST);
}
