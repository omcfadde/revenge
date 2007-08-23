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

#ifndef __DETECT_H__
#define __DETECT_H__

extern unsigned int agp_addr;
extern unsigned int agp_len;
extern unsigned int fb_addr;
extern unsigned int fb_len;
extern unsigned int pcigart_addr;
extern unsigned int pcigart_end;
extern unsigned int pcigart_len;
extern unsigned int pcigart_start;
extern unsigned int reg_addr;
extern unsigned int reg_len;
void detect_agp_aperture (void);
void detect_fb_aperture (void);
void detect_pcigart_aperture (void);
void detect_igpgart_aperture (void);
void detect_reg_aperture (void);

#endif
