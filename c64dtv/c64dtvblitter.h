/*
 * c64dtvblitter.h - C64DTV blitter and DMA controller
 *
 * Written by
 *  M.Kiesel <mayne@users.sourceforge.net>
 *  Daniel Kahlin <daniel@kahlin.net>
 * Based on code by
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#ifndef _C64DTVBLITTER_H
#define _C64DTVBLITTER_H

#include "types.h"

#define CYCLE_EXACT_BLITTER

extern int blitter_on_irq;

extern int c64dtvblitter_resources_init(void);
extern void c64dtvblitter_resources_shutdown(void);
extern int c64dtvblitter_cmdline_options_init(void);
extern void c64dtvblitter_init(void);
extern void c64dtvblitter_reset(void);
extern void c64dtvblitter_shutdown(void);
extern void c64dtv_blitter_irq_init(void);

extern BYTE REGPARM1 c64dtv_dmablit_read(WORD addr);
extern void REGPARM2 c64dtv_dmablit_store(WORD addr, BYTE value);

extern int c64dtvblitter_perform_blitter(void);
extern void c64dtvblitter_trigger_blitter(void);

struct snapshot_s;

extern int c64dtvblitter_snapshot_write_module(struct snapshot_s *s);
extern int c64dtvblitter_snapshot_read_module(struct snapshot_s *s);

#endif
