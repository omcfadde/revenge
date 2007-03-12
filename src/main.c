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
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"
#include "maps.h"

int option_blob = 0;		// define to 1 if you're using the blob.
int option_ioctl = 1;		// define to 1 for ioctl dumping.

void
alloc_opengl (void)
{
  SDL_Surface *Surface;

  SDL_Init (SDL_INIT_VIDEO);

  SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 0);
  SDL_GL_SetAttribute (SDL_GL_STENCIL_SIZE, 8);

  Surface =
    SDL_SetVideoMode (640, 480, 0,
		      SDL_OPENGL | SDL_NOFRAME /*| SDL_FULLSCREEN */ );
}

inline static void
test_prologue (void)
{
  sleep (1);
  glFlush ();
  sleep (1);
  before ();
  sleep (1);
  glFlush ();
  sleep (1);
}

inline static void
test_epilogue (void)
{
  sleep (1);
  glFlush ();
  sleep (1);
  after ();
  sleep (1);
  glFlush ();
  sleep (1);
}

void
test_tri (void)
{
  glBegin (GL_TRIANGLES);
  glVertex3f (1.0, 0.0, 0.0);
  glVertex3f (0.0, 1.0, 0.0);
  glVertex3f (0.0, 0.0, 1.0);
  glEnd ();
}

// TODO: add functionality to read/write any register (like radeontool), in
// addidtion to the normal functionality of running the tests and creating a
// dump.
//
// TODO: add option parsing (see option_* above)
//
// TODO: add ability to dump MMIO stuff via interfacing with the kernel. the
// kernel would be patched with the kmmio patch, and this program will contain
// the code to read that. See mmio-trace. This is very important as apparently
// (unconfirmed) the newer blobs don't use indirect buffers anymore.
//
// TODO: make a better testing framework (test.c, test_<type>.c, ...) etc the
// test_tri stuff in here is just a hack for testing.

int
main (int argc, char **argv)
{
  alloc_opengl ();
  alloc_maps ();

  sleep (1);
  glFlush ();
  sleep (1);

  test_prologue ();
  test_tri ();
  test_epilogue ();

  free_maps ();

  return 0;
}
