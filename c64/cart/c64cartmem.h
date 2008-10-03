/*
 * c64cartmem.h -- C64 cartridge emulation, memory handling.
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

#ifndef _C64CARTMEM_H
#define _C64CARTMEM_H

#include "types.h"

#define CMODE_READ  0
#define CMODE_WRITE 1

extern void cartridge_config_changed(BYTE mode_phi1, BYTE mode_phi2,
                                     unsigned int wflag);

void cartridge_romhbank_set(unsigned int bank);
void cartridge_romlbank_set(unsigned int bank);

extern BYTE export_ram0[];

#endif

