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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <revenge_test.h>

void
gl_render_mode_render (void)
{
  test_prologue (NULL);
  glRenderMode (GL_RENDER);
  tri ();
  test_epilogue (false);

  glRenderMode (GL_RENDER);
}

void
gl_render_mode_select (void)
{
  GLuint select_buffer[128];

  memset (select_buffer, 0, sizeof (select_buffer));

  test_prologue (NULL);
  glRenderMode (GL_SELECT);
  glSelectBuffer (128, select_buffer);
  tri ();
  test_epilogue (false);

  glRenderMode (GL_RENDER);
}

void
gl_render_mode_feedback (void)
{
  GLfloat feedback_buffer[128];

  memset (feedback_buffer, 0, sizeof (feedback_buffer));

  test_prologue (NULL);
  glRenderMode (GL_FEEDBACK);
  glFeedbackBuffer (128, GL_2D, feedback_buffer);
  tri ();
  test_epilogue (false);

  glRenderMode (GL_RENDER);
}
