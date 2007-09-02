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
#include <errno.h>
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

static void
test_quiescent (void)
{
  struct timespec req = { 1, 0 };

  glFinish ();
  nanosleep (&req, NULL);
}

static void
test_prologue (void)
{
  test_quiescent ();
  dump_rb_pre ();
}

static void
test_epilogue (void)
{
  test_quiescent ();
  dump_rb_post ();
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
  test_prologue ();
  test_epilogue ();
}

static void
gl_triangle (void)
{
  test_prologue ();
  tri ();
  test_epilogue ();
}

static void
gl_polygon_offset_fill (void)
{
  test_prologue ();
  glPolygonOffset (0.1, 0.2);
  glEnable (GL_POLYGON_OFFSET_FILL);
  tri ();
  glDisable (GL_POLYGON_OFFSET_FILL);
  test_epilogue ();
}

static void
gl_polygon_offset_line (void)
{
  test_prologue ();
  glPolygonOffset (0.1, 0.2);
  glEnable (GL_POLYGON_OFFSET_LINE);
  tri ();
  glDisable (GL_POLYGON_OFFSET_LINE);
  test_epilogue ();
}

static void
gl_polygon_offset_point (void)
{
  test_prologue ();
  glPolygonOffset (0.1, 0.2);
  glEnable (GL_POLYGON_OFFSET_POINT);
  tri ();
  glDisable (GL_POLYGON_OFFSET_POINT);
  test_epilogue ();
}

static void
gl_clip_plane0 (void)
{
  GLdouble plane[] = { 0.1, 0.2, 0.3, 0.4 };

  test_prologue ();
  glClipPlane (GL_CLIP_PLANE0, plane);
  glEnable (GL_CLIP_PLANE0);
  tri ();
  glDisable (GL_CLIP_PLANE0);
  test_epilogue ();
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

  test_prologue ();
  glGenTextures (1, &tid);
  glActiveTexture (GL_TEXTURE0_ARB);
  glEnable (GL_TEXTURE_2D);

  glBindTexture (GL_TEXTURE_2D, tid);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  tex_tri ();

  glActiveTexture (GL_TEXTURE0_ARB);
  glDisable (GL_TEXTURE_2D);

  test_epilogue ();

  free (texture);
}

#define _(x) #x, x

static struct test_t tests[] = {
  {_(gl_null)},
  {_(gl_triangle)},
  {_(gl_polygon_offset_fill)},
  {_(gl_polygon_offset_line)},
  {_(gl_polygon_offset_point)},
  {_(gl_clip_plane0)},
  {_(gl_texture)},
};

#undef _

static int num_tests = sizeof (tests) / sizeof (tests[0]);

void
test (void)
{
  int i;
  struct test_t *test;

  dump_device_id ();
  dump_device_name ();
  dump_fglrx_version ();
  dump_fglrxinfo ();
  dump_lspci ();

  for (i = 0; i < num_tests; i++)
    {
      test = &tests[i];

      if (mkdir (test->name, 0777) < 0)
	{
	  if (errno != EEXIST)
	    {
	      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		       strerror (errno));
	      exit (EXIT_FAILURE);
	    }
	}

      if (chdir (test->name) < 0)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}

      printf ("%d/%d %s\n", i + 1, num_tests, test->name);

      test->func ();

      if (chdir ("..") < 0)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}
    }
}
