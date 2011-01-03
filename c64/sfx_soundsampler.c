/*
 * digimax.c - Digimax DAC cartridge emulation.
 *
 * Written by
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

#include "vice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c64io.h"
#include "cmdline.h"
#include "lib.h"
#include "maincpu.h"
#include "resources.h"
#include "sfx_soundsampler.h"
#include "sid.h"
#include "sound.h"
#include "uiapi.h"
#include "translate.h"

/* Flag: Do we enable the SFX soundsampler cartridge?  */
int sfx_soundsampler_enabled;

static int set_sfx_soundsampler_enabled(int val, void *param)
{
    if (sid_sound_machine_cycle_based() == 1 && val) {
        ui_error(translate_text(IDGS_SFX_SS_NOT_WITH_RESID));
        return -1;
    }
    sfx_soundsampler_enabled = val;
    return 0;
}

static const resource_int_t resources_int[] = {
    { "SFXSoundSampler", 0, RES_EVENT_STRICT, (resource_value_t)0,
      &sfx_soundsampler_enabled, set_sfx_soundsampler_enabled, NULL },
    { NULL }
};

int sfx_soundsampler_resources_init(void)
{
    return resources_register_int(resources_int);
}

static const cmdline_option_t cmdline_options[] =
{
    { "-sfxss", SET_RESOURCE, 0,
      NULL, NULL, "SFXSoundSampler", (resource_value_t)1,
      USE_PARAM_STRING, USE_DESCRIPTION_ID,
      IDCLS_UNUSED, IDCLS_ENABLE_SFX_SS,
      NULL, NULL },
    { "+sfxss", SET_RESOURCE, 0,
      NULL, NULL, "SFXSoundSampler", (resource_value_t)0,
      USE_PARAM_STRING, USE_DESCRIPTION_ID,
      IDCLS_UNUSED, IDCLS_DISABLE_SFX_SS,
      NULL, NULL },
    { NULL }
};

int sfx_soundsampler_cmdline_options_init(void)
{
    return cmdline_register_options(cmdline_options);
}

/* ---------------------------------------------------------------------*/

static BYTE sfx_soundsampler_sound_data;

struct sfx_soundsampler_sound_s
{
    BYTE voice0;
};

static struct sfx_soundsampler_sound_s snd;

int sfx_soundsampler_sound_machine_calculate_samples(sound_t *psid, SWORD *pbuf, int nr, int interleave, int *delta_t)
{
    int i;

    if (sid_sound_machine_cycle_based() == 0 && sfx_soundsampler_enabled) {
        for (i = 0; i < nr; i++) {
            pbuf[i * interleave] = sound_audio_mix(pbuf[i * interleave], snd.voice0 << 8);
        }
    }
    return 0;
}

int sfx_soundsampler_sound_machine_init(sound_t *psid, int speed, int cycles_per_sec)
{
    snd.voice0 = 0;

    return 1;
}

void sfx_soundsampler_sound_machine_store(sound_t *psid, WORD addr, BYTE val)
{
    snd.voice0 = val;
}

BYTE sfx_soundsampler_sound_machine_read(sound_t *psid, WORD addr)
{
    return sfx_soundsampler_sound_data;
}

void sfx_soundsampler_sound_reset(void)
{
    snd.voice0 = 0;
    sfx_soundsampler_sound_data = 0;
}

/* ---------------------------------------------------------------------*/

void REGPARM2 sfx_soundsampler_sound_store(WORD addr, BYTE value)
{
    sfx_soundsampler_sound_data = value;
    sound_store((WORD)0x40, value, 0);
}
