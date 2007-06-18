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
#include <unistd.h>

#include "dump.h"
#include "main.h"
#include "misc.h"
#include "ring.h"
#include "test.h"

static void
quiescent (void)
{
  glFlush ();
  sleep (1);
}

static void
before (void)
{
  quiescent ();
  dump_before ();
}

static void
after (void)
{
  quiescent ();
  dump_after ();
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
  before ();
  after ();
}

static void
test_tri (void)
{
  before ();
  tri ();
  after ();
}

static void
test_polygon_offset (void)
{
  before ();
  glPolygonOffset (0.1, 0.2);
  tri ();
  after ();

}

static void
test_polygon_offset_fill_enable (void)
{
  before ();
  glEnable (GL_POLYGON_OFFSET_FILL);
  tri ();
  after ();
}

static void
test_polygon_offset_fill_disable (void)
{
  before ();
  glDisable (GL_POLYGON_OFFSET_FILL);
  tri ();
  after ();
}

static void
test_polygon_offset_line_enable (void)
{
  before ();
  glEnable (GL_POLYGON_OFFSET_LINE);
  tri ();
  after ();
}

static void
test_polygon_offset_line_disable (void)
{
  before ();
  glDisable (GL_POLYGON_OFFSET_LINE);
  tri ();
  after ();
}

static void
test_polygon_offset_point_enable (void)
{
  before ();
  glEnable (GL_POLYGON_OFFSET_POINT);
  tri ();
  after ();
}

static void
test_polygon_offset_point_disable (void)
{
  before ();
  glDisable (GL_POLYGON_OFFSET_POINT);
  tri ();
  after ();
}

static void
test_frag_mov (void)
{
  GLuint arbfp;

  /* *INDENT-OFF* */
  char *arbfptxt =
    "!!ARBfp1.0\n"
    "MOV result.color, fragment.color;\n"
    "END\n";
  /* *INDENT-ON* */

  before ();

  glGenProgramsARB (1, &arbfp);
  glBindProgramARB (GL_FRAGMENT_PROGRAM_ARB, arbfp);
  glProgramStringARB (GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
		      strlen (arbfptxt), (const GLubyte *) arbfptxt);

  glEnable (GL_FRAGMENT_PROGRAM_ARB);

  glBegin (GL_TRIANGLES);
  glColor3f (1.0, 0.0, 0.0);
  glVertex3f (1.0, 0.0, 0.0);
  glColor3f (0.0, 1.0, 0.0);
  glVertex3f (0.0, 1.0, 0.0);
  glColor3f (0.0, 0.0, 1.0);
  glVertex3f (0.0, 0.0, 1.0);
  glEnd ();

  after ();

  glDisable (GL_FRAGMENT_PROGRAM_ARB);
}

static void
test_smooth_line (void)
{
  GLint i;
  int enable = 1;
  float pntA[3] = {
    -160.0, 0.0, 0.0
  };
  float pntB[3] = {
    -130.0, 0.0, 0.0
  };

  glClear (GL_COLOR_BUFFER_BIT);

  before ();

  if (enable)
    {
      glEnable (GL_LINE_SMOOTH);
      glEnable (GL_BLEND);
    }
  else
    {
      glDisable (GL_LINE_SMOOTH);
      glDisable (GL_BLEND);
    }

  for (i = 0; i < 360; i += 5)
    {
      glRotatef (5.0, 0, 0, 1);
      glBegin (GL_LINE_STRIP);
      glVertex3fv (pntA);
      glVertex3fv (pntB);
      glEnd ();
    }

  after ();
}

static struct test_t tests[] = {
  {"test_null", test_null},
  {"test_tri", test_tri},

#if 0
  {"test_smooth_line", test_smooth_line},

  /* order is important! */
  {"test_polygon_offset", test_polygon_offset},

  {"test_polygon_offset_point_enable", test_polygon_offset_fill_enable},
  {"test_polygon_offset_point_disable", test_polygon_offset_point_disable},
  {"test_polygon_offset_fill_enable", test_polygon_offset_fill_enable},
  {"test_polygon_offset_fill_disable", test_polygon_offset_fill_disable},
  {"test_polygon_offset_line_enable", test_polygon_offset_line_enable},
  {"test_polygon_offset_line_disable", test_polygon_offset_line_disable},
  {"test_polygon_offset_point_enable", test_polygon_offset_point_enable},
  {"test_polygon_offset_point_disable", test_polygon_offset_point_disable},

  {"test_frag_mov", test_frag_mov},
#endif

  {NULL, NULL}
};

void
test (void)
{
  struct test_t *test;

  for (test = tests; test->name; test++)
    {
      if (option_per_file)
	{
	  open_fd (test->name);
	}
      printf ("%s: %s\n", __func__, test->name);
      test->func ();
      if (option_per_file)
	{
	  close_fd ();
	}
    }
}
