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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <dump.h>
#include <main.h>
#include <test.h>

#define TEXTURE_WIDTH 16
#define TEXTURE_HEIGHT 16

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

void
tex_tri (void)
{
  glBegin (GL_TRIANGLES);
  glMultiTexCoord2f (GL_TEXTURE0, 1.0, 0.0);
  glVertex3f (1.0, 0.0, 0.0);
  glMultiTexCoord2f (GL_TEXTURE0, 0.0, 1.0);
  glVertex3f (0.0, 1.0, 0.0);
  glMultiTexCoord2f (GL_TEXTURE0, 0.0, 0.0);
  glVertex3f (0.0, 0.0, 1.0);
  glEnd ();
}

static void
gl_null (void)
{
}

static void
gl_triangle (void)
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

static void *
create_random_texture (int w, int h)
{
  uint32_t *texture;
  int i;

  texture = malloc (w * h * sizeof (uint32_t));
  if (!texture)
    {
      printf ("can not allocate mem for texture\n");
      return NULL;
    }

  for (i = 0; i < w * h; i++)
    {
      texture[i] = random ();
    }

  return texture;
}

static void
gl_texture (void)
{
  GLuint tid;
  int *texture = NULL;

  if (!(texture = create_random_texture (TEXTURE_WIDTH, TEXTURE_HEIGHT)))
    {
      return;
    }

  glGenTextures (1, &tid);
  glActiveTexture (GL_TEXTURE0_ARB);
  glEnable (GL_TEXTURE_2D);

  glBindTexture (GL_TEXTURE_2D, tid);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH,
		TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  tex_tri ();

  glActiveTexture (GL_TEXTURE0_ARB);
  glDisable (GL_TEXTURE_2D);

  free (texture);
}

static struct test_t tests[] = {
  {"gl_null", gl_null},
  {"gl_triangle", gl_triangle},

  {"gl_polygon_offset_fill", gl_polygon_offset_fill},
  {"gl_polygon_offset_line", gl_polygon_offset_line},
  {"gl_polygon_offset_point", gl_polygon_offset_point},

  {"gl_clip_plane0", gl_clip_plane0},

  {"gl_texture", gl_texture},

  {NULL, NULL}
};

void
test (void)
{
  struct test_t *test;

  dump_device_id ();
  dump_device_name ();
  dump_lspci ();

  for (test = tests; test->name; test++)
    {
      printf ("%s: %s\n", __func__, test->name);

      mkdir (test->name, 0777);
      chdir (test->name);

      quiescent ();
      dump_rb_pre ();
      test->func ();
      quiescent ();
      dump_rb_post ();

      chdir ("..");
    }
}
