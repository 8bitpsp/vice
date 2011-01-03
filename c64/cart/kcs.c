/*
 * kcs.c - Cartridge handling, KCS cart.
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

#include "vice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c64cart.h"
#include "c64cartmem.h"
#include "c64export.h"
#include "c64io.h"
#include "kcs.h"
#include "types.h"

static const c64export_resource_t export_res_kcs = {
    "KCS Power", 1, 1
};

static const c64export_resource_t export_res_simon = {
    "Simon's Basic", 1, 1
};

BYTE REGPARM1 kcs_io1_read(WORD addr)
{
    BYTE config;

    io_source = IO_SOURCE_KCS;

    /* A1 switches off roml/romh banks */
    config = (addr & 2) ? 2 : 0;

    cartridge_config_changed(config, config, CMODE_READ);
    return roml_banks[0x1e00 + (addr & 0xff)];
}

void REGPARM2 kcs_io1_store(WORD addr, BYTE value)
{
    cartridge_config_changed(1, 1, CMODE_WRITE);
}

BYTE REGPARM1 kcs_io2_read(WORD addr)
{
    io_source = IO_SOURCE_KCS;

    if (addr & 0x80) {
        cartridge_config_changed(0x43, 0x43, CMODE_READ);
    }
    return export_ram0[0x1f00 + (addr & 0xff)];
}

void REGPARM2 kcs_io2_store(WORD addr, BYTE value)
{
    if (!cart_ultimax_phi2) {
        cartridge_config_changed(1, 1, CMODE_WRITE);
    }
    export_ram0[0x1f00 + (addr & 0xff)] = value;
}

void kcs_freeze(void)
{
    cartridge_config_changed(3, 3, CMODE_READ);
}

void kcs_config_init(void)
{
    cartridge_config_changed(0, 0, CMODE_READ);
}

void kcs_config_setup(BYTE *rawcart)
{
    memcpy(roml_banks, rawcart, 0x2000);
    memcpy(romh_banks, &rawcart[0x2000], 0x2000);
    cartridge_config_changed(0, 0, CMODE_READ);
}

static int generic_kcs_crt_attach(FILE *fd, BYTE *rawcart)
{
    BYTE chipheader[0x10];
    int i;

    for (i = 0; i <= 1; i++) {
        if (fread(chipheader, 0x10, 1, fd) < 1) {
            return -1;
        }

        if (chipheader[0xc] != 0x80 && chipheader[0xc] != 0xa0) {
            return -1;
        }
        
        if (fread(&rawcart[(chipheader[0xc] << 8) - 0x8000], 0x2000, 1, fd) < 1) {
            return -1;
        }
    }

    return 0;
}

int kcs_crt_attach(FILE *fd, BYTE *rawcart)
{
    if (generic_kcs_crt_attach(fd,rawcart) < 0) {
        return -1;
    }

    if (c64export_add(&export_res_kcs) < 0) {
        return -1;
    }

    return 0;
}

int simon_crt_attach(FILE *fd, BYTE *rawcart)
{
    if (generic_kcs_crt_attach(fd,rawcart) < 0) {
        return -1;
    }

    if (c64export_add(&export_res_simon) < 0) {
        return -1;
    }

    return 0;
}

void kcs_detach(void)
{
    c64export_remove(&export_res_kcs);
}

void simon_detach(void)
{
    c64export_remove(&export_res_simon);
}
