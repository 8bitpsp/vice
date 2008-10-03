/*
 * supersnapshot.c - Cartridge handling, Super Snapshot cart.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Nathan Huizinga <nathan.huizinga@chess.nl>
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
#include <string.h>

#include "c64cart.h"
#include "c64cartmem.h"
#include "c64export.h"
#include "c64io.h"
#include "supersnapshot.h"
#include "types.h"
#include "util.h"


static const c64export_resource_t export_res_v4 = {
    "Super Snapshot V4", 1, 1
};

static const c64export_resource_t export_res_v5 = {
    "Super Snapshot V5", 1, 1
};

/* Super Snapshot configuration flags.  */
static BYTE ramconfig = 0xff, romconfig = 9;
static int ram_bank = 0; /* Version 5 supports 4 - 8Kb RAM banks. */

BYTE REGPARM1 supersnapshot_v4_io1_read(WORD addr)
{
    io_source=IO_SOURCE_SS4;
    return export_ram0[0x1e00 + (addr & 0xff)];
}

void REGPARM2 supersnapshot_v4_io1_store(WORD addr, BYTE value)
{
    export_ram0[0x1e00 + (addr & 0xff)] = value;
}

BYTE REGPARM1 supersnapshot_v4_io2_read(WORD addr)
{
    io_source=IO_SOURCE_SS4;
    if ((addr & 0xff) == 1)
        return ramconfig;

    switch (roml_bank) {
      case 0:
        return roml_banks[addr & 0x1fff];
      case 1:
        return roml_banks[(addr & 0x1fff) + 0x2000];
      case 2:
        return roml_banks[(addr & 0x1fff) + 0x4000];
      case 3:
        return roml_banks[(addr & 0x1fff) + 0x6000];
    }
    io_source=IO_SOURCE_NONE;
    return 0;
}

void REGPARM2 supersnapshot_v4_io2_store(WORD addr, BYTE value)
{
    if ((addr & 0xff) == 0) {
        romconfig = (value == 2) ? 1 : 9;
        romconfig = (romconfig & 0xdf) | ((ramconfig == 0) ? 0x20 : 0);
        if ((value & 0x7f) == 0)
            romconfig = 35;
        if ((value & 0x7f) == 1 || (value & 0x7f) == 3)
            romconfig = 0;
        if ((value & 0x7f) == 6) {
            romconfig = 9;
            cartridge_release_freeze();
        }
        if ((value & 0x7f) == 9)
            romconfig = 6;
        cartridge_config_changed((BYTE)(romconfig & 3), romconfig, CMODE_WRITE);
    }
    if ((addr & 0xff) == 1) {
        if(((ramconfig - 1) & 0xff) == value) {
            ramconfig = value;
            romconfig |= 35;
        }
        if(((ramconfig + 1) & 0xff) == value) {
            ramconfig = value;
            romconfig &= 0xdd;
        }
        cartridge_config_changed((BYTE)(romconfig & 3), romconfig, CMODE_WRITE);
    }
}

BYTE REGPARM1 supersnapshot_v5_io1_read(WORD addr)
{
    io_source=IO_SOURCE_SS5;
    switch (roml_bank) {
      case 0:
        return roml_banks[0x1e00 + (addr & 0xff)];
      case 1:
        return roml_banks[0x1e00 + (addr & 0xff) + 0x2000];
      case 2:
        return roml_banks[0x1e00 + (addr & 0xff) + 0x4000];
      case 3:
        return roml_banks[0x1e00 + (addr & 0xff) + 0x6000];
    }
    io_source=IO_SOURCE_NONE;
    return 0;
}

void REGPARM2 supersnapshot_v5_io1_store(WORD addr, BYTE value)
{
    if (((addr & 0xff) == 0)
        || ((addr & 0xff) == 1)) {
        int banknr;

        if ((value & 1) == 1) {
            cartridge_release_freeze();
        }

        /* D0 = ~GAME */
        romconfig = ((value & 1) ^ 1);

        /* Calc RAM/ROM bank nr. */
        banknr = ((value >> 2) & 0x1) | ((value >> 3) & 0x2);

        /* ROM ~OE set? */
        if (((value >> 3) & 1) == 0) {
            romconfig |= (banknr << 3); /* Select ROM banknr. */
        }

        /* RAM ~OE set? */
        if (((value >> 1) & 1) == 0) {
            ram_bank = banknr;          /* Select RAM banknr. */
            romconfig |= (1 << 5);      /* export_ram */
            romconfig |= (1 << 1);      /* exrom */
        }
        cartridge_config_changed(1, romconfig, CMODE_WRITE);
    }
}

BYTE REGPARM1 supersnapshot_v4_roml_read(WORD addr)
{
    if (export_ram)
        return export_ram0[addr & 0x1fff];

    return roml_banks[(addr & 0x1fff) + (roml_bank << 13)];
}

void REGPARM2 supersnapshot_v4_roml_store(WORD addr, BYTE value)
{
    if (export_ram)
        export_ram0[addr & 0x1fff] = value;
}

BYTE REGPARM1 supersnapshot_v5_roml_read(WORD addr)
{
    if (export_ram)
        return export_ram0[(addr & 0x1fff) + (ram_bank << 13)];

    return roml_banks[(addr & 0x1fff) + (roml_bank << 13)];
}

void REGPARM2 supersnapshot_v5_roml_store(WORD addr, BYTE value)
{
    if (export_ram)
        export_ram0[(addr & 0x1fff) + (ram_bank << 13)] = value;
}

void supersnapshot_v4_freeze(void)
{
    cartridge_config_changed(35, 35, CMODE_READ);
}

void supersnapshot_v5_freeze(void)
{
    cartridge_config_changed(35, 35, CMODE_READ);
}

void supersnapshot_v4_config_init(void)
{
    cartridge_config_changed(9, 9, CMODE_READ);
}

void supersnapshot_v5_config_init(void)
{
    cartridge_store_io1((WORD)0xde00, 2);
}

void supersnapshot_v4_config_setup(BYTE *rawcart)
{
    memcpy(&roml_banks[0x0000], &rawcart[0x0000], 0x2000);
    memcpy(&romh_banks[0x0000], &rawcart[0x2000], 0x2000);
    memcpy(&roml_banks[0x2000], &rawcart[0x4000], 0x2000);
    memcpy(&romh_banks[0x2000], &rawcart[0x6000], 0x2000);
    cartridge_config_changed(9, 9, CMODE_READ);
}

void supersnapshot_v5_config_setup(BYTE *rawcart)
{
    memcpy(&roml_banks[0x0000], &rawcart[0x0000], 0x2000);
    memcpy(&romh_banks[0x0000], &rawcart[0x2000], 0x2000);
    memcpy(&roml_banks[0x2000], &rawcart[0x4000], 0x2000);
    memcpy(&romh_banks[0x2000], &rawcart[0x6000], 0x2000);
    memcpy(&roml_banks[0x4000], &rawcart[0x8000], 0x2000);
    memcpy(&romh_banks[0x4000], &rawcart[0xa000], 0x2000);
    memcpy(&roml_banks[0x6000], &rawcart[0xc000], 0x2000);
    memcpy(&romh_banks[0x6000], &rawcart[0xe000], 0x2000);
    cartridge_store_io1((WORD)0xde00, 2);
}

int supersnapshot_v4_bin_attach(const char *filename, BYTE *rawcart)
{
    if (util_file_load(filename, rawcart, 0x8000,
        UTIL_FILE_LOAD_SKIP_ADDRESS) < 0)
        return -1;

    if (c64export_add(&export_res_v4) < 0)
        return -1;

    return 0;
}

int supersnapshot_v5_bin_attach(const char *filename, BYTE *rawcart)
{
    if (util_file_load(filename, rawcart, 0x10000,
        UTIL_FILE_LOAD_SKIP_ADDRESS) < 0)
        return -1;

    if (c64export_add(&export_res_v5) < 0)
        return -1;

    return 0;
}

int supersnapshot_v5_crt_attach(FILE *fd, BYTE *rawcart)
{
    BYTE chipheader[0x10];
    int i;

    for (i = 0; i < 4; i++) {
        if (fread(chipheader, 0x10, 1, fd) < 1)
            return -1;

    if (chipheader[0xc] != 0x80 || chipheader[0xe] != 0x40
        || chipheader[0xb] > 3
        || fread(rawcart + 0x4000 * chipheader[0xb], chipheader[0xe] << 8, 1,
        fd) < 1)
        return -1;
    }

    if (c64export_add(&export_res_v5) < 0)
        return -1;

    return 0;
}

void supersnapshot_v4_detach(void)
{
    c64export_remove(&export_res_v4);
}

void supersnapshot_v5_detach(void)
{
    c64export_remove(&export_res_v5);
}
