/*
 * cartridge.c - Cartridge emulation.
 *
 * Written by
 *  Andr� Fachat <fachat@physik.tu-chemnitz.de>
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

/*
 * Those cartridge files are different from "normal" ROM images.
 * The VIC20 cartridges are saved with their start address before
 * the actual data.
 * This allows us to autodetect them etc.
 */

#include "vice.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifdef AMIGA_AROS
#define __AROS_OFF_T_DECLARED
#define __AROS_PID_T_DECLARED
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "archdep.h"
#include "cartridge.h"
#include "cmdline.h"
#include "lib.h"
#include "log.h"
#include "mem.h"
#include "monitor.h"
#include "resources.h"
#ifdef HAS_TRANSLATION
#include "translate.h"
#endif
#include "util.h"
#include "vic20mem.h"
#include "zfile.h"


/* Hm, if this gets more, I should introduce an array :-) */
static char *cartridge_file_2 = NULL;
static char *cartridge_file_4 = NULL;
static char *cartridge_file_6 = NULL;
static char *cartridge_file_A = NULL;
static char *cartridge_file_B = NULL;

static char *cartfile2 = NULL;
static char *cartfile4 = NULL;
static char *cartfile6 = NULL;
static char *cartfileA = NULL;
static char *cartfileB = NULL;

static int set_cartridge_file_2(const char *name, void *param)
{
    if (cartridge_file_2 != NULL && name != NULL
        && strcmp(name, cartridge_file_2) == 0)
        return 0;

    util_string_set(&cartridge_file_2, name);
    util_string_set(&cartfile2, name);
    return cartridge_attach_image(CARTRIDGE_VIC20_16KB_2000, cartfile2);
}

static int set_cartridge_file_4(const char *name, void *param)
{
    if (cartridge_file_4 != NULL && name != NULL
        && strcmp(name, cartridge_file_4) == 0)
        return 0;

    util_string_set(&cartridge_file_4, name);
    util_string_set(&cartfile4, name);
    return cartridge_attach_image(CARTRIDGE_VIC20_16KB_4000, cartfile4);
}

static int set_cartridge_file_6(const char *name, void *param)
{
    if (cartridge_file_6 != NULL && name != NULL
        && strcmp(name, cartridge_file_6) == 0)
        return 0;

    util_string_set(&cartridge_file_6, name);
    util_string_set(&cartfile6, name);
    return cartridge_attach_image(CARTRIDGE_VIC20_16KB_6000, cartfile6);
}

static int set_cartridge_file_A(const char *name, void *param)
{
    if (cartridge_file_A != NULL && name != NULL
        && strcmp(name, cartridge_file_A) == 0)
        return 0;

    util_string_set(&cartridge_file_A, name);
    util_string_set(&cartfileA, name);
    return cartridge_attach_image(CARTRIDGE_VIC20_8KB_A000, cartfileA);
}

static int set_cartridge_file_B(const char *name, void *param)
{
    if (cartridge_file_B != NULL && name != NULL
        && strcmp(name, cartridge_file_B) == 0)
        return 0;

    util_string_set(&cartridge_file_B, name);
    util_string_set(&cartfileB, name);
    return cartridge_attach_image(CARTRIDGE_VIC20_4KB_B000, cartfileB);
}

static const resource_string_t resources_string[] =
{
    { "CartridgeFile2000", "", RES_EVENT_STRICT, (resource_value_t)"",
      &cartridge_file_2, set_cartridge_file_2, NULL },
    { "CartridgeFile4000", "", RES_EVENT_STRICT, (resource_value_t)"",
      &cartridge_file_4, set_cartridge_file_4, NULL },
    { "CartridgeFile6000", "", RES_EVENT_STRICT, (resource_value_t)"",
      &cartridge_file_6, set_cartridge_file_6, NULL },
    { "CartridgeFileA000", "", RES_EVENT_STRICT, (resource_value_t)"",
      &cartridge_file_A, set_cartridge_file_A, NULL },
    { "CartridgeFileB000", "", RES_EVENT_STRICT, (resource_value_t)"",
      &cartridge_file_B, set_cartridge_file_B, NULL },
    { NULL }
};

int cartridge_resources_init(void)
{
    return resources_register_string(resources_string);
}

void cartridge_resources_shutdown(void)
{
    lib_free(cartridge_file_2);
    lib_free(cartridge_file_4);
    lib_free(cartridge_file_6);
    lib_free(cartridge_file_A);
    lib_free(cartridge_file_B);
    lib_free(cartfile2);
    lib_free(cartfile4);
    lib_free(cartfile6);
    lib_free(cartfileA);
    lib_free(cartfileB);
}

static int attach_cartB(const char *param, void *extra_param)
{
    return cartridge_attach_image(CARTRIDGE_VIC20_4KB_B000, param);
}

static int attach_cartA(const char *param, void *extra_param)
{
    return cartridge_attach_image(CARTRIDGE_VIC20_8KB_A000, param);
}

static int attach_cart6(const char *param, void *extra_param)
{
    return cartridge_attach_image(CARTRIDGE_VIC20_16KB_6000, param);
}

static int attach_cart4(const char *param, void *extra_param)
{
    return cartridge_attach_image(CARTRIDGE_VIC20_16KB_4000, param);
}

static int attach_cart2(const char *param, void *extra_param)
{
    return cartridge_attach_image(CARTRIDGE_VIC20_16KB_2000, param);
}

#ifdef HAS_TRANSLATION
static const cmdline_option_t cmdline_options[] =
{
    { "-cart2", CALL_FUNCTION, 1, attach_cart2, NULL, NULL, NULL,
      IDCLS_P_NAME, IDCLS_SPECIFY_EXT_ROM_2000_NAME },
    { "-cart4", CALL_FUNCTION, 1, attach_cart4, NULL, NULL, NULL,
      IDCLS_P_NAME, IDCLS_SPECIFY_EXT_ROM_4000_NAME },
    { "-cart6", CALL_FUNCTION, 1, attach_cart6, NULL, NULL, NULL,
      IDCLS_P_NAME, IDCLS_SPECIFY_EXT_ROM_6000_NAME },
    { "-cartA", CALL_FUNCTION, 1, attach_cartA, NULL, NULL, NULL,
      IDCLS_P_NAME, IDCLS_SPECIFY_EXT_ROM_A000_NAME },
    { "-cartB", CALL_FUNCTION, 1, attach_cartB, NULL, NULL, NULL,
      IDCLS_P_NAME, IDCLS_SPECIFY_EXT_ROM_B000_NAME },
    { NULL }
};
#else
static const cmdline_option_t cmdline_options[] =
{
    { "-cart2", CALL_FUNCTION, 1, attach_cart2, NULL, NULL, NULL,
      N_("<name>"), N_("Specify 4/8/16K extension ROM name at $2000") },
    { "-cart4", CALL_FUNCTION, 1, attach_cart4, NULL, NULL, NULL,
      N_("<name>"), N_("Specify 4/8/16K extension ROM name at $4000") },
    { "-cart6", CALL_FUNCTION, 1, attach_cart6, NULL, NULL, NULL,
      N_("<name>"), N_("Specify 4/8/16K extension ROM name at $6000") },
    { "-cartA", CALL_FUNCTION, 1, attach_cartA, NULL, NULL, NULL,
      N_("<name>"), N_("Specify 4/8K extension ROM name at $A000") },
    { "-cartB", CALL_FUNCTION, 1, attach_cartB, NULL, NULL, NULL,
      N_("<name>"), N_("Specify 4K extension ROM name at $B000") },
    { NULL }
};
#endif

int cartridge_cmdline_options_init(void)
{
    mon_cart_cmd.cartridge_attach_image = cartridge_attach_image;
    mon_cart_cmd.cartridge_detach_image = cartridge_detach_image;

    return cmdline_register_options(cmdline_options);
}

/* ------------------------------------------------------------------------- */

int cartridge_attach_image(int type, const char *filename)
{
    BYTE rawcart[0x4000];
    FILE *fd;
    int addr;
    size_t n;
    int type2 = CARTRIDGE_VIC20_DETECT;

    /* Attaching no cartridge always works.  */
    if (type == CARTRIDGE_NONE || filename==NULL || *filename == '\0')
        return 0;

    log_message(LOG_DEFAULT, "Attached cartridge type %d, file=`%s'.",
                type, filename);

    fd = zfile_fopen(filename, MODE_READ);
    if (!fd)
        return -1;

    addr = fgetc(fd);
    addr = (addr & 0xff) | ((fgetc(fd) << 8) & 0xff00);

    if (addr == 0x6000 || addr == 0x7000) {
        type2 = CARTRIDGE_VIC20_16KB_6000;
    } else if (addr == 0xA000) {
        type2 = CARTRIDGE_VIC20_8KB_A000;
    } else if (addr == 0x2000 || addr == 0x3000) {
        type2 = CARTRIDGE_VIC20_16KB_2000;
    } else if (addr == 0xB000) {
        type2 = CARTRIDGE_VIC20_4KB_B000;
    } else if (addr == 0x4000 || addr == 0x5000) {
        type2 = CARTRIDGE_VIC20_16KB_4000;
    }
    if (type2 == CARTRIDGE_VIC20_DETECT) {
        /* rewind to the beginning of the file (no load address) */
        fseek(fd, 0, SEEK_SET);
    }
    if (type == CARTRIDGE_VIC20_DETECT) {
        type = type2;
    }

    memset(rawcart, 0xff, 0x4000);

    switch (type) {
      case CARTRIDGE_VIC20_16KB_4000:
        if ((n = fread(rawcart, 0x1000, 4, fd)) < 1) {
            zfile_fclose(fd);
            return -1;
        }
        if (n < 4) {
            type = CARTRIDGE_VIC20_8KB_4000;
            if (n < 2) {
                memcpy(rawcart + 0x1000, rawcart, 0x1000);
            }
        }
        util_string_set(&cartfile4, filename);
        break;
      case CARTRIDGE_VIC20_16KB_2000:
        if ((n = fread(rawcart, 0x1000, 4, fd)) < 1) {
            zfile_fclose(fd);
            return -1;
        }
        if (n < 4) {
            type = CARTRIDGE_VIC20_8KB_2000;
            if (n < 2) {
                /* type = CARTRIDGE_VIC20_4KB_2000; */
                memcpy(rawcart + 0x1000, rawcart, 0x1000);
            }
        }
        util_string_set(&cartfile2, filename);
        break;
      case CARTRIDGE_VIC20_16KB_6000:
        if ((n = fread(rawcart, 0x1000, 4, fd)) < 1) {
            zfile_fclose(fd);
            return -1;
        }
        if (n < 4) {
            type = CARTRIDGE_VIC20_8KB_6000;
            if (n < 2) {
                /* type = CARTRIDGE_VIC20_4KB_6000; */
                memcpy(rawcart + 0x1000, rawcart, 0x1000);
            }
        }
        util_string_set(&cartfile6, filename);
        break;
      case CARTRIDGE_VIC20_8KB_A000:
        if ((n = fread(rawcart, 0x1000, 2, fd)) < 1) {
            zfile_fclose(fd);
            return -1;
        }
        if (n < 2) {
            if (cartfileB) {
                type = CARTRIDGE_VIC20_4KB_A000;
            } else {
                memcpy(rawcart + 0x1000, rawcart, 0x1000);
            }
        }
        util_string_set(&cartfileA, filename);
        break;
      case CARTRIDGE_VIC20_4KB_B000:
        if ((n = fread(rawcart, 0x1000, 1, fd)) < 1) {
            zfile_fclose(fd);
            return -1;
        }
        util_string_set(&cartfileB, filename);
        break;
      default:
        zfile_fclose(fd);
        return -1;
    }

    zfile_fclose(fd);

    mem_attach_cartridge(type, rawcart);

    return 0;
}

void cartridge_detach_image(void)
{
    mem_detach_cartridge(CARTRIDGE_VIC20_8KB_2000);
    mem_detach_cartridge(CARTRIDGE_VIC20_8KB_4000);
    mem_detach_cartridge(CARTRIDGE_VIC20_8KB_6000);
    mem_detach_cartridge(CARTRIDGE_VIC20_8KB_A000);
    util_string_set(&cartfile2, NULL);
    util_string_set(&cartfile4, NULL);
    util_string_set(&cartfile6, NULL);
    util_string_set(&cartfileA, NULL);
    util_string_set(&cartfileB, NULL);
}

void cartridge_set_default(void)
{
    set_cartridge_file_2(cartfile2, NULL);
    set_cartridge_file_4(cartfile4, NULL);
    set_cartridge_file_6(cartfile6, NULL);
    set_cartridge_file_A(cartfileA, NULL);
    set_cartridge_file_B(cartfileB, NULL);
}

const char *cartridge_get_file_name(WORD addr)
{
    switch (addr) {
      case 0x2000:
        return cartfile2;
      case 0x4000:
        return cartfile4;
      case 0x6000:
        return cartfile6;
      case 0xa000:
        return cartfileA;
      case 0xb000:
        return cartfileB;
      default:
        return NULL;
    }
}

