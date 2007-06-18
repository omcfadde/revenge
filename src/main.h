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

#ifndef __MAIN_H__
#define __MAIN_H__

/* #define DEBUG */

#define RADEON_MC_AGP_LOCATION		0x014c
#define RADEON_CP_RB_BASE		0x0700
#define RADEON_CP_RB_CNTL		0x0704
#define RADEON_CP_RB_RPTR		0x0710
#define RADEON_CP_IB_BASE		0x0738
#define RADEON_CP_IB_BUFSZ		0x073c
#define RADEON_CP_PACKET0		0x00000000
#define CP_PACKET0( reg, n )						\
	(RADEON_CP_PACKET0 | ((n) << 16) | ((reg) >> 2))

#define RADEON_CP_PACKET_MASK		0xC0000000
#define RADEON_CP_PACKET_COUNT_MASK	0x3fff0000
#define RADEON_CP_PACKET0_REG_MASK	0x000007ff
#define RADEON_CP_PACKET1_REG0_MASK	0x000007ff
#define RADEON_CP_PACKET1_REG1_MASK	0x003ff800

/* CP command packets */
#define RADEON_CP_PACKET0		0x00000000
#	define RADEON_ONE_REG_WR		(1 << 15)
#define RADEON_CP_PACKET1		0x40000000
#define RADEON_CP_PACKET2		0x80000000
#define RADEON_CP_PACKET3		0xC0000000
#       define RADEON_CP_NOP                    0x10
#       define RADEON_CP_NEXT_CHAR              0x19
#       define RADEON_CP_PLY_NEXTSCAN           0x1D
#       define RADEON_CP_SET_SCISSORS           0x1E
	     /* GEN_INDX_PRIM is unsupported starting with R300 */
#	define RADEON_3D_RNDR_GEN_INDX_PRIM	0x23
#	define RADEON_WAIT_FOR_IDLE		0x26
#	define RADEON_3D_DRAW_VBUF		0x28
#	define RADEON_3D_DRAW_IMMD		0x29
#	define RADEON_3D_DRAW_INDX		0x2A
#       define RADEON_CP_LOAD_PALETTE           0x2C
#	define RADEON_3D_LOAD_VBPNTR		0x2F
#	define RADEON_MPEG_IDCT_MACROBLOCK	0x30
#	define RADEON_MPEG_IDCT_MACROBLOCK_REV	0x31
#	define RADEON_3D_CLEAR_ZMASK		0x32
#	define RADEON_CP_INDX_BUFFER		0x33
#       define RADEON_CP_3D_DRAW_VBUF_2         0x34
#       define RADEON_CP_3D_DRAW_IMMD_2         0x35
#       define RADEON_CP_3D_DRAW_INDX_2         0x36
#	define RADEON_3D_CLEAR_HIZ		0x37
//#       define RADEON_CP_3D_CLEAR_CMASK         0x00003802
#	define RADEON_CNTL_HOSTDATA_BLT		0x94
#	define RADEON_CNTL_PAINT_MULTI		0x9A
#	define RADEON_CNTL_BITBLT_MULTI		0x9B
//#     define RADEON_CNTL_SET_SCISSORS         0xC0001E00

extern int option_verbose;
extern int mem_fd;
extern unsigned int *agp_mem_map, *reg_mem_map;
int main (int argc, char **argv);

#endif
