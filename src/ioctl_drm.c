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

#include <drm/drm.h>

#include "ioctl.h"

// TODO: this is just an incomplete example. I think I should print the structs
// in a gdb like format...
static void
pretty_drm_ioctl_version (struct ioctl_t *ioctl, int *ioctl_ptr)
{
  drm_version_t *drm_version = (drm_version_t *) ioctl_ptr;

  fprintf (stderr, "%d %d %d ...\n", drm_version->version_major,
	   drm_version->version_minor, drm_version->version_patchlevel);
}

static void
pretty_drm_ioctl_get_unique (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_get_magic (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_irq_busid (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_get_map (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_get_client (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_get_stats (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_set_version (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_set_unique (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_auth_magic (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_block (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_unblock (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_control (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_add_map (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_add_bufs (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_mark_bufs (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_info_bufs (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_map_bufs (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_free_bufs (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_rm_map (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_set_sarea_ctx (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_get_sarea_ctx (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_add_ctx (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_rm_ctx (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_mod_ctx (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_get_ctx (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_switch_ctx (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_new_ctx (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_res_ctx (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_add_draw (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_rm_draw (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_dma (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_lock (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_unlock (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_finish (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_agp_acquire (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_agp_release (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_agp_enable (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_agp_info (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_agp_alloc (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_agp_free (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_agp_bind (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_agp_unbind (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_sg_alloc (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_sg_free (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_wait_vblank (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_fence (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_bufobj (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_mm_init (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_update_draw (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

/* commit 1b3a6d47751018c75e4333ee3ab8ba21dbd55bdd */
struct ioctl_t drm_ioctls[] = {
  {"DRM_IOCTL_VERSION", pretty_drm_ioctl_version,
   _IOC_DIR (DRM_IOCTL_VERSION), _IOC_TYPE (DRM_IOCTL_VERSION),
   _IOC_NR (DRM_IOCTL_VERSION), _IOC_SIZE (DRM_IOCTL_VERSION)},
  {"DRM_IOCTL_GET_UNIQUE", pretty_drm_ioctl_get_unique,
   _IOC_DIR (DRM_IOCTL_GET_UNIQUE), _IOC_TYPE (DRM_IOCTL_GET_UNIQUE),
   _IOC_NR (DRM_IOCTL_GET_UNIQUE), _IOC_SIZE (DRM_IOCTL_GET_UNIQUE)},
  {"DRM_IOCTL_GET_MAGIC", pretty_drm_ioctl_get_magic,
   _IOC_DIR (DRM_IOCTL_GET_MAGIC), _IOC_TYPE (DRM_IOCTL_GET_MAGIC),
   _IOC_NR (DRM_IOCTL_GET_MAGIC), _IOC_SIZE (DRM_IOCTL_GET_MAGIC)},
  {"DRM_IOCTL_IRQ_BUSID", pretty_drm_ioctl_irq_busid,
   _IOC_DIR (DRM_IOCTL_IRQ_BUSID), _IOC_TYPE (DRM_IOCTL_IRQ_BUSID),
   _IOC_NR (DRM_IOCTL_IRQ_BUSID), _IOC_SIZE (DRM_IOCTL_IRQ_BUSID)},
  {"DRM_IOCTL_GET_MAP", pretty_drm_ioctl_get_map,
   _IOC_DIR (DRM_IOCTL_GET_MAP), _IOC_TYPE (DRM_IOCTL_GET_MAP),
   _IOC_NR (DRM_IOCTL_GET_MAP), _IOC_SIZE (DRM_IOCTL_GET_MAP)},
  {"DRM_IOCTL_GET_CLIENT", pretty_drm_ioctl_get_client,
   _IOC_DIR (DRM_IOCTL_GET_CLIENT), _IOC_TYPE (DRM_IOCTL_GET_CLIENT),
   _IOC_NR (DRM_IOCTL_GET_CLIENT), _IOC_SIZE (DRM_IOCTL_GET_CLIENT)},
  {"DRM_IOCTL_GET_STATS", pretty_drm_ioctl_get_stats,
   _IOC_DIR (DRM_IOCTL_GET_STATS), _IOC_TYPE (DRM_IOCTL_GET_STATS),
   _IOC_NR (DRM_IOCTL_GET_STATS), _IOC_SIZE (DRM_IOCTL_GET_STATS)},
  {"DRM_IOCTL_SET_VERSION", pretty_drm_ioctl_set_version,
   _IOC_DIR (DRM_IOCTL_SET_VERSION), _IOC_TYPE (DRM_IOCTL_SET_VERSION),
   _IOC_NR (DRM_IOCTL_SET_VERSION), _IOC_SIZE (DRM_IOCTL_SET_VERSION)},
  {"DRM_IOCTL_SET_UNIQUE", pretty_drm_ioctl_set_unique,
   _IOC_DIR (DRM_IOCTL_SET_UNIQUE), _IOC_TYPE (DRM_IOCTL_SET_UNIQUE),
   _IOC_NR (DRM_IOCTL_SET_UNIQUE), _IOC_SIZE (DRM_IOCTL_SET_UNIQUE)},
  {"DRM_IOCTL_AUTH_MAGIC", pretty_drm_ioctl_auth_magic,
   _IOC_DIR (DRM_IOCTL_AUTH_MAGIC), _IOC_TYPE (DRM_IOCTL_AUTH_MAGIC),
   _IOC_NR (DRM_IOCTL_AUTH_MAGIC), _IOC_SIZE (DRM_IOCTL_AUTH_MAGIC)},
  {"DRM_IOCTL_BLOCK", pretty_drm_ioctl_block, _IOC_DIR (DRM_IOCTL_BLOCK),
   _IOC_TYPE (DRM_IOCTL_BLOCK), _IOC_NR (DRM_IOCTL_BLOCK),
   _IOC_SIZE (DRM_IOCTL_BLOCK)},
  {"DRM_IOCTL_UNBLOCK", pretty_drm_ioctl_unblock,
   _IOC_DIR (DRM_IOCTL_UNBLOCK), _IOC_TYPE (DRM_IOCTL_UNBLOCK),
   _IOC_NR (DRM_IOCTL_UNBLOCK), _IOC_SIZE (DRM_IOCTL_UNBLOCK)},
  {"DRM_IOCTL_CONTROL", pretty_drm_ioctl_control,
   _IOC_DIR (DRM_IOCTL_CONTROL), _IOC_TYPE (DRM_IOCTL_CONTROL),
   _IOC_NR (DRM_IOCTL_CONTROL), _IOC_SIZE (DRM_IOCTL_CONTROL)},
  {"DRM_IOCTL_ADD_MAP", pretty_drm_ioctl_add_map,
   _IOC_DIR (DRM_IOCTL_ADD_MAP), _IOC_TYPE (DRM_IOCTL_ADD_MAP),
   _IOC_NR (DRM_IOCTL_ADD_MAP), _IOC_SIZE (DRM_IOCTL_ADD_MAP)},
  {"DRM_IOCTL_ADD_BUFS", pretty_drm_ioctl_add_bufs,
   _IOC_DIR (DRM_IOCTL_ADD_BUFS), _IOC_TYPE (DRM_IOCTL_ADD_BUFS),
   _IOC_NR (DRM_IOCTL_ADD_BUFS), _IOC_SIZE (DRM_IOCTL_ADD_BUFS)},
  {"DRM_IOCTL_MARK_BUFS", pretty_drm_ioctl_mark_bufs,
   _IOC_DIR (DRM_IOCTL_MARK_BUFS), _IOC_TYPE (DRM_IOCTL_MARK_BUFS),
   _IOC_NR (DRM_IOCTL_MARK_BUFS), _IOC_SIZE (DRM_IOCTL_MARK_BUFS)},
  {"DRM_IOCTL_INFO_BUFS", pretty_drm_ioctl_info_bufs,
   _IOC_DIR (DRM_IOCTL_INFO_BUFS), _IOC_TYPE (DRM_IOCTL_INFO_BUFS),
   _IOC_NR (DRM_IOCTL_INFO_BUFS), _IOC_SIZE (DRM_IOCTL_INFO_BUFS)},
  {"DRM_IOCTL_MAP_BUFS", pretty_drm_ioctl_map_bufs,
   _IOC_DIR (DRM_IOCTL_MAP_BUFS), _IOC_TYPE (DRM_IOCTL_MAP_BUFS),
   _IOC_NR (DRM_IOCTL_MAP_BUFS), _IOC_SIZE (DRM_IOCTL_MAP_BUFS)},
  {"DRM_IOCTL_FREE_BUFS", pretty_drm_ioctl_free_bufs,
   _IOC_DIR (DRM_IOCTL_FREE_BUFS), _IOC_TYPE (DRM_IOCTL_FREE_BUFS),
   _IOC_NR (DRM_IOCTL_FREE_BUFS), _IOC_SIZE (DRM_IOCTL_FREE_BUFS)},
  {"DRM_IOCTL_RM_MAP", pretty_drm_ioctl_rm_map, _IOC_DIR (DRM_IOCTL_RM_MAP),
   _IOC_TYPE (DRM_IOCTL_RM_MAP), _IOC_NR (DRM_IOCTL_RM_MAP),
   _IOC_SIZE (DRM_IOCTL_RM_MAP)},
  {"DRM_IOCTL_SET_SAREA_CTX", pretty_drm_ioctl_set_sarea_ctx,
   _IOC_DIR (DRM_IOCTL_SET_SAREA_CTX), _IOC_TYPE (DRM_IOCTL_SET_SAREA_CTX),
   _IOC_NR (DRM_IOCTL_SET_SAREA_CTX), _IOC_SIZE (DRM_IOCTL_SET_SAREA_CTX)},
  {"DRM_IOCTL_GET_SAREA_CTX", pretty_drm_ioctl_get_sarea_ctx,
   _IOC_DIR (DRM_IOCTL_GET_SAREA_CTX), _IOC_TYPE (DRM_IOCTL_GET_SAREA_CTX),
   _IOC_NR (DRM_IOCTL_GET_SAREA_CTX), _IOC_SIZE (DRM_IOCTL_GET_SAREA_CTX)},
  {"DRM_IOCTL_ADD_CTX", pretty_drm_ioctl_add_ctx,
   _IOC_DIR (DRM_IOCTL_ADD_CTX), _IOC_TYPE (DRM_IOCTL_ADD_CTX),
   _IOC_NR (DRM_IOCTL_ADD_CTX), _IOC_SIZE (DRM_IOCTL_ADD_CTX)},
  {"DRM_IOCTL_RM_CTX", pretty_drm_ioctl_rm_ctx, _IOC_DIR (DRM_IOCTL_RM_CTX),
   _IOC_TYPE (DRM_IOCTL_RM_CTX), _IOC_NR (DRM_IOCTL_RM_CTX),
   _IOC_SIZE (DRM_IOCTL_RM_CTX)},
  {"DRM_IOCTL_MOD_CTX", pretty_drm_ioctl_mod_ctx,
   _IOC_DIR (DRM_IOCTL_MOD_CTX), _IOC_TYPE (DRM_IOCTL_MOD_CTX),
   _IOC_NR (DRM_IOCTL_MOD_CTX), _IOC_SIZE (DRM_IOCTL_MOD_CTX)},
  {"DRM_IOCTL_GET_CTX", pretty_drm_ioctl_get_ctx,
   _IOC_DIR (DRM_IOCTL_GET_CTX), _IOC_TYPE (DRM_IOCTL_GET_CTX),
   _IOC_NR (DRM_IOCTL_GET_CTX), _IOC_SIZE (DRM_IOCTL_GET_CTX)},
  {"DRM_IOCTL_SWITCH_CTX", pretty_drm_ioctl_switch_ctx,
   _IOC_DIR (DRM_IOCTL_SWITCH_CTX), _IOC_TYPE (DRM_IOCTL_SWITCH_CTX),
   _IOC_NR (DRM_IOCTL_SWITCH_CTX), _IOC_SIZE (DRM_IOCTL_SWITCH_CTX)},
  {"DRM_IOCTL_NEW_CTX", pretty_drm_ioctl_new_ctx,
   _IOC_DIR (DRM_IOCTL_NEW_CTX), _IOC_TYPE (DRM_IOCTL_NEW_CTX),
   _IOC_NR (DRM_IOCTL_NEW_CTX), _IOC_SIZE (DRM_IOCTL_NEW_CTX)},
  {"DRM_IOCTL_RES_CTX", pretty_drm_ioctl_res_ctx,
   _IOC_DIR (DRM_IOCTL_RES_CTX), _IOC_TYPE (DRM_IOCTL_RES_CTX),
   _IOC_NR (DRM_IOCTL_RES_CTX), _IOC_SIZE (DRM_IOCTL_RES_CTX)},
  {"DRM_IOCTL_ADD_DRAW", pretty_drm_ioctl_add_draw,
   _IOC_DIR (DRM_IOCTL_ADD_DRAW), _IOC_TYPE (DRM_IOCTL_ADD_DRAW),
   _IOC_NR (DRM_IOCTL_ADD_DRAW), _IOC_SIZE (DRM_IOCTL_ADD_DRAW)},
  {"DRM_IOCTL_RM_DRAW", pretty_drm_ioctl_rm_draw,
   _IOC_DIR (DRM_IOCTL_RM_DRAW), _IOC_TYPE (DRM_IOCTL_RM_DRAW),
   _IOC_NR (DRM_IOCTL_RM_DRAW), _IOC_SIZE (DRM_IOCTL_RM_DRAW)},
  {"DRM_IOCTL_DMA", pretty_drm_ioctl_dma, _IOC_DIR (DRM_IOCTL_DMA),
   _IOC_TYPE (DRM_IOCTL_DMA), _IOC_NR (DRM_IOCTL_DMA),
   _IOC_SIZE (DRM_IOCTL_DMA)},
  {"DRM_IOCTL_LOCK", pretty_drm_ioctl_lock, _IOC_DIR (DRM_IOCTL_LOCK),
   _IOC_TYPE (DRM_IOCTL_LOCK), _IOC_NR (DRM_IOCTL_LOCK),
   _IOC_SIZE (DRM_IOCTL_LOCK)},
  {"DRM_IOCTL_UNLOCK", pretty_drm_ioctl_unlock, _IOC_DIR (DRM_IOCTL_UNLOCK),
   _IOC_TYPE (DRM_IOCTL_UNLOCK), _IOC_NR (DRM_IOCTL_UNLOCK),
   _IOC_SIZE (DRM_IOCTL_UNLOCK)},
  {"DRM_IOCTL_FINISH", pretty_drm_ioctl_finish, _IOC_DIR (DRM_IOCTL_FINISH),
   _IOC_TYPE (DRM_IOCTL_FINISH), _IOC_NR (DRM_IOCTL_FINISH),
   _IOC_SIZE (DRM_IOCTL_FINISH)},
  {"DRM_IOCTL_AGP_ACQUIRE", pretty_drm_ioctl_agp_acquire,
   _IOC_DIR (DRM_IOCTL_AGP_ACQUIRE), _IOC_TYPE (DRM_IOCTL_AGP_ACQUIRE),
   _IOC_NR (DRM_IOCTL_AGP_ACQUIRE), _IOC_SIZE (DRM_IOCTL_AGP_ACQUIRE)},
  {"DRM_IOCTL_AGP_RELEASE", pretty_drm_ioctl_agp_release,
   _IOC_DIR (DRM_IOCTL_AGP_RELEASE), _IOC_TYPE (DRM_IOCTL_AGP_RELEASE),
   _IOC_NR (DRM_IOCTL_AGP_RELEASE), _IOC_SIZE (DRM_IOCTL_AGP_RELEASE)},
  {"DRM_IOCTL_AGP_ENABLE", pretty_drm_ioctl_agp_enable,
   _IOC_DIR (DRM_IOCTL_AGP_ENABLE), _IOC_TYPE (DRM_IOCTL_AGP_ENABLE),
   _IOC_NR (DRM_IOCTL_AGP_ENABLE), _IOC_SIZE (DRM_IOCTL_AGP_ENABLE)},
  {"DRM_IOCTL_AGP_INFO", pretty_drm_ioctl_agp_info,
   _IOC_DIR (DRM_IOCTL_AGP_INFO), _IOC_TYPE (DRM_IOCTL_AGP_INFO),
   _IOC_NR (DRM_IOCTL_AGP_INFO), _IOC_SIZE (DRM_IOCTL_AGP_INFO)},
  {"DRM_IOCTL_AGP_ALLOC", pretty_drm_ioctl_agp_alloc,
   _IOC_DIR (DRM_IOCTL_AGP_ALLOC), _IOC_TYPE (DRM_IOCTL_AGP_ALLOC),
   _IOC_NR (DRM_IOCTL_AGP_ALLOC), _IOC_SIZE (DRM_IOCTL_AGP_ALLOC)},
  {"DRM_IOCTL_AGP_FREE", pretty_drm_ioctl_agp_free,
   _IOC_DIR (DRM_IOCTL_AGP_FREE), _IOC_TYPE (DRM_IOCTL_AGP_FREE),
   _IOC_NR (DRM_IOCTL_AGP_FREE), _IOC_SIZE (DRM_IOCTL_AGP_FREE)},
  {"DRM_IOCTL_AGP_BIND", pretty_drm_ioctl_agp_bind,
   _IOC_DIR (DRM_IOCTL_AGP_BIND), _IOC_TYPE (DRM_IOCTL_AGP_BIND),
   _IOC_NR (DRM_IOCTL_AGP_BIND), _IOC_SIZE (DRM_IOCTL_AGP_BIND)},
  {"DRM_IOCTL_AGP_UNBIND", pretty_drm_ioctl_agp_unbind,
   _IOC_DIR (DRM_IOCTL_AGP_UNBIND), _IOC_TYPE (DRM_IOCTL_AGP_UNBIND),
   _IOC_NR (DRM_IOCTL_AGP_UNBIND), _IOC_SIZE (DRM_IOCTL_AGP_UNBIND)},
  {"DRM_IOCTL_SG_ALLOC", pretty_drm_ioctl_sg_alloc,
   _IOC_DIR (DRM_IOCTL_SG_ALLOC), _IOC_TYPE (DRM_IOCTL_SG_ALLOC),
   _IOC_NR (DRM_IOCTL_SG_ALLOC), _IOC_SIZE (DRM_IOCTL_SG_ALLOC)},
  {"DRM_IOCTL_SG_FREE", pretty_drm_ioctl_sg_free,
   _IOC_DIR (DRM_IOCTL_SG_FREE), _IOC_TYPE (DRM_IOCTL_SG_FREE),
   _IOC_NR (DRM_IOCTL_SG_FREE), _IOC_SIZE (DRM_IOCTL_SG_FREE)},
  {"DRM_IOCTL_WAIT_VBLANK", pretty_drm_ioctl_wait_vblank,
   _IOC_DIR (DRM_IOCTL_WAIT_VBLANK), _IOC_TYPE (DRM_IOCTL_WAIT_VBLANK),
   _IOC_NR (DRM_IOCTL_WAIT_VBLANK), _IOC_SIZE (DRM_IOCTL_WAIT_VBLANK)},
  {"DRM_IOCTL_FENCE", pretty_drm_ioctl_fence, _IOC_DIR (DRM_IOCTL_FENCE),
   _IOC_TYPE (DRM_IOCTL_FENCE), _IOC_NR (DRM_IOCTL_FENCE),
   _IOC_SIZE (DRM_IOCTL_FENCE)},
  {"DRM_IOCTL_BUFOBJ", pretty_drm_ioctl_bufobj, _IOC_DIR (DRM_IOCTL_BUFOBJ),
   _IOC_TYPE (DRM_IOCTL_BUFOBJ), _IOC_NR (DRM_IOCTL_BUFOBJ),
   _IOC_SIZE (DRM_IOCTL_BUFOBJ)},
  {"DRM_IOCTL_MM_INIT", pretty_drm_ioctl_mm_init,
   _IOC_DIR (DRM_IOCTL_MM_INIT), _IOC_TYPE (DRM_IOCTL_MM_INIT),
   _IOC_NR (DRM_IOCTL_MM_INIT), _IOC_SIZE (DRM_IOCTL_MM_INIT)},
  {"DRM_IOCTL_UPDATE_DRAW", pretty_drm_ioctl_update_draw,
   _IOC_DIR (DRM_IOCTL_UPDATE_DRAW), _IOC_TYPE (DRM_IOCTL_UPDATE_DRAW),
   _IOC_NR (DRM_IOCTL_UPDATE_DRAW), _IOC_SIZE (DRM_IOCTL_UPDATE_DRAW)},
  {NULL, 0, 0, 0},
};
