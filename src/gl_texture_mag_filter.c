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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <test.h>

typedef struct
{
  unsigned int id;
  char *name;
} data_store;

#define N_(name)	name, # name

#define TEXTURE_WIDTH 16
#define TEXTURE_HEIGHT 16

static GLuint *
random_texture (int width, int height)
{
  GLuint *texture = NULL;
  int i;

  if (!(texture = (GLuint *) malloc (width * height * sizeof (GLuint))))
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      return NULL;
    }

  for (i = 0; i < width * height; i++)
    {
      texture[i] = random ();
    }

  return texture;
}

void
gl_texture_mag_filter (void)
{
  data_store texture_mag_filter[] = {
    {N_(GL_NEAREST)},
    {N_(GL_LINEAR)},
  };

  GLuint *texture = NULL;;
  GLuint texName;
  int i;

  if (!(texture = random_texture (TEXTURE_WIDTH, TEXTURE_HEIGHT)))
    {
      return;
    }

  for (i = 0;
       i < sizeof (texture_mag_filter) / sizeof (texture_mag_filter[0]); i++)
    {
      test_prologue (texture_mag_filter[i].name);

      glEnable (GL_TEXTURE_2D);

      glGenTextures (1, &texName);
      glBindTexture (GL_TEXTURE_2D, texName);

      glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		       /* GL_NEAREST_MIPMAP_LINEAR */ GL_NEAREST);
      glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		       texture_mag_filter[i].id);
      glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT,
		    0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

      tex_tri ();

      glDisable (GL_TEXTURE_2D);

      test_epilogue (true);
    }

  free (texture);
}
