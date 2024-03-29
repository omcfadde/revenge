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
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <revenge_test.h>

typedef struct
{
  unsigned int id;
  char *name;
} data_store;

#define N_(name)	name, # name

void
gl_texture_wrap_s (void)
{
  data_store texture_wrap[] = {
    {N_(GL_CLAMP)},
    {N_(GL_REPEAT)},
  };

  GLuint *texture = NULL;;
  GLuint texName;
  int i;

  if (!(texture = random_texture (TEXTURE_WIDTH, TEXTURE_HEIGHT)))
    {
      return;
    }

  for (i = 0; i < sizeof (texture_wrap) / sizeof (texture_wrap[0]); i++)
    {
      test_prologue (texture_wrap[i].name);

      glEnable (GL_TEXTURE_2D);

      glGenTextures (1, &texName);
      glBindTexture (GL_TEXTURE_2D, texName);

      glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		       /* GL_NEAREST_MIPMAP_LINEAR */ GL_NEAREST);
      glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap[i].id);
      glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT,
		    0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

      tex_tri ();

      glDisable (GL_TEXTURE_2D);

      test_epilogue (true);
    }

  free (texture);
}
