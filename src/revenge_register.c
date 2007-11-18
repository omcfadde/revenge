/*
 * $Id$
 * Copyright (C) 2007  Maciej Cencora <m.cencora@gmail.com>
 * Copyright (C) 2007  Oliver McFadden <z3ro.geek@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include <revenge_main.h>

unsigned int
register_read (unsigned int key)
{
  return reg_mem_map[key >> 2];
}

void
register_write (unsigned int key, unsigned int val)
{
  reg_mem_map[key >> 2] = val;
}

unsigned int
register_read_pcie (unsigned int key)
{
  register_write (RADEON_PCIE_INDEX, key);
  return register_read (RADEON_PCIE_DATA);
}

void
register_write_pcie (unsigned int key, unsigned int val)
{
  register_write (RADEON_PCIE_INDEX, key);
  register_write (RADEON_PCIE_DATA, val);
}

unsigned int
register_read_igp (unsigned int key)
{
  unsigned int ret;

  register_write (RADEON_IGPGART_INDEX, key & 0x7f);
  ret = register_read (RADEON_IGPGART_DATA);
  register_write (RADEON_IGPGART_INDEX, 0x7f);
  return ret;
}

void
register_write_igp (unsigned int key, unsigned int val)
{
  register_write (RADEON_IGPGART_INDEX, (key & 0x7f) | (1 << 8));
  register_write (RADEON_IGPGART_DATA, val);
  register_write (RADEON_IGPGART_INDEX, 0x7f);
}

unsigned int
register_read_rs690 (unsigned int key)
{
  register_write (RADEON_RS690GART_INDEX, key & RADEON_RS690GART_INDEX_MASK);
  return register_read (RADEON_RS690GART_DATA);
}

void
register_write_rs690 (unsigned int key, unsigned int val)
{
  register_write (RADEON_RS690GART_INDEX,
		  (key & RADEON_RS690GART_INDEX_MASK) |
		  RADEON_RS690GART_INDEX_WRITEBIT);
  register_write (RADEON_RS690GART_DATA, val);
  register_write (RADEON_RS690GART_INDEX, RADEON_RS690GART_INDEX_WRITEACK);
}
