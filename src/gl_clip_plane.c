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
gl_clip_plane (void)
{
  int i, max_planes;
  GLdouble planes[4];
  char buf[BUFSIZ];

  glGetIntegerv (GL_MAX_CLIP_PLANES, &max_planes);

  for (i = 0; i < max_planes; i++)
    {
      planes[0] = (1.0 * (float) i);
      planes[1] = planes[0] + 10.0;
      planes[2] = planes[0] + 100.0;
      planes[3] = planes[0] + 1000.0;

      snprintf (buf, BUFSIZ, "GL_CLIP_PLANE%d", i);
      test_prologue (buf);
      glEnable (GL_CLIP_PLANE0 + i);
      glClipPlane (GL_CLIP_PLANE0 + i, planes);
      tri ();
      glDisable (GL_CLIP_PLANE0 + i);
      test_epilogue (true);
    }
}
