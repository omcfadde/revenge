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

void
gl_stencil_mask (void)
{
  int i;
  char buf[BUFSIZ];

  for (i = 0; i <= 255; i += 64)
    {
      snprintf (buf, BUFSIZ, "%04d", i);
      test_prologue (buf);
      glEnable (GL_STENCIL_TEST);
      glStencilFunc (GL_ALWAYS, 0, ~0);
      glStencilMask (i);
      glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
      tri ();
      glDisable (GL_STENCIL_TEST);
      test_epilogue (true);
    }

  glStencilMask (~0);
  glDisable (GL_STENCIL_TEST);
}
