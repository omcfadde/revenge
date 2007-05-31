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

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "detect.h"
#include "main.h"
#include "ring.h"
#include "test.h"

int option_blob = 0;		/* define to 1 if you're using the blob. */
int option_ioctl = 1;		/* define to 1 for ioctl dumping. */
int option_verbose = 0;		/* define to 1 for printing of information that
				   is not normally meaningful; for example, the
				   ring head/tail pointers. Information that is
				   normally meaningful is always printed. */

int option_ioctl_before = 0;	/* define to 1 to dump the (write) ioctls before
				   executing them as well as after execuding
				   them (the default) see below. */

/*
 * Note about ioctls.
 *
 * _IOC_READ: the data from this ioctl is only read by the kernel.
 * _IOC_WRITE: the data from the ioctl is only written by the kernel.
 * _IOC_READ | _IOC_WRITE: the data from this ioctl is read and/or written by
 * the kernel.
 *
 * For _IOC_READ | _IOC_WRITE it is possible for the data to be set to one value
 * before executing the ioctl, then read by the kernel, then set to a different
 * value. This is why it can be useful to dump the ioctl's before and after
 * executing them, but it's not really needed by default.
 */

static int
alloc_opengl (void)
{
  SDL_Surface *Surface;

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      return 1;
    }

  if (SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16) < 0)
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      return 1;
    }

  if (SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 0) < 0)
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      return 1;
    }

  if (SDL_GL_SetAttribute (SDL_GL_STENCIL_SIZE, 8) < 0)
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      return 1;
    }

  if (!
      (Surface =
       SDL_SetVideoMode (640, 480, 0,
			 SDL_FULLSCREEN | SDL_NOFRAME | SDL_OPENGL)))
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      return 1;
    }

  return 0;
}

/*
 * TODO: add functionality to read/write any register (like radeontool), in
 * addidtion to the normal functionality of running the tests and creating a
 * dump.
 *
 * TODO: add option parsing (see option_* above)
 *
 * TODO: add ability to dump MMIO stuff via interfacing with the kernel. the
 * kernel would be patched with the kmmio patch, and this program will contain
 * the code to read that. See mmio-trace. This is very important as apparently
 * (unconfirmed) the newer blobs don't use indirect buffers anymore.
 *
 * TODO: make a better testing framework (test.c, test_<type>.c, ...) etc the
 * test_tri stuff in here is just a hack for testing.
 */

int
main (int argc, char **argv)
{
  if (alloc_opengl ())
    {
      return 1;
    }

  alloc_agp_aperture ();
  alloc_ring ();
  test ();
  free_ring ();

  return 0;
}
