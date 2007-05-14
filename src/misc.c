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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int _old_fd = -1;
static fpos_t _old_fpos;

void
open_fd (const char *name)
{
  char buf[BUFSIZ];

  fflush (stdout);

  fgetpos (stdout, &_old_fpos);
  _old_fd = dup (fileno (stdout));

  snprintf (buf, BUFSIZ, "revenge_%s.txt", name);

  if (!freopen (buf, "w", stdout))
    {
      /* ERR_PRINT ("Opening '%s' failed: %s\n", buf, strerror (errno)); */
    }
}

/* NOTE: This can be called only once after a redirect_output(),
 * they do not really nest.
 */
void
close_fd (void)
{
  fflush (stdout);
  if (_old_fd == -1)
    {
      /* ERR_PRINT("no previous stream to return output to, continuing with the current output.\n"); */
      return;
    }

  /* return the previous output file */
  if (dup2 (_old_fd, fileno (stdout)) == -1)
    {
      /* ERR_PRINT ("Redirecting to previous file failed: %s\n", strerror (errno)); */
    }
  else
    {
      close (_old_fd);
    }

  _old_fd = -1;
  clearerr (stdout);
  fsetpos (stdout, &_old_fpos);
}
