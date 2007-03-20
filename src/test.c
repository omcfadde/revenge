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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <GL/gl.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "misc.h"
#include "ring.h"
#include "ring_analyze.h"
#include "test.h"

static void
test_null (void)
{
  /* empty */
}

static void
test_tri (void)
{
  glBegin (GL_TRIANGLES);
  glColor3f (1.0, 0.0, 0.0);
  glVertex3f (1.0, 0.0, 0.0);
  glColor3f (0.0, 1.0, 0.0);
  glVertex3f (0.0, 1.0, 0.0);
  glColor3f (0.0, 0.0, 1.0);
  glVertex3f (0.0, 0.0, 1.0);
  glEnd ();
}

static struct test_t tests[] = {
  {"test_null", test_null},
  {"test_tri", test_tri},
  {NULL, NULL}
};

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

void
test (void)
{
  char buf[BUFSIZ];
  int i;
  struct test_t *test;

  for (test = tests; test->name; test++)
    {
      for (i = 0; i < 3; i++)
	{
	  printf ("%s %d\n", test->name, i);
	  snprintf (buf, BUFSIZ, "%s_%d", test->name, i);
	  open_fd (buf);

	  before ();
	  test->func ();
	  after ();

	  close_fd ();
	  printf ("\n");
	}
    }
}
