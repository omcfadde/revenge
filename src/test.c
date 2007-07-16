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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "dump.h"
#include "main.h"
#include "test.h"

static inline void
quiescent (void)
{
  struct timespec req = { 1, 0 };

  glFinish ();
  nanosleep (&req, NULL);
}

static void
tri (void)
{
  glBegin (GL_TRIANGLES);
  glVertex3f (1.0, 0.0, 0.0);
  glVertex3f (0.0, 1.0, 0.0);
  glVertex3f (0.0, 0.0, 1.0);
  glEnd ();
}

static void
test_null (void)
{
}

static void
test_tri (void)
{
  tri ();
}

static void
gl_polygon_offset_fill (void)
{
  glPolygonOffset (0.1, 0.2);
  glEnable (GL_POLYGON_OFFSET_FILL);
  tri ();
  glDisable (GL_POLYGON_OFFSET_FILL);
}

static void
gl_polygon_offset_line (void)
{
  glPolygonOffset (0.1, 0.2);
  glEnable (GL_POLYGON_OFFSET_LINE);
  tri ();
  glDisable (GL_POLYGON_OFFSET_LINE);
}

static void
gl_polygon_offset_point (void)
{
  glPolygonOffset (0.1, 0.2);
  glEnable (GL_POLYGON_OFFSET_POINT);
  tri ();
  glDisable (GL_POLYGON_OFFSET_POINT);
}

static void
gl_clip_plane0 (void)
{
  GLdouble plane[] = { 0.1, 0.2, 0.3, 0.4 };

  glClipPlane (GL_CLIP_PLANE0, plane);
  glEnable (GL_CLIP_PLANE0);
  tri ();
  glDisable (GL_CLIP_PLANE0);
}

static struct test_t tests[] = {
  {"test_null", test_null},
  {"test_tri", test_tri},

  {"gl_polygon_offset_fill", gl_polygon_offset_fill},
  {"gl_polygon_offset_line", gl_polygon_offset_line},
  {"gl_polygon_offset_point", gl_polygon_offset_point},

  {"gl_clip_plane0", gl_clip_plane0},

  {NULL, NULL}
};

void
test (void)
{
  char filename[BUFSIZ];
  struct test_t *test;

  for (test = tests; test->name; test++)
    {
      snprintf (filename, BUFSIZ, "revenge-%s.txt", test->name);
      quiescent ();
      dump_rb_pre (filename);
      test->func ();
      quiescent ();
      dump_rb_post ();
    }
}
