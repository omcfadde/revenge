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

#include <revenge_test.h>

typedef struct
{
  unsigned int id;
  char *name;
} data_store;

#define N_(name)	name, # name

void
gl_alpha_test (void)
{
  data_store alpha_test[] = {
    {N_(GL_NEVER)},
    {N_(GL_LESS)},
    {N_(GL_EQUAL)},
    {N_(GL_LEQUAL)},
    {N_(GL_GREATER)},
    {N_(GL_NOTEQUAL)},
    {N_(GL_GEQUAL)},
    {N_(GL_ALWAYS)},
  };

  char buf[BUFSIZ];
  float j;
  int i;

  for (i = 0; i < sizeof (alpha_test) / sizeof (alpha_test[0]); i++)
    {
      for (j = 0.0; j <= 1.0; j += 0.5)
	{
	  snprintf (buf, BUFSIZ, "%s_%f", alpha_test[i].name, j);
	  test_prologue (buf);
	  glEnable (GL_ALPHA_TEST);
	  glAlphaFunc (alpha_test[i].id, j);
	  tri ();
	  test_epilogue (true);
	}
    }


  glAlphaFunc (GL_ALWAYS, 0.0);
  glDisable (GL_ALPHA_TEST);
}
