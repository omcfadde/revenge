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
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <config.h>

#include <detect.h>
#include <main.h>
#include <test.h>

int option_debug = 0;
int option_disable_ib = 0;
int option_interface = IF_AGP;
int option_verbose = 0;

static void
opengl_open (void)
{
  SDL_Surface *Surface;

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("%s: %s\n", __func__, SDL_GetError ());
      SDL_Quit ();
      exit (EXIT_FAILURE);
    }

  if (SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16) < 0)
    {
      printf ("%s: %s\n", __func__, SDL_GetError ());
      SDL_Quit ();
      exit (EXIT_FAILURE);
    }

  if (SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 0) < 0)
    {
      printf ("%s: %s\n", __func__, SDL_GetError ());
      SDL_Quit ();
      exit (EXIT_FAILURE);
    }

  if (SDL_GL_SetAttribute (SDL_GL_STENCIL_SIZE, 8) < 0)
    {
      printf ("%s: %s\n", __func__, SDL_GetError ());
      SDL_Quit ();
      exit (EXIT_FAILURE);
    }

  if (!(Surface = SDL_SetVideoMode (640, 480, 0,
				    /* SDL_FULLSCREEN | */
				    SDL_NOFRAME | SDL_OPENGL)))
    {
      printf ("%s: %s\n", __func__, SDL_GetError ());
      SDL_Quit ();
      exit (EXIT_FAILURE);
    }
}

static void
opengl_close (void)
{
  SDL_Quit ();
}

static struct option long_options[] = {
  {"agp", no_argument, &option_interface, IF_AGP},
  {"brief", no_argument, &option_verbose, 0},
  {"debug", no_argument, &option_debug, 1},
  {"disable-ib", no_argument, &option_disable_ib, 1},
  {"igp", no_argument, &option_interface, IF_IGP},
  {"output", required_argument, 0, 'o'},
  {"pci-e", no_argument, &option_interface, IF_PCIE},
  {"verbose", no_argument, &option_verbose, 1},
  {0, 0, 0, 0},
};

int mem_fd = -1;
unsigned int *agp_mem_map = NULL;
unsigned int *fb_mem_map = NULL;
unsigned int *pcigart_mem_map = NULL;
unsigned int *reg_mem_map = NULL;

int
main (int argc, char **argv)
{
  char buf[BUFSIZ];
  int i = 0;
  int opt;
  int revenge_rand;

  srand (time (NULL));
  revenge_rand = rand () & 0xffff;

  while ((opt = getopt_long (argc, argv, "bdiv", long_options, &i)) != -1)
    {
      switch (opt)
	{
	case 'b':
	  option_verbose = 0;
	  break;
	case 'd':
	  option_debug = 1;
	  break;
	case 'i':
	  option_disable_ib = 1;
	  break;
	case 'v':
	  option_verbose = 1;
	  break;
	case '?':
	case 0:
	  /* empty */
	  break;
	default:
	  abort ();
	  break;
	}
    }

  if ((mem_fd = open ("/dev/mem", O_RDWR)) < 0)
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  detect_reg_aperture ();
  if ((reg_mem_map =
       mmap (NULL, reg_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
	     reg_addr)) == MAP_FAILED)
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  detect_fb_aperture ();
  if ((fb_mem_map =
       mmap (NULL, fb_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
	     fb_addr)) == MAP_FAILED)
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  switch (option_interface)
    {
    case IF_AGP:
      detect_agp_aperture ();
      if ((agp_mem_map =
	   mmap (NULL, agp_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
		 agp_addr)) == MAP_FAILED)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}
      break;
    case IF_PCIE:
      detect_pcigart_aperture ();
      if ((pcigart_mem_map =
	   mmap (NULL, pcigart_len, PROT_READ | PROT_WRITE, MAP_SHARED,
		 mem_fd, pcigart_addr)) == MAP_FAILED)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}
      break;
    case IF_IGP:
      detect_igpgart_aperture ();
      if ((pcigart_mem_map =
	   mmap (NULL, pcigart_len, PROT_READ | PROT_WRITE, MAP_SHARED,
		 mem_fd, pcigart_addr)) == MAP_FAILED)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}
      break;
    default:
      assert (0);
      break;
    }

  snprintf (buf, BUFSIZ, "%1$s-%2$s-%3$04x-%4$04x", PACKAGE_NAME,
	    PACKAGE_VERSION, reg_device_id, revenge_rand);

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

  opengl_open ();
  test ();
  opengl_close ();

  if (munmap (reg_mem_map, reg_len) < 0)
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  if (munmap (fb_mem_map, fb_len) < 0)
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  switch (option_interface)
    {
    case IF_AGP:
      if (munmap (agp_mem_map, agp_len) < 0)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}
      break;
    case IF_PCIE:
    case IF_IGP:
    default:
      if (munmap (pcigart_mem_map, pcigart_len) < 0)
	{
	  fprintf (stderr, "%s: %s\n", program_invocation_short_name,
		   strerror (errno));
	  exit (EXIT_FAILURE);
	}
      break;
    }

  if (close (mem_fd) < 0)
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  if (chdir ("..") < 0)
    {
      fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	       strerror (errno));
      exit (EXIT_FAILURE);
    }

  snprintf (buf, BUFSIZ,
	    "tar -cjf %1$s-%2$s-%3$04x-%4$04x.tar.bz2 %1$s-%2$s-%3$04x-%4$04x/",
	    PACKAGE_NAME, PACKAGE_VERSION, reg_device_id, revenge_rand);
  system (buf);

  return 0;
}
