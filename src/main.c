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
#include "main.h"
#include "test.h"

#include "config.h"

int option_agp = 1;
int option_verbose = 0;

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
  {"agp", no_argument, &option_agp, 1},
  {"verbose", no_argument, &option_verbose, 1},
  {0, 0, 0, 0},
};

int mem_fd;
unsigned int *agp_mem_map, *reg_mem_map;

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

  if ((reg_mem_map =
       mmap (NULL, reg_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
	     reg_addr)) < 0)
    {
      return 1;
    }

  if (option_agp)
    {
      if ((agp_mem_map =
	   mmap (NULL, agp_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
		 agp_addr)) < 0)
	{
	  return 1;
	}
    }

  if (alloc_opengl ())
    {
      return 1;
    }

  test ();

  if (option_agp)
    {
      if (munmap (agp_mem_map, agp_len) < 0)
	{
	  assert (0);
	}
    }

  if (munmap (reg_mem_map, reg_len) < 0)
    {
      assert (0);
    }

  close (mem_fd);

  return 0;
}
