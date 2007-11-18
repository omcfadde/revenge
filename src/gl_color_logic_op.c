/*
 * $Id$
 * Copyright (C) 2007  Oliver McFadden <z3ro.geek@gmail.com>
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
gl_color_logic_op (void)
{
  data_store color_logic_op[] = {
    {N_(GL_CLEAR)},
    {N_(GL_SET)},
    {N_(GL_COPY)},
    {N_(GL_COPY_INVERTED)},
    {N_(GL_NOOP)},
    {N_(GL_INVERT)},
    {N_(GL_AND)},
    {N_(GL_NAND)},
    {N_(GL_OR)},
    {N_(GL_NOR)},
    {N_(GL_XOR)},
    {N_(GL_EQUIV)},
    {N_(GL_AND_REVERSE)},
    {N_(GL_AND_INVERTED)},
    {N_(GL_OR_REVERSE)},
    {N_(GL_OR_INVERTED)},
  };

  int i;

  for (i = 0; i < sizeof (color_logic_op) / sizeof (color_logic_op[0]); i++)
    {
      test_prologue (color_logic_op[i].name);
      glEnable (GL_COLOR_LOGIC_OP);
      glLogicOp (color_logic_op[i].id);
      tri ();
      glDisable (GL_COLOR_LOGIC_OP);
      test_epilogue (true);
    }

  glLogicOp (GL_COPY);
  glDisable (GL_COLOR_LOGIC_OP);
}
