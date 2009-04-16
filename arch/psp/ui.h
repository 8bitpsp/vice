/*
 * ui.h - PSP user interface.
 *
 */

#ifndef _UI_PSP_H
#define _UI_PSP_H

#include "vice.h"

#include "types.h"
#include "uiapi.h"

#define DISPLAY_MODE_UNSCALED    0
#define DISPLAY_MODE_FIT_HEIGHT  1
#define DISPLAY_MODE_FILL_SCREEN 2

typedef struct psp_options
{
  int display_mode;
  int show_fps;
  int control_mode;
  int animate_menu;
  int toggle_vk;
  int clock_freq;
  int autoload_slot;
} psp_options_t;

extern psp_options_t psp_options;

extern int ui_vblank_sync_enabled();
extern void psp_display_menu();

extern void ui_exit(void);
extern void ui_display_speed(float percent, float framerate, int warp_flag);
extern void ui_display_paused(int flag);
extern void ui_dispatch_next_event(void);
extern void ui_dispatch_events(void);
extern void ui_error_string(const char *text);

#endif

