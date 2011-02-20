/*
 * $Id$
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

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <revenge_detect.h>
#include <revenge_main.h>
#include <revenge_memory.h>
#include <revenge_register.h>

static unsigned int ib_addr = 0, ib_num = 0, ib_size = 0;
static unsigned int rb_addr = 0, rb_head = 0, rb_size = 0, rb_tail = 0;
static unsigned int tx_addr = 0, tx_width = 0, tx_height = 0;

static void dump_ib (void);

static void
dump_tx (void)
{
  if (option_debug)
    {
      printf ("%s: tx_addr = 0x%08x tx_width = 0x%08x tx_height = 0x%08x\n",
	      __func__, tx_addr, tx_width, tx_height);
    }
}

static void
dump_reg (unsigned int key, unsigned int val)
{
  int i;

  if (option_debug)
    {
      printf ("%s: key = 0x%04x val = 0x%08x\n", __func__, key, val);
    }

  if (key == RADEON_CP_IB_BASE)
    {
      ib_addr = val;
    }

  if (key == RADEON_CP_IB_BUFSZ)
    {
      ib_size = val;
      dump_ib ();
      ib_addr = ib_size = 0;
    }

  for (i = 0; i < gl_max_texture_units; i++)
    {
      if (key == R300_TX_SIZE_0 + (i << 2))
	{
	  tx_width =
	    (val >> R300_TX_WIDTHMASK_SHIFT) & R300_TX_WIDTHMASK_MASK;
	  tx_height =
	    (val >> R300_TX_HEIGHTMASK_SHIFT) & R300_TX_HEIGHTMASK_MASK;
	}

      if (key == R300_TX_OFFSET_0 + (i << 2))
	{
	  tx_addr = (val >> R300_TXO_OFFSET_SHIFT) & R300_TXO_OFFSET_MASK;
	  dump_tx ();
	  tx_addr = tx_width = tx_height = 0;
	}
    }
}

static int
dump_packet0 (unsigned int packet_type, unsigned int packet_cnt,
	      unsigned int packet_bit15, unsigned int packet_reg,
	      unsigned int *mem_map, FILE * file)
{
  int i;
  unsigned int reg;

  if (option_debug)
    {
      printf ("%s: type = %d cnt = %d bit15 = %d reg = 0x%04x\n", __func__,
	      packet_type, packet_cnt, packet_bit15, packet_reg);
    }

  for (i = 0; i < packet_cnt + 1; i++)
    {
      fprintf (file, "%08x\n", mem_map[i]);
      reg = packet_bit15 ? packet_reg : packet_reg + (i << 2);
      dump_reg (reg, mem_map[i]);
    }

  return packet_cnt + 1;
}

static int
dump_packet2 (unsigned int packet_type, unsigned int packet_cnt,
	      unsigned int packet_bit15, unsigned int packet_reg,
	      unsigned int *mem_map, FILE * file)
{
  if (option_debug)
    {
      printf ("%s: type = %d cnt = %d bit15 = %d reg = 0x%04x\n", __func__,
	      packet_type, packet_cnt, packet_bit15, packet_reg);
    }

  return 0;
}

static int
dump_packet3 (unsigned int packet_type, unsigned int packet_cnt,
	      unsigned int packet_opcode, unsigned int *mem_map, FILE * file)
{
  int i;

  if (option_debug)
    {
      printf ("%s: type = %d cnt = %d opcode = 0x%02x\n", __func__,
	      packet_type, packet_cnt, packet_opcode);
    }

  for (i = 0; i < packet_cnt + 1; i++)
    {
      fprintf (file, "%08x\n", mem_map[i]);
      if (option_debug)
	{
	  printf ("%s: 0x%08x\n", __func__, mem_map[i]);
	}
    }

  return packet_cnt + 1;
}

static void
dump_packets (unsigned int head, unsigned int tail, unsigned int *mem_map,
	      char *name)
{
  FILE *file;
  int i;
  unsigned int packet_type, packet_cnt, packet_bit15, packet_reg,
    packet_opcode;
  unsigned int proc;

  if (!(file = fopen (name, "w")))
    {
      fprintf (stderr, "%s: %s:%d: %s\n", program_invocation_short_name,
	       __FILE__, __LINE__, strerror (errno));
      exit (EXIT_FAILURE);
    }

  assert (tail >= head);

  for (i = head; i < tail; i += proc + 1)
    {
      assert (mem_map[i]);

      packet_type = (mem_map[i] >> 30) & 0x3;
      packet_cnt = (mem_map[i] >> 16) & 0x3fff;
      packet_bit15 = (mem_map[i] >> 15) & 0x1;
      packet_reg = ((mem_map[i] >> 0) & 0x1fff) << 2;

      packet_opcode = (mem_map[i] >> 8) & 0xff;

      fprintf (file, "%08x\n", mem_map[i]);

      switch (packet_type)
	{
	case 0x0:
	  proc =
	    dump_packet0 (packet_type, packet_cnt, packet_bit15, packet_reg,
			  &mem_map[i + 1], file);
	  break;
	case 0x2:
	  proc =
	    dump_packet2 (packet_type, packet_cnt, packet_bit15, packet_reg,
			  &mem_map[i + 1], file);
	  break;
	case 0x3:
	  proc =
	    dump_packet3 (packet_type, packet_cnt, packet_opcode,
			  &mem_map[i + 1], file);
	  break;
	default:
	  assert (0);
	  break;
	}
    }

  assert (i - tail <= 1);

  fclose (file);
}

static void
dump_ib (void)
{
  char buf[BUFSIZ];
  unsigned int *ib_mem_map;

  if (!option_disable_ib)
    {
      if (option_debug)
	{
	  printf ("%s: ib_addr = 0x%08x ib_size = 0x%08x\n", __func__,
		  ib_addr, ib_size);
	}

      snprintf (buf, BUFSIZ, "ib_%04d.txt", ib_num);
      ib_mem_map = memory_read (ib_addr, ib_size << 2);
      dump_packets (0, ib_size, ib_mem_map, buf);
      free (ib_mem_map);
      ib_num++;
    }
}

void
dump_rb_pre (void)
{
  ib_num = 0;
  rb_addr = register_read (RADEON_CP_RB_BASE);
  rb_head = register_read (RADEON_CP_RB_RPTR);
  rb_size = (1 << ((register_read (RADEON_CP_RB_CNTL) & 0xff) + 1));

  if (option_debug)
    {
      printf ("%s: rb_addr = 0x%08x rb_head = 0x%08x rb_size = 0x%08x\n",
	      __func__, rb_addr, rb_head, rb_size);
    }
}

void
dump_rb_post (void)
{
  char buf[BUFSIZ];
  unsigned int *rb_mem_map;

  rb_tail = register_read (RADEON_CP_RB_RPTR);

  if (option_debug)
    {
      printf ("%s: rb_tail = 0x%08x (%d)\n", __func__, rb_tail,
	      rb_tail - rb_head);
    }

  snprintf (buf, BUFSIZ, "rb.txt");
  rb_mem_map = memory_read (rb_addr, rb_size << 2);
  dump_packets (rb_head, rb_tail, rb_mem_map, buf);
  free (rb_mem_map);
}
