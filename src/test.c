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

#include <gl_alpha_test.h>
#include <gl_blend.h>
#include <gl_clear.h>
#include <gl_clip_plane.h>
#include <gl_color_logic_op.h>
#include <gl_cull_face.h>
#include <gl_depth_func.h>
#include <gl_depth_mask.h>
#include <gl_depth_range.h>
#include <gl_front_face.h>
#include <gl_line_smooth.h>
#include <gl_line_smooth_hint.h>
#include <gl_line_width.h>
#include <gl_null.h>
#include <gl_point_size.h>
#include <gl_primitives.h>
#include <gl_render_mode.h>
#include <gl_scissor_test.h>
#include <gl_shade_model.h>
#include <gl_texture.h>

void
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
  glTexCoord2f (1.0, 0.0);
  glVertex3f (1.0, 0.0, 0.0);
  glTexCoord2f (0.0, 1.0);
  glVertex3f (0.0, 1.0, 0.0);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (0.0, 0.0, 1.0);
  glEnd ();
}

static void
test_quiescent (void)
{
  struct timespec req = { 1, 0 };

  glFinish ();
  nanosleep (&req, NULL);
}

void
test_prologue (char *buf)
{
  test_quiescent ();
  dump_rb_pre ();

  if (buf)
    {
      printf ("        %s\n", buf);

      if (mkdir (buf, 0777) < 0)
	{
	  if (errno != EEXIST)
	    {
	      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		       strerror (errno));
	      exit (EXIT_FAILURE);
	    }
	}

      if (chdir (buf) < 0)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}
    }
}

void
test_epilogue (bool buf)
{
  test_quiescent ();
  dump_rb_post ();

  if (buf)
    {
      if (chdir ("..") < 0)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}
    }
}

#define _(x) #x, x

static struct test_t tests[] = {
  {_(gl_alpha_test)},
  {_(gl_blend_dst)},
  {_(gl_blend_src)},
  {_(gl_clear_accum_buffer)},
  {_(gl_clear_color_buffer)},
  {_(gl_clear_depth_buffer)},
  {_(gl_clear_stencil_buffer)},
  {_(gl_clip_plane)},
  {_(gl_color_logic_op)},
  {_(gl_cull_face)},
  {_(gl_depth_func)},
  {_(gl_depth_mask)},
  {_(gl_depth_range)},
  {_(gl_front_face)},
  {_(gl_line_smooth)},
  {_(gl_line_smooth_hint)},
  {_(gl_line_width)},
  {_(gl_null)},
  {_(gl_point_size)},
  {_(gl_primitives)},
  {_(gl_render_mode_feedback)},
  {_(gl_render_mode_render)},
  {_(gl_render_mode_select)},
  {_(gl_scissor_test)},
  {_(gl_shade_model)},
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
  dump_interface ();
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
