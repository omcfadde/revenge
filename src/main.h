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

#define IF_PCIE 0
#define IF_AGP 1
#define IF_IGP 2

#define AVIVO_CRTC1_FB_LOCATION				0x6110
#define AVIVO_CRTC1_FB_END					0x6118

#define RADEON_MC_AGP_LOCATION		0x014c
#define RADEON_MC_FB_LOCATION		0x0148
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

#define RADEON_PCIE_INDEX               0x0030
#define RADEON_PCIE_DATA                0x0034
#define RADEON_PCIE_TX_GART_CNTL	0x10
#	define RADEON_PCIE_TX_GART_EN   	(1 << 0)
#	define RADEON_PCIE_TX_GART_UNMAPPED_ACCESS_PASS_THRU (0<<1)
#	define RADEON_PCIE_TX_GART_UNMAPPED_ACCESS_CLAMP_LO  (1<<1)
#	define RADEON_PCIE_TX_GART_UNMAPPED_ACCESS_DISCARD   (3<<1)
#	define RADEON_PCIE_TX_GART_MODE_32_128_CACHE	(0<<3)
#	define RADEON_PCIE_TX_GART_MODE_8_4_128_CACHE	(1<<3)
#	define RADEON_PCIE_TX_GART_CHK_RW_VALID_EN      (1<<5)
#	define RADEON_PCIE_TX_GART_INVALIDATE_TLB	(1<<8)
#define RADEON_PCIE_TX_DISCARD_RD_ADDR_LO 0x11
#define RADEON_PCIE_TX_DISCARD_RD_ADDR_HI 0x12
#define RADEON_PCIE_TX_GART_BASE  	0x13
#define RADEON_PCIE_TX_GART_START_LO	0x14
#define RADEON_PCIE_TX_GART_START_HI	0x15
#define RADEON_PCIE_TX_GART_END_LO	0x16
#define RADEON_PCIE_TX_GART_END_HI	0x17

#define ATI_PCIGART_PAGE_SIZE		4096	/**< PCI GART page size */

#define RADEON_IGPGART_INDEX            0x168
#define RADEON_IGPGART_DATA             0x16c
#define RADEON_IGPGART_UNK_18           0x18
#define RADEON_IGPGART_CTRL             0x2b
#define RADEON_IGPGART_BASE_ADDR        0x2c
#define RADEON_IGPGART_FLUSH            0x2e
#define RADEON_IGPGART_ENABLE           0x38
#define RADEON_IGPGART_UNK_39           0x39

#define R300_TX_SIZE_0                      0x4480
#       define R300_TX_WIDTHMASK_SHIFT           0
#       define R300_TX_WIDTHMASK_MASK            0x7ff
#       define R300_TX_HEIGHTMASK_SHIFT          11
#       define R300_TX_HEIGHTMASK_MASK           0x7ff
#       define R300_TX_UNK23                     (1 << 23)
#       define R300_TX_SIZE_SHIFT                26	/* largest of width, height */
#       define R300_TX_SIZE_MASK                 (15 << 26)

#define R300_TX_OFFSET_0                    0x4540
/* BEGIN: Guess from R200 */
#       define R300_TXO_ENDIAN_NO_SWAP           (0 << 0)
#       define R300_TXO_ENDIAN_BYTE_SWAP         (1 << 0)
#       define R300_TXO_ENDIAN_WORD_SWAP         (2 << 0)
#       define R300_TXO_ENDIAN_HALFDW_SWAP       (3 << 0)
#       define R300_TXO_OFFSET_MASK              /* 0xffffffe0 */ 0x7ffffff
#       define R300_TXO_OFFSET_SHIFT             5
/* END */

extern int option_debug;
extern int option_disable_ib;
extern int option_interface;
extern int option_verbose;
extern int mem_fd;
extern unsigned int *agp_mem_map;
extern unsigned int *pcigart_mem_map;
extern unsigned int *reg_mem_map;
int main (int argc, char **argv);

#endif
