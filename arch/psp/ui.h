/*
 * ui.h - PSP user interface.
 *
 */

#ifndef _UI_PSP_H
#define _UI_PSP_H

#include "vice.h"

#include "types.h"
#include "uiapi.h"

extern int ui_vblank_sync_enabled();

extern void ui_exit(void);
extern void ui_display_speed(float percent, float framerate, int warp_flag);
extern void ui_display_paused(int flag);
extern void ui_dispatch_next_event(void);
extern void ui_dispatch_events(void);
extern void ui_error_string(const char *text);

#endif

