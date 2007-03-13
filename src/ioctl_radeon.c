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
#include <drm/radeon_drm.h>

#include "ioctl.h"

static void
pretty_drm_ioctl_radeon_cp_init (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_cp_start (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_cp_stop (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_cp_reset (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_cp_idle (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_reset (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_fullscreen (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_swap (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_clear (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_vertex (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_indices (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_stipple (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_indirect (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_texture (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_vertex2 (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_cmdbuf (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_getparam (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_flip (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_alloc (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_free (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_init_heap (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_irq_emit (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_irq_wait (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_cp_resume (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_setparam (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_surf_alloc (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

static void
pretty_drm_ioctl_radeon_surf_free (struct ioctl_t *ioctl, int *ioctl_ptr)
{
}

/* commit 1b3a6d47751018c75e4333ee3ab8ba21dbd55bdd */
struct ioctl_t radeon_ioctls[] = {
  {"DRM_IOCTL_RADEON_CP_INIT", pretty_drm_ioctl_radeon_cp_init,
   _IOC_DIR (DRM_IOCTL_RADEON_CP_INIT), _IOC_TYPE (DRM_IOCTL_RADEON_CP_INIT),
   _IOC_NR (DRM_IOCTL_RADEON_CP_INIT), _IOC_SIZE (DRM_IOCTL_RADEON_CP_INIT)},
  {"DRM_IOCTL_RADEON_CP_START", pretty_drm_ioctl_radeon_cp_start,
   _IOC_DIR (DRM_IOCTL_RADEON_CP_START),
   _IOC_TYPE (DRM_IOCTL_RADEON_CP_START), _IOC_NR (DRM_IOCTL_RADEON_CP_START),
   _IOC_SIZE (DRM_IOCTL_RADEON_CP_START)},
  {"DRM_IOCTL_RADEON_CP_STOP", pretty_drm_ioctl_radeon_cp_stop,
   _IOC_DIR (DRM_IOCTL_RADEON_CP_STOP), _IOC_TYPE (DRM_IOCTL_RADEON_CP_STOP),
   _IOC_NR (DRM_IOCTL_RADEON_CP_STOP), _IOC_SIZE (DRM_IOCTL_RADEON_CP_STOP)},
  {"DRM_IOCTL_RADEON_CP_RESET", pretty_drm_ioctl_radeon_cp_reset,
   _IOC_DIR (DRM_IOCTL_RADEON_CP_RESET),
   _IOC_TYPE (DRM_IOCTL_RADEON_CP_RESET), _IOC_NR (DRM_IOCTL_RADEON_CP_RESET),
   _IOC_SIZE (DRM_IOCTL_RADEON_CP_RESET)},
  {"DRM_IOCTL_RADEON_CP_IDLE", pretty_drm_ioctl_radeon_cp_idle,
   _IOC_DIR (DRM_IOCTL_RADEON_CP_IDLE), _IOC_TYPE (DRM_IOCTL_RADEON_CP_IDLE),
   _IOC_NR (DRM_IOCTL_RADEON_CP_IDLE), _IOC_SIZE (DRM_IOCTL_RADEON_CP_IDLE)},
  {"DRM_IOCTL_RADEON_RESET", pretty_drm_ioctl_radeon_reset,
   _IOC_DIR (DRM_IOCTL_RADEON_RESET), _IOC_TYPE (DRM_IOCTL_RADEON_RESET),
   _IOC_NR (DRM_IOCTL_RADEON_RESET), _IOC_SIZE (DRM_IOCTL_RADEON_RESET)},
  {"DRM_IOCTL_RADEON_FULLSCREEN", pretty_drm_ioctl_radeon_fullscreen,
   _IOC_DIR (DRM_IOCTL_RADEON_FULLSCREEN),
   _IOC_TYPE (DRM_IOCTL_RADEON_FULLSCREEN),
   _IOC_NR (DRM_IOCTL_RADEON_FULLSCREEN),
   _IOC_SIZE (DRM_IOCTL_RADEON_FULLSCREEN)},
  {"DRM_IOCTL_RADEON_SWAP", pretty_drm_ioctl_radeon_swap,
   _IOC_DIR (DRM_IOCTL_RADEON_SWAP), _IOC_TYPE (DRM_IOCTL_RADEON_SWAP),
   _IOC_NR (DRM_IOCTL_RADEON_SWAP), _IOC_SIZE (DRM_IOCTL_RADEON_SWAP)},
  {"DRM_IOCTL_RADEON_CLEAR", pretty_drm_ioctl_radeon_clear,
   _IOC_DIR (DRM_IOCTL_RADEON_CLEAR), _IOC_TYPE (DRM_IOCTL_RADEON_CLEAR),
   _IOC_NR (DRM_IOCTL_RADEON_CLEAR), _IOC_SIZE (DRM_IOCTL_RADEON_CLEAR)},
  {"DRM_IOCTL_RADEON_VERTEX", pretty_drm_ioctl_radeon_vertex,
   _IOC_DIR (DRM_IOCTL_RADEON_VERTEX), _IOC_TYPE (DRM_IOCTL_RADEON_VERTEX),
   _IOC_NR (DRM_IOCTL_RADEON_VERTEX), _IOC_SIZE (DRM_IOCTL_RADEON_VERTEX)},
  {"DRM_IOCTL_RADEON_INDICES", pretty_drm_ioctl_radeon_indices,
   _IOC_DIR (DRM_IOCTL_RADEON_INDICES), _IOC_TYPE (DRM_IOCTL_RADEON_INDICES),
   _IOC_NR (DRM_IOCTL_RADEON_INDICES), _IOC_SIZE (DRM_IOCTL_RADEON_INDICES)},
  {"DRM_IOCTL_RADEON_STIPPLE", pretty_drm_ioctl_radeon_stipple,
   _IOC_DIR (DRM_IOCTL_RADEON_STIPPLE), _IOC_TYPE (DRM_IOCTL_RADEON_STIPPLE),
   _IOC_NR (DRM_IOCTL_RADEON_STIPPLE), _IOC_SIZE (DRM_IOCTL_RADEON_STIPPLE)},
  {"DRM_IOCTL_RADEON_INDIRECT", pretty_drm_ioctl_radeon_indirect,
   _IOC_DIR (DRM_IOCTL_RADEON_INDIRECT),
   _IOC_TYPE (DRM_IOCTL_RADEON_INDIRECT), _IOC_NR (DRM_IOCTL_RADEON_INDIRECT),
   _IOC_SIZE (DRM_IOCTL_RADEON_INDIRECT)},
  {"DRM_IOCTL_RADEON_TEXTURE", pretty_drm_ioctl_radeon_texture,
   _IOC_DIR (DRM_IOCTL_RADEON_TEXTURE), _IOC_TYPE (DRM_IOCTL_RADEON_TEXTURE),
   _IOC_NR (DRM_IOCTL_RADEON_TEXTURE), _IOC_SIZE (DRM_IOCTL_RADEON_TEXTURE)},
  {"DRM_IOCTL_RADEON_VERTEX2", pretty_drm_ioctl_radeon_vertex2,
   _IOC_DIR (DRM_IOCTL_RADEON_VERTEX2), _IOC_TYPE (DRM_IOCTL_RADEON_VERTEX2),
   _IOC_NR (DRM_IOCTL_RADEON_VERTEX2), _IOC_SIZE (DRM_IOCTL_RADEON_VERTEX2)},
  {"DRM_IOCTL_RADEON_CMDBUF", pretty_drm_ioctl_radeon_cmdbuf,
   _IOC_DIR (DRM_IOCTL_RADEON_CMDBUF), _IOC_TYPE (DRM_IOCTL_RADEON_CMDBUF),
   _IOC_NR (DRM_IOCTL_RADEON_CMDBUF), _IOC_SIZE (DRM_IOCTL_RADEON_CMDBUF)},
  {"DRM_IOCTL_RADEON_GETPARAM", pretty_drm_ioctl_radeon_getparam,
   _IOC_DIR (DRM_IOCTL_RADEON_GETPARAM),
   _IOC_TYPE (DRM_IOCTL_RADEON_GETPARAM), _IOC_NR (DRM_IOCTL_RADEON_GETPARAM),
   _IOC_SIZE (DRM_IOCTL_RADEON_GETPARAM)},
  {"DRM_IOCTL_RADEON_FLIP", pretty_drm_ioctl_radeon_flip,
   _IOC_DIR (DRM_IOCTL_RADEON_FLIP), _IOC_TYPE (DRM_IOCTL_RADEON_FLIP),
   _IOC_NR (DRM_IOCTL_RADEON_FLIP), _IOC_SIZE (DRM_IOCTL_RADEON_FLIP)},
  {"DRM_IOCTL_RADEON_ALLOC", pretty_drm_ioctl_radeon_alloc,
   _IOC_DIR (DRM_IOCTL_RADEON_ALLOC), _IOC_TYPE (DRM_IOCTL_RADEON_ALLOC),
   _IOC_NR (DRM_IOCTL_RADEON_ALLOC), _IOC_SIZE (DRM_IOCTL_RADEON_ALLOC)},
  {"DRM_IOCTL_RADEON_FREE", pretty_drm_ioctl_radeon_free,
   _IOC_DIR (DRM_IOCTL_RADEON_FREE), _IOC_TYPE (DRM_IOCTL_RADEON_FREE),
   _IOC_NR (DRM_IOCTL_RADEON_FREE), _IOC_SIZE (DRM_IOCTL_RADEON_FREE)},
  {"DRM_IOCTL_RADEON_INIT_HEAP", pretty_drm_ioctl_radeon_init_heap,
   _IOC_DIR (DRM_IOCTL_RADEON_INIT_HEAP),
   _IOC_TYPE (DRM_IOCTL_RADEON_INIT_HEAP),
   _IOC_NR (DRM_IOCTL_RADEON_INIT_HEAP),
   _IOC_SIZE (DRM_IOCTL_RADEON_INIT_HEAP)},
  {"DRM_IOCTL_RADEON_IRQ_EMIT", pretty_drm_ioctl_radeon_irq_emit,
   _IOC_DIR (DRM_IOCTL_RADEON_IRQ_EMIT),
   _IOC_TYPE (DRM_IOCTL_RADEON_IRQ_EMIT), _IOC_NR (DRM_IOCTL_RADEON_IRQ_EMIT),
   _IOC_SIZE (DRM_IOCTL_RADEON_IRQ_EMIT)},
  {"DRM_IOCTL_RADEON_IRQ_WAIT", pretty_drm_ioctl_radeon_irq_wait,
   _IOC_DIR (DRM_IOCTL_RADEON_IRQ_WAIT),
   _IOC_TYPE (DRM_IOCTL_RADEON_IRQ_WAIT), _IOC_NR (DRM_IOCTL_RADEON_IRQ_WAIT),
   _IOC_SIZE (DRM_IOCTL_RADEON_IRQ_WAIT)},
  {"DRM_IOCTL_RADEON_CP_RESUME", pretty_drm_ioctl_radeon_cp_resume,
   _IOC_DIR (DRM_IOCTL_RADEON_CP_RESUME),
   _IOC_TYPE (DRM_IOCTL_RADEON_CP_RESUME),
   _IOC_NR (DRM_IOCTL_RADEON_CP_RESUME),
   _IOC_SIZE (DRM_IOCTL_RADEON_CP_RESUME)},
  {"DRM_IOCTL_RADEON_SETPARAM", pretty_drm_ioctl_radeon_setparam,
   _IOC_DIR (DRM_IOCTL_RADEON_SETPARAM),
   _IOC_TYPE (DRM_IOCTL_RADEON_SETPARAM), _IOC_NR (DRM_IOCTL_RADEON_SETPARAM),
   _IOC_SIZE (DRM_IOCTL_RADEON_SETPARAM)},
  {"DRM_IOCTL_RADEON_SURF_ALLOC", pretty_drm_ioctl_radeon_surf_alloc,
   _IOC_DIR (DRM_IOCTL_RADEON_SURF_ALLOC),
   _IOC_TYPE (DRM_IOCTL_RADEON_SURF_ALLOC),
   _IOC_NR (DRM_IOCTL_RADEON_SURF_ALLOC),
   _IOC_SIZE (DRM_IOCTL_RADEON_SURF_ALLOC)},
  {"DRM_IOCTL_RADEON_SURF_FREE", pretty_drm_ioctl_radeon_surf_free,
   _IOC_DIR (DRM_IOCTL_RADEON_SURF_FREE),
   _IOC_TYPE (DRM_IOCTL_RADEON_SURF_FREE),
   _IOC_NR (DRM_IOCTL_RADEON_SURF_FREE),
   _IOC_SIZE (DRM_IOCTL_RADEON_SURF_FREE)},
  {NULL, NULL, 0, 0, 0, 0},
};
