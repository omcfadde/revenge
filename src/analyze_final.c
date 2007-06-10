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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyze.h"
#include "analyze_final.h"
#include "resolv.h"

/*
 * this is a horrible quick and dirty hack! it can miss a lot of writes, and
 * generally sucks.
 */

static int regs[0xffff];
static int regs_cnt[0xffff];

static void
analyze_final_begin (void)
{
  memset (regs, 0, sizeof (int) * 0xffff);
  memset (regs_cnt, 0, sizeof (int) * 0xffff);
}

static void
analyze_final_end (void)
{
  int i;
  char *resolv;

  for (i = 0; i < 0xffff; i++)
    {
      if (regs_cnt[i] > 0)
	{
	  if ((resolv = resolv_reg (i)))
	    {
	      printf ("%33s 0x%04x <- 0x%08x\n", resolv, i, regs[i]);
	      free (resolv);
	    }
	  else
	    {
	      printf ("                                  0x%04x <- 0x%08x\n", i,
		      regs[i]);
	    }
	}
    }
}

static void
analyze_final_reg (unsigned int key, unsigned int val)
{
  assert (key < 0xffff);
  regs[key] = val;
  regs_cnt[key] = regs_cnt[key] + 1;
}

const struct analyze_t analyze_final = {
  .begin = analyze_final_begin,
  .end = analyze_final_end,
  .reg = analyze_final_reg,
};
