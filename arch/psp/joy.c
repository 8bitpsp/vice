/*
 * joy.c - Linux/BSD joystick support.
 *
 * Written by
 *  Bernhard Kuhn <kuhn@eikon.e-technik.tu-muenchen.de>
 *  Ulmer Lionel <ulmer@poly.polytechnique.fr>
 *
 * Patches by
 *  Daniel Sladic <sladic@eecg.toronto.edu>
 *
 * NetBSD support by
 *  Krister Walfridsson <cato@df.lth.se>
 *
 * 1.1.xxx Linux API by
 *   Luca Montecchiani  <m.luca@usa.net> (http://i.am/m.luca)
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
#include "joy.h"

/* (Used by `kbd.c').  */
int joystick_port_map[2];

int joystick_arch_init_resources(void)
{
}

/**********************************************************
 * Generic high level joy routine                         *
 **********************************************************/
int joy_arch_init(void)
{
    return 0;
}

int joystick_close()
{
}

void kbd_initialize_numpad_joykeys(int* joykeys)
{
}

int joystick_init_cmdline_options()
{
}
