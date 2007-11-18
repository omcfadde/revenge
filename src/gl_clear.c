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

void
gl_clear_color_buffer (void)
{
  test_prologue (NULL);
  glClearColor (0.1, 0.2, 0.3, 0.4);
  glClear (GL_COLOR_BUFFER_BIT);
  tri ();
  test_epilogue (false);
}

void
gl_clear_depth_buffer (void)
{
  test_prologue (NULL);
  glClearDepth (0.1);
  glClear (GL_DEPTH_BUFFER_BIT);
  tri ();
  test_epilogue (false);
}

void
gl_clear_accum_buffer (void)
{
  test_prologue (NULL);
  glClearAccum (0.1, 0.2, 0.3, 0.4);
  glClear (GL_ACCUM_BUFFER_BIT);
  tri ();
  test_epilogue (false);
}

void
gl_clear_stencil_buffer (void)
{
  test_prologue (NULL);
  glClearStencil (64);
  glClear (GL_STENCIL_BUFFER_BIT);
  tri ();
  test_epilogue (false);
}
