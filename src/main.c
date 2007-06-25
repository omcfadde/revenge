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

int option_disable_ib = 0;
int option_interface = 1;
int option_verbose = 0;

static void
opengl_open (void)
{
  SDL_Surface *Surface;

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      assert (0);
    }

  if (SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16) < 0)
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      assert (0);
    }

  if (SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 0) < 0)
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      assert (0);
    }

  if (SDL_GL_SetAttribute (SDL_GL_STENCIL_SIZE, 8) < 0)
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      assert (0);
    }

  if (!(Surface = SDL_SetVideoMode (640, 480, 0,
				    /* SDL_FULLSCREEN | */
				    SDL_NOFRAME | SDL_OPENGL)))
    {
      printf ("%s\n", SDL_GetError ());
      SDL_Quit ();
      assert (0);
    }
}

static void
opengl_close (void)
{
  SDL_Quit ();
}

static struct option long_options[] = {
  {"igp", no_argument, &option_interface, IF_IGP},
  {"agp", no_argument, &option_interface, IF_AGP},
  {"pcie", no_argument, &option_interface, IF_PCIE},
  {"disable-ib", no_argument, &option_disable_ib, 1},
  {"verbose", no_argument, &option_verbose, 1},
  {0, 0, 0, 0},
};

int mem_fd = -1;
unsigned int *agp_mem_map = NULL;
unsigned int *pcigart_mem_map = NULL;
unsigned int *reg_mem_map = NULL;

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

  if ((mem_fd = open ("/dev/mem", O_RDWR)) < 0)
    {
      assert (0);
    }

  detect_reg_aperture ();
  if ((reg_mem_map =
       mmap (NULL, reg_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
	     reg_addr)) < 0)
    {
      assert (0);
    }

  if (option_interface == IF_AGP)
    {
      detect_agp_aperture ();
      if ((agp_mem_map =
	   mmap (NULL, agp_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
		 agp_addr)) < 0)
	{
	  assert (0);
	}
    }
  else
    {
      if (option_interface == IF_IGP)
         detect_igpgart_aperture ();
      else
         detect_pcigart_aperture ();
      if ((pcigart_mem_map =
	   mmap (NULL, pcigart_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
		 pcigart_addr)) < 0)
	{
	  assert (0);
	}
    }

  opengl_open ();
  test ();
  opengl_close ();

  if (munmap (reg_mem_map, reg_len) < 0)
    {
      assert (0);
    }

  switch (option_interface)
    {
      case IF_AGP:
	if (munmap (agp_mem_map, agp_len) < 0)
	  assert (0);
	break;
      case IF_PCIE:
      case IF_IGP:
      default:
        if (munmap (pcigart_mem_map, pcigart_len) < 0)
	  assert (0);
	break;
    }

  if (close (mem_fd) < 0)
    {
      assert (0);
    }

  return 0;
}
