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

#include "analyze.h"
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
  before_ring ();
}

static void
after (void)
{
  quiescent ();
  after_ring ();
  analyze_ring ();
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
  printf ("glPolygonOffset\n");
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

static struct test_t tests[] = {
  {"test_null", test_null},
  {"test_tri", test_tri},

  /* order is important! */
  {"test_polygon_offset", test_polygon_offset},
  {"test_polygon_offset_point_enable", test_polygon_offset_fill_enable},
  {"test_polygon_offset_point_disable", test_polygon_offset_point_disable},

  /*
     {"test_polygon_offset", test_polygon_offset},
     {"test_polygon_offset_fill_enable", test_polygon_offset_fill_enable},
     {"test_polygon_offset_fill_disable", test_polygon_offset_fill_disable},
   */

  /*
     {"test_polygon_offset", test_polygon_offset},
     {"test_polygon_offset_line_enable", test_polygon_offset_line_enable},
     {"test_polygon_offset_line_disable", test_polygon_offset_line_disable},
   */

  /*
     {"test_polygon_offset", test_polygon_offset},
     {"test_polygon_offset_point_enable", test_polygon_offset_point_enable},
     {"test_polygon_offset_point_disable", test_polygon_offset_point_disable},
   */

  {"test_frag_mov", test_frag_mov},

  {NULL, NULL}
};

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int _old_fd = -1;
static fpos_t _old_fpos;

void
open_fd (const char *name)
{
  char buf[BUFSIZ];

  fflush (stdout);

  fgetpos (stdout, &_old_fpos);
  _old_fd = dup (fileno (stdout));

  snprintf (buf, BUFSIZ, "revenge_%s.txt", name);

  if (!freopen (buf, "w", stdout))
    {
      /* ERR_PRINT ("Opening '%s' failed: %s\n", buf, strerror (errno)); */
    }
}

/* NOTE: This can be called only once after a redirect_output(),
 * they do not really nest.
 */
void
close_fd (void)
{
  fflush (stdout);
  if (_old_fd == -1)
    {
      /* ERR_PRINT("no previous stream to return output to, continuing with the current output.\n"); */
      return;
    }

  /* return the previous output file */
  if (dup2 (_old_fd, fileno (stdout)) == -1)
    {
      /* ERR_PRINT ("Redirecting to previous file failed: %s\n", strerror (errno)); */
    }
  else
    {
      close (_old_fd);
    }

  _old_fd = -1;
  clearerr (stdout);
  fsetpos (stdout, &_old_fpos);
}

void
test (void)
{
  struct test_t *test;

  for (test = tests; test->name; test++)
    {
      open_fd (test->name);

      printf
	("--------------------------------------------------------------------------------\n");
      printf ("%s\n", test->name);
      printf
	("--------------------------------------------------------------------------------\n");
      test->func ();

      close_fd ();
    }
}
