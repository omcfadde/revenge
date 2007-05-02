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

#ifndef __MAIN_H__
#define __MAIN_H__

#define REG_ADDR 0xe5000000	/* lspci */
#define REG_SIZE 0x7d4		/* lspci */

#define RADEON_MC_AGP_LOCATION		0x014c
#define RADEON_CP_RB_BASE		0x0700
#define RADEON_CP_RB_CNTL		0x0704
#define RADEON_CP_RB_RPTR		0x0710
#define RADEON_CP_IB_BASE		0x0738
#define RADEON_CP_PACKET0		0x00000000
#define CP_PACKET0( reg, n )						\
	(RADEON_CP_PACKET0 | ((n) << 16) | ((reg) >> 2))

#define RADEON_CP_PACKET_MASK		0xC0000000
#define RADEON_CP_PACKET_COUNT_MASK	0x3fff0000
#define RADEON_CP_PACKET0_REG_MASK	0x000007ff
#define RADEON_CP_PACKET1_REG0_MASK	0x000007ff
#define RADEON_CP_PACKET1_REG1_MASK	0x003ff800

extern int option_blob;
extern int option_ioctl;
extern int option_verbose;
extern int option_ioctl_before;
void quiescent (void);
int main (int argc, char **argv);

#endif
