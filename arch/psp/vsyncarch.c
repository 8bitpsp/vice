/*
 * vsyncarch.c - End-of-frame handling for Unix
 *
 * Written by
 *  Dag Lem <resid@nimrod.no>
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

#include "kbdbuf.h"
#include "ui.h"
#include "vsyncapi.h"
#include "videoarch.h"

#include <time.h>
#include <psptypes.h>
#include <psprtc.h>
#include <pspkernel.h>

/* hook to ui event dispatcher */
static void_hook_t ui_dispatch_hook;

/* ------------------------------------------------------------------------- */

/* Number of timer units per second. */
signed long vsyncarch_frequency(void)
{
    /* Microseconds resolution. */
    return sceRtcGetTickResolution();
}

/* Get time in timer units. */
unsigned long vsyncarch_gettime(void)
{
    u64 ticks;
    sceRtcGetCurrentTick(&ticks);

    return ticks;
}

void vsyncarch_init(void)
{
    (void)vsync_set_event_dispatcher(ui_dispatch_events);
}

/* Display speed (percentage) and frame rate (frames per second). */
void vsyncarch_display_speed(double speed, double frame_rate, int warp_enabled)
{
    ui_display_speed((float)speed, (float)frame_rate, warp_enabled);
}

/* Sleep a number of timer units. */
void vsyncarch_sleep(signed long delay)
{
  sceKernelDelayThread(delay);
}

void vsyncarch_presync(void)
{
    psp_input_poll();
    kbdbuf_flush();

    /* Refresh screen */
    psp_refresh_screen();
}

void_hook_t vsync_set_event_dispatcher(void_hook_t hook)
{
    void_hook_t t = ui_dispatch_hook;
    ui_dispatch_hook = hook;
    return t;
}

void vsyncarch_postsync(void)
{
    /* Dispatch all the pending UI events.  */
    ui_dispatch_events();
}
