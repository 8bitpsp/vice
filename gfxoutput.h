/*
 * gfxoutput.h - Graphics output driver.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
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

#ifndef VICE_GFXOUTPUT_H
#define VICE_GFXOUTPUT_H

#include "types.h"

struct screenshot_s;

typedef struct gfxoutputdrv_codec_s {
    int id;
    const char *name;
} gfxoutputdrv_codec_t;

typedef struct gfxoutputdrv_format_s {
    char *name;
    gfxoutputdrv_codec_t *audio_codecs;
    gfxoutputdrv_codec_t *video_codecs;
} gfxoutputdrv_format_t;

typedef struct gfxoutputdrv_s {
    const char *name;
    const char *displayname;
    const char *default_extension;
    gfxoutputdrv_format_t *formatlist;
    int (*open)(struct screenshot_s *, const char *);
    int (*close)(struct screenshot_s *);
    int (*write)(struct screenshot_s *);
    int (*save)(struct screenshot_s *, const char *);
    int (*record)(struct screenshot_s *);
    void (*shutdown)(void);
    int (*resources_init)(void);
    int (*cmdline_options_init)(void);
#ifdef FEATURE_CPUMEMHISTORY
    int (*savememmap)(const char *, int, int, BYTE *, BYTE *);
#endif
} gfxoutputdrv_t;

/* Functions called by external emulator code.  */
extern int gfxoutput_resources_init(void);
extern int gfxoutput_cmdline_options_init(void);
extern int gfxoutput_early_init(void);
extern int gfxoutput_init(void);
extern void gfxoutput_shutdown(void);
extern int gfxoutput_num_drivers(void);
extern gfxoutputdrv_t *gfxoutput_drivers_iter_init(void);
extern gfxoutputdrv_t *gfxoutput_drivers_iter_next(void);
extern gfxoutputdrv_t *gfxoutput_get_driver(const char *drvname);

/* Functions called by graphic output driver modules.  */
extern int gfxoutput_register(gfxoutputdrv_t *drv);

/* GIF driver functions. */
extern void gfxoutput_init_gif(void);

#endif

