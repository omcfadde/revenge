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

void
gl_depth_range (void)
{
  char buf[BUFSIZ];

  snprintf (buf, BUFSIZ, "%f_%f", 0.1, 0.5);
  test_prologue (buf);
  glEnable (GL_DEPTH_TEST);
  glDepthRange (0.1, 0.5);
  tri ();
  glDisable (GL_DEPTH_TEST);
  test_epilogue (true);

  snprintf (buf, BUFSIZ, "%f_%f", 0.0, 1.0);
  test_prologue (buf);
  glEnable (GL_DEPTH_TEST);
  glDepthRange (0.0, 1.0);
  tri ();
  glDisable (GL_DEPTH_TEST);
  test_epilogue (true);
}
