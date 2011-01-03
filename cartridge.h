/*
 * cartridge.h - Cartridge emulation.
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

#ifndef VICE_CARTRIDGE_H
#define VICE_CARTRIDGE_H

#include "types.h"

extern void cartridge_init(void);
extern void cartridge_reset(void);
extern int cartridge_resources_init(void);
extern void cartridge_resources_shutdown(void);
extern int cartridge_cmdline_options_init(void);

extern int cartridge_attach_image(int type, const char *filename);
extern void cartridge_detach_image(void);
extern void cartridge_set_default(void);
extern void cartridge_trigger_freeze(void);
extern void cartridge_trigger_freeze_nmi_only(void);
extern const char *cartridge_get_file_name(WORD addr);

extern int cartridge_save_image(const char *filename);

extern void cartridge_attach(int type, BYTE *rawcart);
extern void cartridge_detach(int type);
extern void cartridge_freeze(int type);

/* Known cartridge types.  */
#define CARTRIDGE_ACTION_REPLAY3    -29
#define CARTRIDGE_IEEE488           -11
#define CARTRIDGE_IDE64             -7
#define CARTRIDGE_ULTIMAX           -6
#define CARTRIDGE_RETRO_REPLAY      -5
#define CARTRIDGE_SUPER_SNAPSHOT    -4
#define CARTRIDGE_GENERIC_8KB       -3
#define CARTRIDGE_GENERIC_16KB      -2
#define CARTRIDGE_NONE              -1
#define CARTRIDGE_CRT                0
#define CARTRIDGE_ACTION_REPLAY      1
#define CARTRIDGE_KCS_POWER          2
#define CARTRIDGE_FINAL_III          3
#define CARTRIDGE_SIMONS_BASIC       4
#define CARTRIDGE_OCEAN              5
#define CARTRIDGE_EXPERT             6
#define CARTRIDGE_FUNPLAY            7
#define CARTRIDGE_SUPER_GAMES        8
#define CARTRIDGE_ATOMIC_POWER       9
#define CARTRIDGE_EPYX_FASTLOAD      10
#define CARTRIDGE_WESTERMANN         11
#define CARTRIDGE_REX                12
#define CARTRIDGE_FINAL_I            13
#define CARTRIDGE_MAGIC_FORMEL       14
#define CARTRIDGE_GS                 15
#define CARTRIDGE_WARPSPEED          16
#define CARTRIDGE_DINAMIC            17
#define CARTRIDGE_ZAXXON             18
#define CARTRIDGE_MAGIC_DESK         19
#define CARTRIDGE_SUPER_SNAPSHOT_V5  20
#define CARTRIDGE_COMAL80            21
#define CARTRIDGE_STRUCTURED_BASIC   22
#define CARTRIDGE_ROSS               23
#define CARTRIDGE_DELA_EP64          24
#define CARTRIDGE_DELA_EP7x8         25
#define CARTRIDGE_DELA_EP256         26
#define CARTRIDGE_REX_EP256          27
#define CARTRIDGE_MIKRO_ASSEMBLER    28
/* 29 is reserved for the real fc1,
   which would cause the current fc1
   to become fc2 */
#define CARTRIDGE_ACTION_REPLAY4     30
#define CARTRIDGE_STARDOS            31
#define CARTRIDGE_EASYFLASH          32

/* Expert cartridge has three modes: */
#define CARTRIDGE_MODE_OFF                      0
#define CARTRIDGE_MODE_PRG                      1
#define CARTRIDGE_MODE_ON                       2

/* 
 * VIC20 cartridge system
 */
/* #define CARTRIDGE_NONE               -1 */
#define CARTRIDGE_VIC20_GENERIC      1
#define CARTRIDGE_VIC20_MEGACART     2
#define CARTRIDGE_VIC20_FINAL_EXPANSION 3

/* 
 * VIC20 Generic cartridges
 *
 * The cartridge types below are only used during attach requests.
 * They will always be converted to CARTRIDGE_VIC20_GENERIC when
 * attached.  This also means they can be remapped at will.
 *
 * VIC20: &1 -> 0=4k, 1=8k; &16 -> 0= < 16k, 1= 16k 2nd half at $a000
 * (this logic is not used AFAIK /tlr)
 */
#define CARTRIDGE_VIC20_DETECT       0x8000
#define CARTRIDGE_VIC20_4KB_2000     0x8002
#define CARTRIDGE_VIC20_8KB_2000     0x8003
#define CARTRIDGE_VIC20_4KB_6000     0x8004
#define CARTRIDGE_VIC20_8KB_6000     0x8005
#define CARTRIDGE_VIC20_4KB_A000     0x8006
#define CARTRIDGE_VIC20_8KB_A000     0x8007
#define CARTRIDGE_VIC20_4KB_B000     0x8008
#define CARTRIDGE_VIC20_8KB_4000     0x8009
#define CARTRIDGE_VIC20_4KB_4000     0x800a

#define CARTRIDGE_VIC20_16KB_2000    0x8013
#define CARTRIDGE_VIC20_16KB_4000    0x8019
#define CARTRIDGE_VIC20_16KB_6000    0x8015


#endif
