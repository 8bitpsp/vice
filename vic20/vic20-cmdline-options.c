/*
 * vic20-cmdline-options.c
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Ettore Perazzoli <ettore@comm2000.it>
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

#include "vice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"
#include "log.h"
#include "cmdline.h"
#include "machine.h"
#include "resources.h"
#ifdef HAS_TRANSLATION
#include "translate.h"
#endif
#include "vic20-cmdline-options.h"
#include "vic20mem.h"


/* This function parses the mem config string given as `-memory' and returns
 * the appropriate values or'ed together.
 *
 * basically we accept a comma separated list of options like the following:
 * ""   - no extension
 * none - same
 * all  - all blocks
 * 3k   - 3k space in block 0 (=3k extension cartridge)
 * 8k   - 1st 8k extension block
 * 16k  - 1st and 2nd 8 extension (= 16k extension cartridge)
 * 24k  - 1-3rd extension block (=8k and 16k extension cartridges)
 *
 * 0,1,2,3,5      - memory in respective block
 * 04,20,40,60,a0 - memory at respective address (same as block #s)
 *
 * example: xvic -memory ""
 *
 *            enables unexpanded computer
 *
 *          xvic -memory 60,a0
 *
 *            enables memory in blocks 3 and 5, which is the usual
 *            configuration for 16k rom modules
 *
 * 12/27/96 Alexander Lehmann <alex@mathematik.th-darmstadt.de>
 * Edited by Ettore to fit in the new command-line parsing.
 */

static int cmdline_memory(const char *param, void *extra_param)
{
    int memconf = 0;
    const char *memstring = param, *optend;

    /* Default is all banks. */
    if (!memstring) {
        memconf = VIC_BLK_ALL;
    } else {
        char *opt;

        opt = lib_malloc(strlen(param) + 1);
        while (*memstring) {
            for (optend = memstring; *optend && *optend != ','; optend++);

            strncpy(opt, memstring, optend - memstring);
            opt[optend - memstring] = '\0';

            if (strcmp(opt, "") == 0 || strcmp(opt, "none") == 0) {
                /* no extension */
            } else if (strcmp(opt, "all") == 0) {
                memconf = VIC_BLK_ALL;
            } else if (strcmp(opt, "3k") == 0) {
                memconf |= VIC_BLK0;
            } else if (strcmp(opt, "8k") == 0) {
                memconf |= VIC_BLK1;
            } else if (strcmp(opt, "16k") == 0) {
                memconf |= VIC_BLK1 | VIC_BLK2;
            } else if (strcmp(opt, "24k") == 0) {
                memconf |= VIC_BLK1 | VIC_BLK2 | VIC_BLK3;;
            } else if (strcmp(opt, "0") == 0 || strcmp(opt, "04") == 0) {
                memconf |= VIC_BLK0;
            } else if (strcmp(opt, "1") == 0 || strcmp(opt, "20") == 0) {
                memconf |= VIC_BLK1;
            } else if (strcmp(opt, "2") == 0 || strcmp(opt, "40") == 0) {
                memconf |= VIC_BLK2;
            } else if (strcmp(opt, "3") == 0 || strcmp(opt, "60") == 0) {
                memconf |= VIC_BLK3;
            } else if (strcmp(opt, "5") == 0 || strcmp(opt, "a0") == 0
                       || strcmp(opt, "A0") == 0) {
                memconf |= VIC_BLK5;
            } else {
                log_error(LOG_ERR,
                          "Unsupported memory extension option: `%s'.", opt);
                lib_free(opt);
                return -1;
            }
            memstring = optend;
            if (*memstring)
                memstring++;    /* skip ',' */
        }
        lib_free(opt);
    }

    /* FIXME: this is before log is initialized, right? */
    log_message(LOG_DEFAULT, "Extension memory enabled: ");

    if (memconf & VIC_BLK0) {
        resources_set_int("RAMBlock0", 1);
        log_message(LOG_DEFAULT, "blk0 ");
    } else {
        resources_set_int("RAMBlock0", 0);
    }
    if (memconf & VIC_BLK1) {
        resources_set_int("RAMBlock1", 1);
        log_message(LOG_DEFAULT, "blk1 ");
    } else {
        resources_set_int("RAMBlock1", 0);
    }
    if (memconf & VIC_BLK2) {
        resources_set_int("RAMBlock2", 1);
        log_message(LOG_DEFAULT, "blk2 ");
    } else {
        resources_set_int("RAMBlock2", 0);
    }
    if (memconf & VIC_BLK3) {
        resources_set_int("RAMBlock3", 1);
        log_message(LOG_DEFAULT, "blk3 ");
    } else {
        resources_set_int("RAMBlock3", 0);
    }
    if (memconf & VIC_BLK5) {
        resources_set_int("RAMBlock5", 1);
        log_message(LOG_DEFAULT, "blk5");
    } else {
        resources_set_int("RAMBlock5", 0);
    }
    if (memconf == 0)
        log_message(LOG_DEFAULT, "none");

    return 0;
}

#ifdef HAS_TRANSLATION
static cmdline_option_t const cmdline_options[] =
{
    { "-pal", SET_RESOURCE, 0, NULL, NULL, "MachineVideoStandard",
      (resource_value_t)MACHINE_SYNC_PAL, 0, IDCLS_USE_PAL_SYNC_FACTOR },
    { "-ntsc", SET_RESOURCE, 0, NULL, NULL, "MachineVideoStandard",
      (resource_value_t)MACHINE_SYNC_NTSC, 0, IDCLS_USE_NTSC_SYNC_FACTOR },
    { "-kernal", SET_RESOURCE, 1, NULL, NULL, "KernalName", NULL,
      IDCLS_P_NAME, IDCLS_SPECIFY_KERNAL_ROM_NAME },
    { "-basic", SET_RESOURCE, 1, NULL, NULL, "BasicName", NULL,
      IDCLS_P_NAME, IDCLS_SPECIFY_BASIC_ROM_NAME },
    { "-chargen", SET_RESOURCE, 1, NULL, NULL, "ChargenName", NULL,
      IDCLS_P_NAME, IDCLS_SPECIFY_CHARGEN_ROM_NAME },
    { "-memory", CALL_FUNCTION, 1, cmdline_memory, NULL, NULL, NULL,
      IDCLS_P_SPEC, IDCLS_SPECIFY_MEMORY_CONFIG},
    { "-emuid", SET_RESOURCE, 0, NULL, NULL, "EmuID", (resource_value_t)1,
      0, IDCLS_ENABLE_EMULATOR_ID},
    { "+emuid", SET_RESOURCE, 0, NULL, NULL, "EmuID", (resource_value_t)0,
      0, IDCLS_DISABLE_EMULATOR_ID},
    { "-ieee488", SET_RESOURCE, 0, NULL, NULL, "IEEE488", (resource_value_t)1,
      0, IDCLS_ENABLE_VIC1112_IEEE488},
    { "+ieee488", SET_RESOURCE, 0, NULL, NULL, "IEEE488", (resource_value_t)0,
      0, IDCLS_DISABLE_VIC1112_IEEE488},
#ifdef COMMON_KBD
    { "-keymap", SET_RESOURCE, 1, NULL, NULL, "KeymapIndex", NULL,
      IDCLS_P_NUMBER, IDCLS_SPECIFY_KEYMAP_FILE_INDEX },
    { "-symkeymap", SET_RESOURCE, 1, NULL, NULL, "KeymapSymFile", NULL,
      IDCLS_P_NAME, IDCLS_SPECIFY_SYM_KEYMAP_FILE_NAME },
    { "-poskeymap", SET_RESOURCE, 1, NULL, NULL, "KeymapPosFile", NULL,
      IDCLS_P_NAME, IDCLS_SPECIFY_POS_KEYMAP_FILE_NAME },
#endif
    { NULL}
};
#else
static cmdline_option_t const cmdline_options[] =
{
    { "-pal", SET_RESOURCE, 0, NULL, NULL, "MachineVideoStandard",
      (resource_value_t)MACHINE_SYNC_PAL, NULL, N_("Use PAL sync factor") },
    { "-ntsc", SET_RESOURCE, 0, NULL, NULL, "MachineVideoStandard",
      (resource_value_t)MACHINE_SYNC_NTSC, NULL, N_("Use NTSC sync factor") },
    { "-kernal", SET_RESOURCE, 1, NULL, NULL, "KernalName", NULL,
      N_("<name>"), N_("Specify name of Kernal ROM image") },
    { "-basic", SET_RESOURCE, 1, NULL, NULL, "BasicName", NULL,
      N_("<name>"), N_("Specify name of BASIC ROM image") },
    { "-chargen", SET_RESOURCE, 1, NULL, NULL, "ChargenName", NULL,
      N_("<name>"), N_("Specify name of character generator ROM image") },
    { "-memory", CALL_FUNCTION, 1, cmdline_memory, NULL, NULL, NULL,
      N_("<spec>"), N_("Specify memory configuration")},
    { "-emuid", SET_RESOURCE, 0, NULL, NULL, "EmuID", (resource_value_t)1,
      NULL, N_("Enable emulator identification")},
    { "+emuid", SET_RESOURCE, 0, NULL, NULL, "EmuID", (resource_value_t)0,
      NULL, N_("Disable emulator identification")},
    { "-ieee488", SET_RESOURCE, 0, NULL, NULL, "IEEE488", (resource_value_t)1,
      NULL, N_("Enable VIC-1112 IEEE488 interface")},
    { "+ieee488", SET_RESOURCE, 0, NULL, NULL, "IEEE488", (resource_value_t)0,
      NULL, N_("Disable VIC-1112 IEEE488 interface")},
#ifdef COMMON_KBD
    { "-keymap", SET_RESOURCE, 1, NULL, NULL, "KeymapIndex", NULL,
      N_("<number>"), N_("Specify index of keymap file (0=symbol, 1=positional)") },
    { "-symkeymap", SET_RESOURCE, 1, NULL, NULL, "KeymapSymFile", NULL,
      N_("<name>"), N_("Specify name of symbolic keymap file") },
    { "-poskeymap", SET_RESOURCE, 1, NULL, NULL, "KeymapPosFile", NULL,
      N_("<name>"), N_("Specify name of positional keymap file") },
#endif
    { NULL}
};
#endif

int vic20_cmdline_options_init(void)
{
    return cmdline_register_options(cmdline_options);
}

