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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * horrible hack. will leak memory!
 */
char *
resolv_reg (unsigned int key)
{
  FILE *file;
  char *delim = " \t";
  char *line = NULL;
  char *token;
  char keystr[BUFSIZ];
  size_t len = 0;

  if (!(file = fopen ("r300_reg.h", "r")))
    {
      return NULL;
    }

  /* loop over all the lines */
  while (getline (&line, &len, file) != -1)
    {
      snprintf (keystr, BUFSIZ, "0x%04x", key);
      /* find one that has 0xdead on it */
      if (strcasestr (line, keystr))
	{
	  /* now find the R300_* token in the line... */
	  for (token = strtok (line, delim); token != NULL;
	       token = strtok (NULL, delim))
	    {
	      /* found it. return it. */
	      if (strcasestr (token, "R300_"))
		{
		  return token;
		}
	    }
	}
    }

  if (line)
    {
      free (line);
    }

  fclose (file);

  return NULL;
}
