/*
 * $Id$
 * Copyright (C) 2007  Christoph Brill <egore911@egore911.de>
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

/*
 * TODO: add functionality to read/write any register (like radeontool), in
 * addidtion to the normal functionality of running the tests and creating a
 * dump.
 *
 * TODO: add ability to dump MMIO stuff via interfacing with the kernel. the
 * kernel would be patched with the kmmio patch, and this program will contain
 * the code to read that. See mmio-trace. This is very important as apparently
 * (unconfirmed) the newer blobs don't use indirect buffers anymore.
 *
 * TODO: make a better testing framework (test.c, test_<type>.c, ...) etc the
 * test_tri stuff in here is just a hack for testing.
 */

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

#include <SDL.h>
#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "detect.h"
#include "detect_agp.h"
#include "main.h"
#include "ring.h"
#include "test.h"

#include "config.h"

int option_blob = 0;		/* define to 1 if you're using the blob. */
int option_ioctl = 0;		/* define to 1 for ioctl dumping. */
int option_ioctl_before = 0;	/* define to 1 to dump the (write) ioctls
				   before executing them as well as after
				   executing them (the default) see below. */
int option_per_file = 0;	/* define to 1 for per file stdout redirection. */
int option_verbose = 0;		/* define to 1 for printing of information that
				   is not normally meaningful; for example, the
				   ring head/tail pointers. Information that is
				   normally meaningful is always printed. */

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

  if (!(Surface = SDL_SetVideoMode (640, 480, 0,
				    /* SDL_FULLSCREEN | */
				    SDL_NOFRAME | SDL_OPENGL)))
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      return 1;
    }

  return 0;
}

static struct option long_options[] = {
  {"blob", no_argument, &option_blob, 1},
  {"ioctl", no_argument, &option_ioctl, 1},
  {"ioctl-before", no_argument, &option_ioctl_before, 1},
  {"per-file", no_argument, &option_per_file, 1},
  {"verbose", no_argument, &option_verbose, 1},
  {0, 0, 0, 0},
};

int mem_fd;
unsigned int *agp_mem_map, *mem_map, *ring_mem_map;

int
main (int argc, char **argv)
{
  int i = 0;
  int opt;

  while ((opt = getopt_long (argc, argv, "v", long_options, &i)) != -1)
    {
      switch (opt)
	{
	case 0:
	  /* empty */
	  break;
	case 'v':
	  option_verbose = 1;
	  break;
	default:
	  abort ();
	  break;
	}
    }

  detect_agp_aperture ();
  detect_reg_aperture ();

  if ((mem_fd = open ("/dev/mem", O_RDWR)) < 0)
    {
      return 1;
    }

  if ((mem_map =
       mmap (NULL, reg_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
	     reg_addr)) < 0)
    {
      return 1;
    }

  if ((agp_mem_map =
       mmap (NULL, agp_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
	     agp_addr)) < 0)
    {
      return 1;
    }

  if (alloc_opengl ())
    {
      return 1;
    }

  alloc_ring ();
  test ();

  if (munmap (agp_mem_map, agp_len) < 0)
    {
      assert (0);
    }

  if (munmap (mem_map, reg_len) < 0)
    {
      assert (0);
    }

  close (mem_fd);

  return 0;
}
