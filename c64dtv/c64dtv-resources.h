/*
 * c64dtv-resources.h
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

#ifndef VICE_C64DTV_RESOURCES_H
#define VICE_C64DTV_RESOURCES_H

#define HUMMER_USERPORT_NONE  0
#define HUMMER_USERPORT_ADC   1
#define HUMMER_USERPORT_JOY   2

extern int c64dtv_resources_init(void);
extern void c64dtv_resources_shutdown(void);

extern int emu_id_enabled;
extern char *kernal_revision;

extern int c64dtv_hummer_userport_device;
extern unsigned int c64dtv_hummer_userport_joy_port;

#endif