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

#define MAP_BUTTONS         28
#define MAP_SHIFT_START_POS 18 /* Shift buttons start here */

typedef struct psp_ctrl_map
{
  unsigned int button_map[MAP_BUTTONS];
} psp_ctrl_map_t;

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

#define SPC 0x1000000
#define KBD 0x2000000
#define JOY 0x4000000

#define SPC_MENU 0x00001
#define SPC_KYBD 0x00002

#define CK(x,y,z) (KBD|(((x&0xff) << 8)|(y&0xff)))
#define CKROW(ck) ((ck&0xff00)>>8)
#define CKCOL(ck) (ck&0xff)
#define CODE_MASK(ck) (ck&0xffffff)

extern psp_options_t psp_options;
extern psp_ctrl_map_t current_map;

extern int ui_vblank_sync_enabled();
extern void psp_display_menu();

extern void ui_exit(void);
extern void ui_display_speed(float percent, float framerate, int warp_flag);
extern void ui_display_paused(int flag);
extern void ui_dispatch_next_event(void);
extern void ui_dispatch_events(void);
extern void ui_error_string(const char *text);

#endif

