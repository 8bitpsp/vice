/*
 * ui.c - Windows user interface.
 *
 * Written by
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Tibor Biczo <crown@mail.matav.hu>
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

#include "vice.h"
#include "ui.h"
#include "uiapi.h"
#include "cmdline.h"
#include "resources.h"
#include "keyboard.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const cmdline_option_t cmdline_options[] = {
    { NULL },
};

static const resource_int_t resources_int[] = {
    { NULL }
};

/* Report an error to the user.  */
void ui_error(const char *format,...)
{
#ifdef PSP_DEBUG
  FILE *error_log = fopen("errorlog.txt", "a");

  va_list ap;
  va_start (ap, format);
  vfprintf (error_log, format, ap);
  va_end (ap);

  fclose(error_log);
#endif
}

void ui_display_joyport(BYTE *joyport)
{
  /* needed */
}

void ui_display_event_time(unsigned int current, unsigned int total)
{
  /* needed */
}

void ui_display_volume(int vol)
{
}

/* Display a mesage without interrupting emulation */
void ui_display_statustext(const char *text, int fade_out)
{
}

void ui_display_paused(int flag)
{
}

void ui_display_drive_led(int drive_number, unsigned int pwm1,
                                 unsigned int led_pwm2)
{
}

void ui_display_drive_current_image(unsigned int drive_number,
                                           const char *image)
{
}

void ui_display_tape_motor_status(int motor)
{
}

void ui_display_tape_control_status(int control)
{
}

void ui_display_tape_counter(int counter)
{
}

void ui_display_tape_current_image(const char *image)
{
}

void ui_display_playback(int playback_status, char *version)
{
}

void ui_display_recording(int recording_status)
{
}

void ui_display_drive_track(unsigned int drive_number,
                                   unsigned int drive_base,
                                   unsigned int half_track_number)
{
}

void ui_enable_drive_status(ui_drive_enable_t state,
                                   int *drive_led_color)
{
}

/* tape-related ui, dummies so far */
void ui_set_tape_status(int tape_status)
{
}

/* Update all the menus according to the current settings.  */
void ui_update_menus(void)
{
}

/* Show a CPU JAM dialog.  */
ui_jam_action_t ui_jam_dialog(const char *format, ...)
{
  return UI_JAM_NONE;
}

int ui_extend_image_dialog()
{
  return 0;
}

void ui_dispatch_events()
{
}

signed long kbd_arch_keyname_to_keynum(char *keyname) {
	return (signed long)atoi(keyname);
}

const char *kbd_arch_keynum_to_keyname(signed long keynum) {
	static char keyname[20];

	memset(keyname, 0, 20);
	sprintf(keyname, "%li", keynum);
	return keyname;
}

void kbd_arch_init()
{
  keyboard_clear_keymatrix();
}

int ui_resources_init(void)
{
  return resources_register_int(resources_int);
}

void ui_resources_shutdown()
{
}

int ui_cmdline_options_init(void)
{
  return cmdline_register_options(cmdline_options);
}

int ui_init(int *argc, char **argv)
{
  return 0;
}

int ui_init_finish()
{
  return 0;
}

int ui_init_finalize()
{
  return 0;
}

void ui_shutdown()
{
}
