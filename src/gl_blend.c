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

#include <test.h>

typedef struct
{
  unsigned int id;
  char *name;
} data_store;

#define N_(name)	name, # name

void
gl_blend_src (void)
{
  data_store blend_src[] = {
    {N_(GL_ZERO)},
    {N_(GL_ONE)},
    {N_(GL_SRC_COLOR)},
    {N_(GL_ONE_MINUS_SRC_COLOR)},
    {N_(GL_DST_COLOR)},
    {N_(GL_ONE_MINUS_DST_COLOR)},
    {N_(GL_SRC_ALPHA)},
    {N_(GL_ONE_MINUS_SRC_ALPHA)},
    {N_(GL_DST_ALPHA)},
    {N_(GL_ONE_MINUS_DST_ALPHA)},
    {N_(GL_SRC_ALPHA_SATURATE)},
  };

  int i;

  for (i = 0; i < sizeof (blend_src) / sizeof (blend_src[0]); i++)
    {
      test_prologue (blend_src[i].name);
      glEnable (GL_BLEND);
      glBlendFunc (blend_src[i].id, GL_ZERO);
      tri ();
      glDisable (GL_BLEND);
      test_epilogue (true);
    }

  glDisable (GL_BLEND);
  glBlendFunc (GL_ONE, GL_ZERO);
}

void
gl_blend_dst (void)
{
  data_store blend_dst[] = {
    {N_(GL_ZERO)},
    {N_(GL_ONE)},
    {N_(GL_SRC_COLOR)},
    {N_(GL_ONE_MINUS_SRC_COLOR)},
    {N_(GL_DST_COLOR)},
    {N_(GL_ONE_MINUS_DST_COLOR)},
    {N_(GL_SRC_ALPHA)},
    {N_(GL_ONE_MINUS_SRC_ALPHA)},
    {N_(GL_DST_ALPHA)},
    {N_(GL_ONE_MINUS_DST_ALPHA)},
    {N_(GL_SRC_ALPHA_SATURATE)},
  };

  int i;

  for (i = 0; i < sizeof (blend_dst) / sizeof (blend_dst[0]); i++)
    {
      test_prologue (blend_dst[i].name);
      glEnable (GL_BLEND);
      glBlendFunc (GL_ONE, blend_dst[i].id);
      tri ();
      glDisable (GL_BLEND);
      test_epilogue (true);
    }

  glDisable (GL_BLEND);
  glBlendFunc (GL_ONE, GL_ZERO);
}
