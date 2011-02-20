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
