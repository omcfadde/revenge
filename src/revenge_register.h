/*
 * $Id$
 * Copyright (C) 2007  Maciej Cencora <m.cencora@gmail.com>
 * Copyright (C) 2007  Oliver McFadden <omcfadde@gmail.com>
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

#ifndef __REGISTER_H__
#define __REGISTER_H__

#include <stdio.h>
#include <stdlib.h>

unsigned int register_read (unsigned int key);
void register_write (unsigned int key, unsigned int val);
unsigned int register_read_pcie (unsigned int key);
void register_write_pcie (unsigned int key, unsigned int val);
unsigned int register_read_igp (unsigned int key);
void register_write_igp (unsigned int key, unsigned int val);
unsigned int register_read_rs690 (unsigned int key);
void register_write_rs690 (unsigned int key, unsigned int val);

#endif
