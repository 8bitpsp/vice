/*
 * ui.c - PSP user interface.
 *
 * Written by
 *  Akop Karapetyan <dev@psp.akop.org>
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

#include "autostart.h"
#include "vice.h"
#include "machine.h"
#include "ui.h"
#include "attach.h"
#include "util.h"
#include "tape.h"
#include "cartridge.h"
#include "imagecontents.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pspkernel.h>

#include "lib/video.h"
#include "lib/ui.h"
#include "lib/pl_menu.h"
#include "lib/pl_file.h"
#include "lib/ctrl.h"
#include "lib/pl_psp.h"
#include "lib/pl_ini.h"
#include "lib/pl_util.h"
#include "lib/pl_gfx.h"
#include "libmz/unzip.h"

#define TAB_QUICKLOAD 0
#define TAB_STATE     1
#define TAB_CONTROLS  2
#define TAB_OPTIONS   3
#define TAB_SYSTEM    4
#define TAB_MAX       TAB_SYSTEM
#define TAB_ABOUT     5

#define OPTION_DISPLAY_MODE  0x01
#define OPTION_FRAME_LIMITER 0x02
#define OPTION_CLOCK_FREQ    0x03
#define OPTION_SHOW_FPS      0x04
#define OPTION_CONTROL_MODE  0x05
#define OPTION_ANIMATE       0x06
#define OPTION_TOGGLE_VK     0x08
#define OPTION_AUTOLOAD      0x09

#define SYSTEM_SCRNSHOT     0x11
#define SYSTEM_RESET        0x12
#define SYSTEM_JOYPORT      0x13
#define SYSTEM_DRIVE8       0x18
#define SYSTEM_CART         0x20
#define SYSTEM_TAPE         0x21

#define GET_DRIVE(code) ((code)&0x0F)
#define GET_DRIVE_MENU_ID(code) (((code)&0x0F)|0x10)

static const char 
  PresentSlotText[] = "\026\244\020 Save\t\026\001\020 Load\t\026\243\020 Delete\t\026"PSP_CHAR_START"\020 Export",
  EmptySlotText[]   = "\026\244\020 Save\t\026"PSP_CHAR_START"\020 Export",
  ControlHelpText[] = "\026\250\020 Change mapping\t"
                      "\026\244\020 Set as default\t\026\243\020 Load defaults";

static const char 
  *QuickloadFilter[] =
     { "ZIP",
       "D64","D71","D80","D81","D82","G64","G41","X64",
       "T64","TAP",
       "PRG","P00",
       "CRT",'\0' },
  *DiskFilter[] =
     { "ZIP", "D64","D71","D80","D81","D82","G64","G41","X64",'\0' },
  *CartFilter[] =
     { "ZIP", "CRT",'\0' },
  *TapeFilter[] =
     { "ZIP", "T64","TAP",'\0' };

/* Tab labels */
static const char *TabLabel[] = 
{
  "Game",
  "Save/Load",
  "Controls",
  "Options",
  "System",
  "About"
};

/* Menu definitions */
PL_MENU_OPTIONS_BEGIN(ToggleOptions)
  PL_MENU_OPTION("Disabled", 0)
  PL_MENU_OPTION("Enabled", 1)
PL_MENU_OPTIONS_END
PL_MENU_OPTIONS_BEGIN(ScreenSizeOptions)
  PL_MENU_OPTION("Actual size", DISPLAY_MODE_UNSCALED)
  PL_MENU_OPTION("16:9 scaled (fit screen)", DISPLAY_MODE_FILL_SCREEN)
PL_MENU_OPTIONS_END
PL_MENU_OPTIONS_BEGIN(PspClockFreqOptions)
  PL_MENU_OPTION("222 MHz", 222)
  PL_MENU_OPTION("266 MHz", 266)
  PL_MENU_OPTION("300 MHz", 300)
  PL_MENU_OPTION("333 MHz", 333)
PL_MENU_OPTIONS_END
PL_MENU_OPTIONS_BEGIN(JoyPortOptions)
  PL_MENU_OPTION("Port 1", 1)
  PL_MENU_OPTION("Port 2", 2)
PL_MENU_OPTIONS_END
PL_MENU_OPTIONS_BEGIN(ControlModeOptions)
  PL_MENU_OPTION("\026\242\020 cancels, \026\241\020 confirms (US)", 0)
  PL_MENU_OPTION("\026\241\020 cancels, \026\242\020 confirms (Japan)", 1)
PL_MENU_OPTIONS_END
PL_MENU_OPTIONS_BEGIN(VkModeOptions)
  PL_MENU_OPTION("Display when button is held down (classic mode)", 0)
  PL_MENU_OPTION("Toggle display on and off when button is pressed", 1)
PL_MENU_OPTIONS_END
PL_MENU_OPTIONS_BEGIN(AutoloadSlots)
  PL_MENU_OPTION("Disabled", -1)
  PL_MENU_OPTION("1", 0)
  PL_MENU_OPTION("2", 1)
  PL_MENU_OPTION("3", 2)
  PL_MENU_OPTION("4", 3)
  PL_MENU_OPTION("5", 4)
  PL_MENU_OPTION("6", 5)
  PL_MENU_OPTION("7", 6)
  PL_MENU_OPTION("8", 7)
  PL_MENU_OPTION("9", 8)
  PL_MENU_OPTION("10",9)
PL_MENU_OPTIONS_END
PL_MENU_OPTIONS_BEGIN(MappableButtons)
  /* Unmapped */
  PL_MENU_OPTION("None", 0)
  /* Special */
  PL_MENU_OPTION("Special: Open Menu",     (SPC|SPC_MENU))
  PL_MENU_OPTION("Special: Show keyboard", (SPC|SPC_KYBD))
  /* Function keys */
  PL_MENU_OPTION("Joystick Up",    JOY|0x01)
  PL_MENU_OPTION("Joystick Down",  JOY|0x02)
  PL_MENU_OPTION("Joystick Left",  JOY|0x04)
  PL_MENU_OPTION("Joystick Right", JOY|0x08)
  PL_MENU_OPTION("Joystick Fire",  JOY|0x10)
  /* Cursor */
  PL_MENU_OPTION("Up/Down",    CK(0,7,8))
  PL_MENU_OPTION("Left/Right", CK(0,2,8))
  PL_MENU_OPTION("Space",        CK(7,4,8))
  /* Function keys */
  PL_MENU_OPTION("F1/F2", CK(0,4,8))
  PL_MENU_OPTION("F3/F4", CK(0,5,8))
  PL_MENU_OPTION("F5/F6", CK(0,6,8))
  PL_MENU_OPTION("F7/F8", CK(0,3,8))
  /* Etc.. */
  PL_MENU_OPTION("Clr/Home",     CK(6,3,8))
  PL_MENU_OPTION("Ins/Del",      CK(0,0,8))
  PL_MENU_OPTION("Ctrl",         CK(7,2,8))
  PL_MENU_OPTION("Restore",      CK(0xf,0xf,8))
  PL_MENU_OPTION("Run/Stop",     CK(7,7,8))
  PL_MENU_OPTION("Return",       CK(0,1,8))
  PL_MENU_OPTION("C= (CBM)",     CK(7,5,2))
  PL_MENU_OPTION("L. Shift",     CK(1,7,2))
  PL_MENU_OPTION("R. Shift",     CK(6,4,4))
  /* Symbols */
  PL_MENU_OPTION("+", CK(5,0,8))
  PL_MENU_OPTION("-", CK(5,3,8))
  PL_MENU_OPTION("Pound sterling", CK(6,0,8))
  PL_MENU_OPTION("@", CK(5,6,8))
  PL_MENU_OPTION("*", CK(6,1,8))
  PL_MENU_OPTION("^", CK(6,6,8))
  PL_MENU_OPTION("[", CK(5,5,8))
  PL_MENU_OPTION("]", CK(6,2,8))
  PL_MENU_OPTION("=", CK(6,5,8))
  PL_MENU_OPTION("<", CK(5,7,8))
  PL_MENU_OPTION(">", CK(5,4,8))
  PL_MENU_OPTION("?", CK(6,7,8))
  /* Digits */
  PL_MENU_OPTION("1", CK(7,0,8))
  PL_MENU_OPTION("2", CK(7,3,8))
  PL_MENU_OPTION("3", CK(1,0,8))
  PL_MENU_OPTION("4", CK(1,3,8))
  PL_MENU_OPTION("5", CK(2,0,8))
  PL_MENU_OPTION("6", CK(2,3,8))
  PL_MENU_OPTION("7", CK(3,0,8))
  PL_MENU_OPTION("8", CK(3,3,8))
  PL_MENU_OPTION("9", CK(4,0,8))
  PL_MENU_OPTION("0", CK(4,3,8))
  /* Alphabet */
  PL_MENU_OPTION("A", CK(1,2,8))
  PL_MENU_OPTION("B", CK(3,4,8))
  PL_MENU_OPTION("C", CK(2,4,8))
  PL_MENU_OPTION("D", CK(2,2,8))
  PL_MENU_OPTION("E", CK(1,6,8))
  PL_MENU_OPTION("F", CK(2,5,8))
  PL_MENU_OPTION("G", CK(3,2,8))
  PL_MENU_OPTION("H", CK(3,5,8))
  PL_MENU_OPTION("I", CK(4,1,8))
  PL_MENU_OPTION("J", CK(4,2,8))
  PL_MENU_OPTION("K", CK(4,5,8))
  PL_MENU_OPTION("L", CK(5,2,8))
  PL_MENU_OPTION("M", CK(4,4,8))
  PL_MENU_OPTION("N", CK(4,7,8))
  PL_MENU_OPTION("O", CK(4,6,8))
  PL_MENU_OPTION("P", CK(5,1,8))
  PL_MENU_OPTION("Q", CK(7,6,8))
  PL_MENU_OPTION("R", CK(2,1,8))
  PL_MENU_OPTION("S", CK(1,5,8))
  PL_MENU_OPTION("T", CK(2,6,8))
  PL_MENU_OPTION("U", CK(3,6,8))
  PL_MENU_OPTION("V", CK(3,7,8))
  PL_MENU_OPTION("W", CK(1,1,8))
  PL_MENU_OPTION("X", CK(2,7,8))
  PL_MENU_OPTION("Y", CK(3,1,8))
  PL_MENU_OPTION("Z", CK(1,4,8))
PL_MENU_OPTIONS_END

PL_MENU_ITEMS_BEGIN(SystemMenuDef)
  PL_MENU_HEADER("Input")
  PL_MENU_ITEM("Joystick port",SYSTEM_JOYPORT,JoyPortOptions,
               "\026\250\020 Select joystick port")
  PL_MENU_HEADER("Peripherals")
  PL_MENU_ITEM("Cartridge",SYSTEM_CART,NULL,
               "\026\001\020 Browse\t\026\243\020 Eject")
  PL_MENU_ITEM("Tape",SYSTEM_TAPE,NULL,
               "\026\001\020 Browse\t\026\250\020 Autoload program\t\026\243\020 Eject")
  PL_MENU_ITEM("Drive 8",SYSTEM_DRIVE8,NULL,
               "\026\001\020 Browse\t\026\250\020 Autoload program\t\026\243\020 Eject")
  PL_MENU_HEADER("Options")
  PL_MENU_ITEM("Reset",SYSTEM_RESET,NULL,
               "\026\001\020 Reset system")
  PL_MENU_ITEM("Save screenshot",SYSTEM_SCRNSHOT,NULL,
               "\026\001\020 Save screenshot")
PL_MENU_ITEMS_END
PL_MENU_ITEMS_BEGIN(OptionMenuDef)
  PL_MENU_HEADER("Video")
  PL_MENU_ITEM("Screen size",OPTION_DISPLAY_MODE,ScreenSizeOptions,
               "\026\250\020 Change screen size")
  PL_MENU_HEADER("Input")
  PL_MENU_ITEM("Virtual keyboard mode",OPTION_TOGGLE_VK,VkModeOptions,
               "\026\250\020 Select virtual keyboard mode")
  PL_MENU_HEADER("Enhancements")
  PL_MENU_ITEM("Autoload slot",OPTION_AUTOLOAD,AutoloadSlots,
               "\026\250\020 Select save state to be loaded automatically")
  PL_MENU_HEADER("Performance")
  PL_MENU_ITEM("PSP clock frequency",OPTION_CLOCK_FREQ,PspClockFreqOptions,
               "\026\250\020 Larger values: faster emulation, faster battery depletion (default: 222MHz)")
  PL_MENU_ITEM("Show FPS counter",OPTION_SHOW_FPS,ToggleOptions,
               "\026\250\020 Show/hide the frames-per-second counter")
  PL_MENU_HEADER("Menu")
  PL_MENU_ITEM("Button mode",OPTION_CONTROL_MODE,ControlModeOptions,
               "\026\250\020 Change OK and Cancel button mapping")
  PL_MENU_ITEM("Animations",OPTION_ANIMATE,ToggleOptions,
               "\026\250\020 Enable/disable menu animations")
PL_MENU_ITEMS_END
PL_MENU_ITEMS_BEGIN(ControlMenuDef)
  PL_MENU_ITEM(PSP_CHAR_ANALUP,0,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_ANALDOWN,1,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_ANALLEFT,2,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_ANALRIGHT,3,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_UP,4,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_DOWN,5,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_LEFT,6,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_RIGHT,7,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_SQUARE,8,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_CROSS,9,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_CIRCLE,10,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_TRIANGLE,11,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_LTRIGGER,12,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_RTRIGGER,13,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_SELECT,14,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_START,15,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_LTRIGGER"+"PSP_CHAR_RTRIGGER,16,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_START"+"PSP_CHAR_SELECT,17,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_LTRIGGER"+"PSP_CHAR_SELECT,18,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_RTRIGGER"+"PSP_CHAR_SELECT,19,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_LTRIGGER"+"PSP_CHAR_SQUARE,20,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_LTRIGGER"+"PSP_CHAR_CROSS,21,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_LTRIGGER"+"PSP_CHAR_CIRCLE,22,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_LTRIGGER"+"PSP_CHAR_TRIANGLE,23,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_RTRIGGER"+"PSP_CHAR_SQUARE,24,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_RTRIGGER"+"PSP_CHAR_CROSS,25,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_RTRIGGER"+"PSP_CHAR_CIRCLE,26,MappableButtons,ControlHelpText)
  PL_MENU_ITEM(PSP_CHAR_RTRIGGER"+"PSP_CHAR_TRIANGLE,27,MappableButtons,ControlHelpText)
PL_MENU_ITEMS_END

/* Default configuration */
static psp_ctrl_map_t default_map =
{
  {
    JOY|0x01,  /* Analog Up    */
    JOY|0x02,  /* Analog Down  */
    JOY|0x04,  /* Analog Left  */
    JOY|0x08,  /* Analog Right */
    JOY|0x01,  /* D-pad Up     */
    JOY|0x02,  /* D-pad Down   */
    JOY|0x04,  /* D-pad Left   */
    JOY|0x08,  /* D-pad Right  */
    0,         /* Square       */
    JOY|0x10,  /* Cross        */
    CK(7,4,8), /* Circle       */
    0,         /* Triangle     */
    0,         /* L Trigger    */
    SPC|SPC_KYBD,          /* R Trigger    */
    0,         /* Select       */
    CK(7,7,8), /* Start        */
    SPC|SPC_MENU,          /* L+R Triggers */
    0,                     /* Start+Select */
    0,                     /* L + Select   */
    0,                     /* R + Select   */
    0,                     /* L + Square   */
    0,                     /* L + Cross    */
    0,                     /* L + Circle   */
    0,                     /* L + Triangle */
    0,                     /* R + Square   */
    0,                     /* R + Cross    */
    0,                     /* R + Circle   */
    0,                     /* R + Triangle */
  }
};

/* Menu callbacks */
static int         OnSplashButtonPress(const struct PspUiSplash *splash,
                                       u32 button_mask);
static void        OnSplashRender(const void *uiobject, const void *null);
static const char* OnSplashGetStatusBarText(const struct PspUiSplash *splash);

static int  OnGenericCancel(const void *uiobject, const void* param);
static void OnGenericRender(const void *uiobject, const void *item_obj);
static int  OnGenericButtonPress(const PspUiFileBrowser *browser, const char *path,
                                 u32 button_mask);

static int OnMenuOk(const void *menu, const void *item);
static int OnMenuButtonPress(const struct PspUiMenu *uimenu, pl_menu_item* item,
                             u32 button_mask);
static int OnMenuItemChanged(const struct PspUiMenu *uimenu, pl_menu_item* item,
                             const pl_menu_option* option);

static void OnSystemRender(const void *uiobject, const void *item_obj);

static int OnQuickloadOk(const void *browser, const void *path);
static int OnFileOk(const void *browser, const void *path);

static int OnSaveStateOk(const void *gallery, const void *item);
static int OnSaveStateButtonPress(const PspUiGallery *gallery,
                                  pl_menu_item *sel,
                                  u32 button_mask);

PspUiSplash SplashScreen = 
{
  OnSplashRender,
  OnGenericCancel,
  OnSplashButtonPress,
  OnSplashGetStatusBarText
};
PspUiFileBrowser
  QuickloadBrowser = 
{
  OnGenericRender,
  OnQuickloadOk,
  OnGenericCancel,
  OnGenericButtonPress,
  QuickloadFilter,
  0
},
  FileBrowser = 
{
  OnGenericRender,
  OnFileOk,
  NULL,
  NULL,
  NULL,
  NULL
};

PspUiMenu
  OptionUiMenu =
  {
    OnGenericRender,       /* OnRender() */
    OnMenuOk,              /* OnOk() */
    OnGenericCancel,       /* OnCancel() */
    OnMenuButtonPress,     /* OnButtonPress() */
    OnMenuItemChanged,     /* OnItemChanged() */
  },
  SystemUiMenu =
  {
    OnSystemRender,        /* OnRender() */
    OnMenuOk,              /* OnOk() */
    OnGenericCancel,       /* OnCancel() */
    OnMenuButtonPress,     /* OnButtonPress() */
    OnMenuItemChanged,     /* OnItemChanged() */
  },
  ControlUiMenu =
  {
    OnGenericRender,       /* OnRender() */
    OnMenuOk,              /* OnOk() */
    OnGenericCancel,       /* OnCancel() */
    OnMenuButtonPress,     /* OnButtonPress() */
    OnMenuItemChanged,     /* OnItemChanged() */
  };
PspUiGallery SaveStateGallery = 
{
  OnGenericRender,             /* OnRender() */
  OnSaveStateOk,               /* OnOk() */
  OnGenericCancel,             /* OnCancel() */
  OnSaveStateButtonPress,      /* OnButtonPress() */
  NULL                         /* Userdata */
};

pl_file_path psp_current_game = {'\0'},
             psp_game_path = {'\0'},
             psp_save_state_path,
             psp_screenshot_path,
             psp_config_path,
             psp_tmp_file = {'\0'};
psp_options_t psp_options;

#define CURRENT_GAME (psp_current_game)
#define GAME_LOADED (psp_current_game[0] != '\0')
#define SET_AS_CURRENT_GAME(filename) \
  strncpy(psp_current_game, filename, sizeof(psp_current_game) - 1)

static int psp_controls_changed;
static int psp_tab_index;
static PspImage *psp_menu_bg;
static PspImage *psp_blank_ss_icon;
static int psp_exit_menu;
extern PspImage *Screen;
psp_ctrl_map_t current_map;

static void psp_load_options();
static int  psp_save_options();

static void psp_init_controls(psp_ctrl_map_t *config);
static int  psp_load_controls(const char *filename, psp_ctrl_map_t *config);
static int  psp_save_controls(const char *filename, const psp_ctrl_map_t *config);

static PspImage* psp_load_state_icon(const char *path);
static int psp_load_state(const char *path);
static PspImage* psp_save_state(const char *path, PspImage *icon);

static void psp_display_state_tab();
static void psp_display_control_tab();
static void psp_display_system_tab();
static void psp_refresh_devices();

int ui_init(int *argc, char **argv)
{
  /* Initialize paths */
  sprintf(psp_save_state_path, "%sstates/", pl_psp_get_app_directory());
  sprintf(psp_screenshot_path, "ms0:/PSP/PHOTO/%s/", PSP_APP_NAME);
  sprintf(psp_config_path, "%sconfig/", pl_psp_get_app_directory());

  /* Initialize menus */
  pl_menu_create(&OptionUiMenu.Menu, OptionMenuDef);
  pl_menu_create(&SystemUiMenu.Menu, SystemMenuDef);
  pl_menu_create(&ControlUiMenu.Menu, ControlMenuDef);

  /* Init NoSaveState icon image */
  psp_blank_ss_icon = pspImageCreate(192, 136, PSP_IMAGE_16BPP);
  pspImageClear(psp_blank_ss_icon, RGB(0x3e,0x31,0xa2));

  /* Initialize state menu */
  int i;
  pl_menu_item *item;
  for (i = 0; i < 10; i++)
  {
    item = pl_menu_append_item(&SaveStateGallery.Menu, i, NULL);
    pl_menu_set_item_help_text(item, EmptySlotText);
  }

  /* Load the background image */
  psp_menu_bg = pspImageLoadPng("background.png");

  psp_load_options();

  /* Initialize UI components */
  UiMetric.Background = psp_menu_bg;
  UiMetric.Font = &PspStockFont;
  UiMetric.Left = 8;
  UiMetric.Top = 24;
  UiMetric.Right = 472;
  UiMetric.Bottom = 240;
  UiMetric.OkButton = (!psp_options.control_mode)
    ? PSP_CTRL_CROSS : PSP_CTRL_CIRCLE;
  UiMetric.CancelButton = (!psp_options.control_mode)
    ? PSP_CTRL_CIRCLE : PSP_CTRL_CROSS;
  UiMetric.ScrollbarColor = PSP_COLOR_GRAY;
  UiMetric.ScrollbarBgColor = 0x44ffffff;
  UiMetric.ScrollbarWidth = 10;
  UiMetric.TextColor = PSP_COLOR_GRAY;
  UiMetric.SelectedColor = COLOR(0xf7,0xc2,0x50,0xFF);
  UiMetric.SelectedBgColor = COLOR(0xff,0xff,0xff,0x99);
  UiMetric.StatusBarColor = PSP_COLOR_WHITE;
  UiMetric.BrowserFileColor = PSP_COLOR_GRAY;
  UiMetric.BrowserDirectoryColor = PSP_COLOR_YELLOW;
  UiMetric.GalleryIconsPerRow = 5;
  UiMetric.GalleryIconMarginWidth = 8;
  UiMetric.MenuItemMargin = 20;
  UiMetric.MenuSelOptionBg = PSP_COLOR_BLACK;
  UiMetric.MenuOptionBoxColor = PSP_COLOR_GRAY;
  UiMetric.MenuOptionBoxBg = COLOR(0x3e,0x31,0xa2,0xCC);
  UiMetric.MenuDecorColor = UiMetric.SelectedColor;
  UiMetric.DialogFogColor = COLOR(0x3e,0x31,0xa2,0x66);
  UiMetric.TitlePadding = 4;
  UiMetric.TitleColor = PSP_COLOR_WHITE;
  UiMetric.MenuFps = 30;
  UiMetric.TabBgColor = PSP_COLOR_WHITE;
  UiMetric.Animate = psp_options.animate_menu;
  UiMetric.BrowserScreenshotPath = psp_screenshot_path;
  UiMetric.BrowserScreenshotDelay = 30;

  psp_tab_index = TAB_ABOUT;

  /* Load default configuration */
  psp_load_controls("DEFAULT", &default_map);
  psp_init_controls(&current_map);

  return 0;
}

void ui_shutdown()
{
  if (psp_menu_bg) pspImageDestroy(psp_menu_bg);

  pl_menu_destroy(&ControlUiMenu.Menu);
  pl_menu_destroy(&OptionUiMenu.Menu);
  pl_menu_destroy(&SystemUiMenu.Menu);
  pl_menu_destroy(&SaveStateGallery.Menu);

  /* Remove temp. file (if any) */
  if (*psp_tmp_file && pl_file_exists(psp_tmp_file))
    pl_file_rm(psp_tmp_file);

  psp_save_options();
}

/**************************/
/* Helper functions       */
/**************************/

static void psp_refresh_devices()
{
  int unit;
  pl_menu_item *item;
  const char *name;
  char *contents, *line;

  /* Refresh tape contents */
  do /* For flow control - not a loop */
  {
    name = tape_image_dev1->name; /* Filename */
    item = pl_menu_find_item_by_id(&SystemUiMenu.Menu, SYSTEM_TAPE);
    pl_menu_clear_options(item); /* Clear current names */

    if (!name) break;

    /* Read contents into a string (\n delimited) */
    contents = image_contents_read_string(IMAGE_CONTENTS_TAPE, name, 0,
                                          IMAGE_CONTENTS_STRING_ASCII);

    if (!contents) break;

    /* Add an entry for each line */
    line = strtok(contents, "\n");
    while (line)
    {
      pl_menu_append_option(item, line, line, 1);
      line = strtok(NULL, "\n");
    }

    /* Free TOC */
    if (contents)
      free(contents);

    /* Select first option */
    pl_menu_select_option_by_index(item, 0);
  } while (0);

  /* Refresh drive contents */  
  for (unit = 8; unit <= 8; unit++)
  {
    name = file_system_get_disk_name(unit); /* Filename */
    item = pl_menu_find_item_by_id(&SystemUiMenu.Menu, GET_DRIVE_MENU_ID(unit));
    pl_menu_clear_options(item); /* Clear current names */

    if (!name) continue;

    /* Read contents into a string (\n delimited) */
    contents = image_contents_read_string(IMAGE_CONTENTS_DISK, name, unit,
                                          IMAGE_CONTENTS_STRING_ASCII);

    if (!contents) continue;

    /* Add an entry for each line */
    line = strtok(contents, "\n");
    while (line)
    {
      pl_menu_append_option(item, line, line, 1);
      line = strtok(NULL, "\n");
    }

    /* Free TOC */
    if (contents)
      free(contents);

    /* Select first option */
    pl_menu_select_option_by_index(item, 0);
  }
}

static void psp_display_system_tab()
{
  psp_refresh_devices();
  pspUiOpenMenu(&SystemUiMenu, NULL);
}

static void psp_display_control_tab()
{
  pl_menu_item *item;
  const char *config_name = (GAME_LOADED)
    ? pl_file_get_filename(psp_current_game) : "BASIC";
  char *game_name = strdup(config_name);
  char *dot = strrchr(game_name, '.');
  int i;
  if (dot) *dot='\0';

  /* Load current button mappings */
  for (item = ControlUiMenu.Menu.items, i = 0; item; item = item->next, i++)
    pl_menu_select_option_by_value(item, (void*)current_map.button_map[i]);

  psp_controls_changed = 0;

  pspUiOpenMenu(&ControlUiMenu, game_name);
  free(game_name);

  /* Save to MS, if configuration changed */
  if (psp_controls_changed)
  {
    pspUiFlashMessage("Saving configuration, please wait...");
    if (!psp_save_controls(config_name, &current_map))
      pspUiAlert("ERROR: Changes not saved");
  }
}

static void psp_init_controls(psp_ctrl_map_t *config)
{
  /* Initialize to default configuration */
  if (config != &default_map)
    memcpy(config, &default_map, sizeof(psp_ctrl_map_t));
}

static int psp_load_controls(const char *filename, psp_ctrl_map_t *config)
{
  pl_file_path path;
  snprintf(path, sizeof(path), "%s%s.cnf", psp_config_path, filename);

  /* Initialize default controls */
  psp_init_controls(config);

  /* No configuration; defaults are fine */
  if (!pl_file_exists(path))
    return 1;

  /* Open file for reading */
  FILE *file = fopen(path, "r");
  if (!file) return 0;

  /* Load defaults; attempt to read controls from file */
  psp_init_controls(config);
  int nread = fread(config, sizeof(uint32_t), MAP_BUTTONS, file);

  fclose(file);

  /* Reading less than MAP_BUTTONS is ok; may be an older config file */
  if (nread < 1)
  {
    psp_init_controls(config);
    return 0;
  }

  return 1;
}

static int psp_save_controls(const char *filename, const psp_ctrl_map_t *config)
{
  pl_file_path path;
  snprintf(path, sizeof(path)-1, "%s%s.cnf", psp_config_path, filename);

  /* Open file for writing */
  FILE *file = fopen(path, "w");
  if (!file) return 0;

  /* Write contents of struct */
  int nwritten = fwrite(config, sizeof(psp_ctrl_map_t), 1, file);
  fclose(file);

  return (nwritten == 1);
}

/* Load state icon */
static PspImage* psp_load_state_icon(const char *path)
{
  FILE *f = fopen(path, "r");
  if (!f) return NULL;

  /* Load image */
  PspImage *image = pspImageLoadPngFd(f);
  fclose(f);

  return image;
}

/* Load state */
static int psp_load_state(const char *path)
{
return 0; // TODO
  /* Open file for reading */
  FILE *f = fopen(path, "r");
  if (!f) return 0;

  /* Load image into temporary object */
  PspImage *image = pspImageLoadPngFd(f);
  pspImageDestroy(image);

  /* Load the state data */
  int error;// = !snapshot_read_file(path, f);
  fclose(f);

  return error;
}

/* Save state */
static PspImage* psp_save_state(const char *path, PspImage *icon)
{
return 0; /*TODO */
  /* Open file for writing */
  FILE *f;
  if (!(f = fopen(path, "w")))
    return NULL;

  /* Create thumbnail */
  PspImage *thumb;
  thumb = (icon->Viewport.Width <= 256)
    ? pspImageCreateCopy(icon) : pspImageCreateThumbnail(icon);
  if (!thumb) { fclose(f); return NULL; }

  /* Write the thumbnail */
  if (!pspImageSavePngFd(f, thumb))
  {
    pspImageDestroy(thumb);
    fclose(f);
    return NULL;
  }

  /* Write the state */
//  if (snapshot_write_file(path, f))
  {
    pspImageDestroy(thumb);
    thumb = NULL;
  }

  fclose(f);
  return thumb;
}

static void psp_load_options()
{
  pl_file_path path;
  snprintf(path, sizeof(path) - 1, "%s%s", pl_psp_get_app_directory(), "options.ini");

  /* Load INI */
  pl_ini_file file;
  pl_ini_load(&file, path);

  psp_options.autoload_slot = pl_ini_get_int(&file, "System", "Autoload Slot", 9);
  psp_options.joyport = pl_ini_get_int(&file, "System", "Joystick Port", 2);
  psp_options.display_mode = pl_ini_get_int(&file, "Video", "Display Mode", 
                                            DISPLAY_MODE_UNSCALED);
  psp_options.clock_freq = pl_ini_get_int(&file, "Video", "PSP Clock Frequency", 222);
  psp_options.show_fps = pl_ini_get_int(&file, "Video", "Show FPS", 0);
  psp_options.control_mode = pl_ini_get_int(&file, "Menu", "Control Mode", 0);
  psp_options.animate_menu = pl_ini_get_int(&file, "Menu", "Animate", 1);
  psp_options.toggle_vk = pl_ini_get_int(&file, "Input", "VK Mode", 0);
  pl_ini_get_string(&file, "File", "Game Path", NULL, psp_game_path, sizeof(psp_game_path));

  /* Clean up */
  pl_ini_destroy(&file);
}

static int psp_save_options()
{
  pl_file_path path;
  snprintf(path, sizeof(path)-1, "%s%s", pl_psp_get_app_directory(), "options.ini");

  /* Initialize INI structure */
  pl_ini_file file;
  pl_ini_create(&file);
  pl_ini_set_int(&file, "System", "Autoload Slot", psp_options.autoload_slot);
  pl_ini_set_int(&file, "System", "Joystick Port", psp_options.joyport);
  pl_ini_set_int(&file, "Video", "Display Mode", psp_options.display_mode);
  pl_ini_set_int(&file, "Video", "PSP Clock Frequency", psp_options.clock_freq);
  pl_ini_set_int(&file, "Video", "Show FPS", psp_options.show_fps);
  pl_ini_set_int(&file, "Menu", "Control Mode", psp_options.control_mode);
  pl_ini_set_int(&file, "Menu", "Animate", psp_options.animate_menu);
  pl_ini_set_int(&file, "Input", "VK Mode", psp_options.toggle_vk);
  pl_ini_set_string(&file, "File", "Game Path", psp_game_path);

  int status = pl_ini_save(&file, path);
  pl_ini_destroy(&file);

  return status;
}

static const char *prepare_file(const char *path)
{
  const char *game_path = path;
  void *file_buffer = NULL;
  int file_size = 0;

  if (pl_file_is_of_type(path, "ZIP"))
  {
    pspUiFlashMessage("Loading compressed file, please wait...");

    char archived_file[512];
    unzFile zipfile = NULL;
    unz_global_info gi;
    unz_file_info fi;

    /* Open archive for reading */
    if (!(zipfile = unzOpen(path)))
      return NULL;

    /* Get global ZIP file information */
    if (unzGetGlobalInfo(zipfile, &gi) != UNZ_OK)
    {
      unzClose(zipfile);
      return NULL;
    }

    const char *extension;
    int i, j;

    for (i = 0; i < (int)gi.number_entry; i++)
    {
      /* Get name of the archived file */
      if (unzGetCurrentFileInfo(zipfile, &fi, archived_file, 
          sizeof(archived_file), NULL, 0, NULL, 0) != UNZ_OK)
      {
        unzClose(zipfile);
        return NULL;
      }

      extension = pl_file_get_extension(archived_file);
      for (j = 1; QuickloadFilter[j]; j++)
      {
        if (strcasecmp(QuickloadFilter[j], extension) == 0)
        {
          file_size = fi.uncompressed_size;

          /* Open archived file for reading */
          if(unzOpenCurrentFile(zipfile) != UNZ_OK)
          {
            unzClose(zipfile); 
            return NULL;
          }

          if (!(file_buffer = malloc(file_size)))
          {
            unzCloseCurrentFile(zipfile);
            unzClose(zipfile); 
            return NULL;
          }

          unzReadCurrentFile(zipfile, file_buffer, file_size);
          unzCloseCurrentFile(zipfile);

          goto close_archive;
        }
      }

      /* Go to the next file in the archive */
      if (i + 1 < (int)gi.number_entry)
      {
        if (unzGoToNextFile(zipfile) != UNZ_OK)
        {
          unzClose(zipfile);
          return NULL;
        }
      }
    }

    /* No eligible files */
    return NULL;

close_archive:
    unzClose(zipfile);

    /* Remove temp. file (if any) */
    if (*psp_tmp_file && pl_file_exists(psp_tmp_file))
      pl_file_rm(psp_tmp_file);

    /* Define temp filename */
    sprintf(psp_tmp_file, "%st%04i_%s", 
      pl_psp_get_app_directory(), rand() % 10000, archived_file);

    /* Write file to stick */
    FILE *file = fopen(psp_tmp_file, "w");
    if (!file)
    {
      *psp_tmp_file = '\0';
      return NULL;
    }
    if (fwrite(file_buffer, 1, file_size, file) < file_size)
    {
      fclose(file);
      *psp_tmp_file = '\0';
      return NULL;
    }
    fclose(file);

    game_path = psp_tmp_file;
  }

  return game_path;
}

static int psp_load_game(const char *path)
{
  const char *game_path = prepare_file(path);

  if (!game_path)
  {
    pspUiAlert("Error loading compressed file");
    return 0;
  }

  /* Eject all cartridges, tapes & disks */
  cartridge_detach_image();
  tape_image_detach(1);
  file_system_detach_disk(GET_DRIVE(8));

  if (pl_file_is_of_type(game_path, "CRT"))
    return !cartridge_attach_image(CARTRIDGE_CRT, game_path);

  return !autostart_autodetect(game_path, NULL, 0, AUTOSTART_MODE_RUN);
}

void psp_display_menu()
{
  pl_menu_item *item;
  psp_exit_menu = 0;

  /* Set normal clock frequency */
  pl_psp_set_clock_freq(222);
  /* Set buttons to autorepeat */
  pspCtrlSetPollingMode(PSP_CTRL_AUTOREPEAT);

  /* Menu loop */
  while (!ExitPSP && !psp_exit_menu)
  {
    /* Display appropriate tab */
    switch (psp_tab_index)
    {
    case TAB_QUICKLOAD:
      pspUiOpenBrowser(&QuickloadBrowser, 
        (GAME_LOADED) ? psp_current_game 
          : ((psp_game_path[0]) ? psp_game_path : NULL));
      break;
    case TAB_CONTROLS:
      psp_display_control_tab();
      break;
    case TAB_OPTIONS:
      item = pl_menu_find_item_by_id(&OptionUiMenu.Menu, OPTION_DISPLAY_MODE);
      pl_menu_select_option_by_value(item, (void*)(int)psp_options.display_mode);
      item = pl_menu_find_item_by_id(&OptionUiMenu.Menu, OPTION_CLOCK_FREQ);
      pl_menu_select_option_by_value(item, (void*)(int)psp_options.clock_freq);
      item = pl_menu_find_item_by_id(&OptionUiMenu.Menu, OPTION_SHOW_FPS);
      pl_menu_select_option_by_value(item, (void*)(int)psp_options.show_fps);
      item = pl_menu_find_item_by_id(&OptionUiMenu.Menu, OPTION_CONTROL_MODE);
      pl_menu_select_option_by_value(item, (void*)(int)psp_options.control_mode);
      item = pl_menu_find_item_by_id(&OptionUiMenu.Menu, OPTION_ANIMATE);
      pl_menu_select_option_by_value(item, (void*)(int)psp_options.animate_menu);
      item = pl_menu_find_item_by_id(&OptionUiMenu.Menu, OPTION_TOGGLE_VK);
      pl_menu_select_option_by_value(item, (void*)(int)psp_options.toggle_vk);

      pspUiOpenMenu(&OptionUiMenu, NULL);
      break;
    case TAB_STATE:
      psp_display_state_tab();
      break;
    case TAB_SYSTEM:
      psp_display_system_tab();
      break;
    case TAB_ABOUT:
      pspUiSplashScreen(&SplashScreen);
      break;
    }
  }

  if (!ExitPSP)
  {
    /* Set clock frequency during emulation */
    pl_psp_set_clock_freq(psp_options.clock_freq);
    /* Set buttons to normal mode */
    pspCtrlSetPollingMode(PSP_CTRL_NORMAL);

    if (psp_options.animate_menu)
      pspUiFadeout();
  }
}

static void psp_display_state_tab()
{
  pl_menu_item *item, *sel = NULL;
  SceIoStat stat;
  ScePspDateTime latest;
  char caption[32];
  const char *config_name = (GAME_LOADED)
    ? pl_file_get_filename(psp_current_game) : "BASIC";
  char *path = (char*)malloc(strlen(psp_save_state_path) + strlen(config_name) + 8);
  char *game_name = strdup(config_name);
  char *dot = strrchr(game_name, '.');
  if (dot) *dot='\0';

  memset(&latest,0,sizeof(latest));

  /* Initialize icons */
  for (item = SaveStateGallery.Menu.items; item; item = item->next)
  {
    sprintf(path, "%s%s_%02i.sta", psp_save_state_path, config_name, item->id);

    if (pl_file_exists(path))
    {
      if (sceIoGetstat(path, &stat) < 0)
        sprintf(caption, "ERROR");
      else
      {
        /* Determine the latest save state */
        if (pl_util_date_compare(&latest, &stat.st_mtime) < 0)
        {
          sel = item;
          latest = stat.st_mtime;
        }

        sprintf(caption, "%02i/%02i/%02i %02i:%02i%s", 
          stat.st_mtime.month,
          stat.st_mtime.day,
          stat.st_mtime.year - (stat.st_mtime.year / 100) * 100,
          stat.st_mtime.hour,
          stat.st_mtime.minute,
          ((int)item->id == psp_options.autoload_slot) ? "*" : "");
      }

      pl_menu_set_item_caption(item, caption);
      item->param = psp_load_state_icon(path);
      pl_menu_set_item_help_text(item, PresentSlotText);
    }
    else
    {
      pl_menu_set_item_caption(item, ((int)item->id == psp_options.autoload_slot)
          ? "Autoload" : "Empty");
      item->param = psp_blank_ss_icon;
      pl_menu_set_item_help_text(item, EmptySlotText);
    }
  }

  free(path);

  /* Highlight the latest save state if none are selected */
  if (SaveStateGallery.Menu.selected == NULL)
    SaveStateGallery.Menu.selected = sel;

  pspUiOpenGallery(&SaveStateGallery, game_name);
  free(game_name);

  /* Destroy any icons */
  for (item = SaveStateGallery.Menu.items; item; item = item->next)
    if (item->param != NULL && item->param != psp_blank_ss_icon)
      pspImageDestroy((PspImage*)item->param);
}

/**************************/
/* psplib event callbacks */
/**************************/
static int OnGenericCancel(const void *uiobject, const void* param)
{
  psp_exit_menu = 1;
  return 1;
}

static void OnGenericRender(const void *uiobject, const void *item_obj)
{
  int height = pspFontGetLineHeight(UiMetric.Font);
  int width;

  /* Draw tabs */
  int i, x;
  for (i = 0, x = 5; i <= TAB_MAX; i++, x += width + 10)
  {
    /* Determine width of text */
    width = pspFontGetTextWidth(UiMetric.Font, TabLabel[i]);

    /* Draw background of active tab */
    if (i == psp_tab_index) 
      pspVideoFillRect(x - 5, 0, x + width + 5, height + 1, 
        UiMetric.TabBgColor);

    /* Draw name of tab */
    pspVideoPrint(UiMetric.Font, x, 0, TabLabel[i], PSP_COLOR_WHITE);
  }
}

static int OnGenericButtonPress(const PspUiFileBrowser *browser,
  const char *path, u32 button_mask)
{
  /* If L or R are pressed, switch tabs */
  if (button_mask & PSP_CTRL_LTRIGGER)
  { if (--psp_tab_index < 0) psp_tab_index = TAB_MAX; }
  else if (button_mask & PSP_CTRL_RTRIGGER)
  { if (++psp_tab_index > TAB_MAX) psp_tab_index = 0; }
  else if ((button_mask & (PSP_CTRL_START | PSP_CTRL_SELECT)) 
    == (PSP_CTRL_START | PSP_CTRL_SELECT))
  {
    if (pl_util_save_vram_seq(psp_screenshot_path, "UI"))
      pspUiAlert("Saved successfully");
    else
      pspUiAlert("ERROR: Not saved");
    return 0;
  }
  else return 0;

  return 1;
}

static void OnSplashRender(const void *splash, const void *null)
{
  int fh, i, x, y, height;
  const char *lines[] = 
  { 
    PSP_APP_NAME" version "PSP_APP_VER" ("__DATE__")",
    "\026http://psp.akop.org/vice",
    " ",
    "2009 Akop Karapetyan",
    "1998-2009 VICE team",
    NULL
  };

  fh = pspFontGetLineHeight(UiMetric.Font);

  for (i = 0; lines[i]; i++);
  height = fh * (i - 1);

  /* Render lines */
  for (i = 0, y = SCR_HEIGHT / 2 - height / 2; lines[i]; i++, y += fh)
  {
    x = SCR_WIDTH / 2 - pspFontGetTextWidth(UiMetric.Font, lines[i]) / 2;
    pspVideoPrint(UiMetric.Font, x, y, lines[i], PSP_COLOR_GRAY);
  }

  /* Render PSP status */
  OnGenericRender(splash, null);
}

static int OnSplashButtonPress(const struct PspUiSplash *splash, 
                               u32 button_mask)
{
  return OnGenericButtonPress(NULL, NULL, button_mask);
}

static const char* OnSplashGetStatusBarText(const struct PspUiSplash *splash)
{
  return "\026\255\020/\026\256\020 Switch tabs";
}

static int OnMenuOk(const void *uimenu, const void* sel_item)
{
  switch (((const pl_menu_item*)sel_item)->id)
  {
  case SYSTEM_CART:
    FileBrowser.Userdata = (void*)0; /* Cartridge */
    FileBrowser.Filter = CartFilter;
    pspUiOpenBrowser(&FileBrowser, (*psp_game_path) ? psp_game_path : NULL);
    break;
  case SYSTEM_TAPE:
    FileBrowser.Userdata = (void*)1; /* Tape */
    FileBrowser.Filter = TapeFilter;
    pspUiOpenBrowser(&FileBrowser, (*psp_game_path) ? psp_game_path : NULL);
    psp_refresh_devices();
    break;
  case SYSTEM_DRIVE8:
    FileBrowser.Userdata = (void*)GET_DRIVE(((const pl_menu_item*)sel_item)->id);
    FileBrowser.Filter = DiskFilter;
    pspUiOpenBrowser(&FileBrowser, (*psp_game_path) ? psp_game_path : NULL);
    psp_refresh_devices();
    break;
  case SYSTEM_RESET:
    if (pspUiConfirm("Reset the system?"))
    {
      psp_exit_menu = 1;
      machine_trigger_reset(MACHINE_RESET_MODE_SOFT);
      return 1;
    }
    break;
  case SYSTEM_SCRNSHOT:
    /* Save screenshot */
    if (!pl_util_save_image_seq(psp_screenshot_path, (GAME_LOADED)
                                ? pl_file_get_filename(psp_current_game) : "BASIC",
                                Screen))
      pspUiAlert("ERROR: Screenshot not saved");
    else
      pspUiAlert("Screenshot saved successfully");
    break;
  }

  return 0;
}

static int OnMenuButtonPress(const struct PspUiMenu *uimenu,
                             pl_menu_item* sel_item,
                             u32 button_mask)
{
  if (uimenu == &ControlUiMenu)
  {
    if (button_mask & PSP_CTRL_SQUARE)
    {
      /* Save to MS as default mapping */
      if (psp_save_controls("DEFAULT", &current_map))
      {
        /* Modify in-memory defaults */
        memcpy(&default_map, &current_map, sizeof(psp_ctrl_map_t));
        pspUiAlert("Changes saved");
      }
      else
        pspUiAlert("ERROR: Changes not saved");

      return 0;
    }
    else if (button_mask & PSP_CTRL_TRIANGLE)
    {
      pl_menu_item *item;
      int i;

      /* Load default mapping */
      memcpy(&current_map, &default_map, sizeof(psp_ctrl_map_t));

      /* Modify the menu */
      for (item = ControlUiMenu.Menu.items, i = 0; item; item = item->next, i++)
        pl_menu_select_option_by_value(item, (void*)default_map.button_map[i]);

      return 0;
    }
  }
  else
  {
    int id = ((const pl_menu_item*)sel_item)->id;
    if (button_mask & PSP_CTRL_TRIANGLE)
    {
      switch (id)
      {
      case SYSTEM_CART:
        cartridge_detach_image();
        break;
      case SYSTEM_TAPE:
        tape_image_detach(1);
        psp_refresh_devices();
        break;
      case SYSTEM_DRIVE8:
        file_system_detach_disk(GET_DRIVE(id));
        psp_refresh_devices();
        break;
      }
    }
  }

  return OnGenericButtonPress(NULL, NULL, button_mask);
}

static int OnMenuItemChanged(const struct PspUiMenu *uimenu,
                             pl_menu_item* item,
                             const pl_menu_option* option)
{
  if (uimenu == &ControlUiMenu)
  {
    current_map.button_map[item->id] = (unsigned int)option->value;
    psp_controls_changed = 1;
  }
  else
  {
    switch((int)item->id)
    {
    case OPTION_DISPLAY_MODE:
      psp_options.display_mode = (int)option->value;
      break;
    case OPTION_CLOCK_FREQ:
      psp_options.clock_freq = (int)option->value;
      break;
    case OPTION_SHOW_FPS:
      psp_options.show_fps = (int)option->value;
      break;
    case OPTION_TOGGLE_VK:
      psp_options.toggle_vk = (int)option->value;
      break;
    case OPTION_CONTROL_MODE:
      psp_options.control_mode = (int)option->value;
      UiMetric.OkButton = (!psp_options.control_mode) 
                          ? PSP_CTRL_CROSS : PSP_CTRL_CIRCLE;
      UiMetric.CancelButton = (!psp_options.control_mode) 
                              ? PSP_CTRL_CIRCLE : PSP_CTRL_CROSS;
      break;
    case OPTION_ANIMATE:
      psp_options.animate_menu = (int)option->value;
      UiMetric.Animate = psp_options.animate_menu;
      break;
    case SYSTEM_TAPE:
      {
        int index = pl_menu_get_option_index(item, option);
        const char *name = tape_image_dev1->name; /* Filename */

        pspUiFlashMessage("Autoloading, please wait...");

        if (autostart_autodetect(name, NULL, index, AUTOSTART_MODE_RUN))
        {
          pspUiAlert("Error loading program");
          return 0;
        }
      }
      break;
    case SYSTEM_DRIVE8:
      {
        int index = pl_menu_get_option_index(item, option);
        const char *name = file_system_get_disk_name(GET_DRIVE((int)item->id));

        pspUiFlashMessage("Autoloading, please wait...");

        if (autostart_autodetect(name, NULL, index, AUTOSTART_MODE_RUN))
        {
          pspUiAlert("Error loading program");
          return 0;
        }
      }
      break;
    }
  }

  return 1;
}

static void OnSystemRender(const void *uiobject, const void *item_obj)
{
  int w, h, x, y;
  w = Screen->Viewport.Width >> 1;
  h = Screen->Viewport.Height >> 1;
  x = UiMetric.Right - w - UiMetric.ScrollbarWidth;
  y = SCR_HEIGHT - h - 80;

  /* Draw a small representation of the screen */
  pspVideoShadowRect(x, y, x + w - 1, y + h - 1, PSP_COLOR_BLACK, 3);
  pl_gfx_put_image(Screen, x, y, w, h);
  pspVideoDrawRect(x, y, x + w - 1, y + h - 1, PSP_COLOR_GRAY);

  OnGenericRender(uiobject, item_obj);
}

static int OnFileOk(const void *browser, const void *path)
{
  int drive_num = (int)((const PspUiFileBrowser*)browser)->Userdata;
  const char *game_path;
  int set_as_current = 0;

  if (!(game_path = prepare_file(path)))
  {
    pspUiAlert("Error loading file");
    return 0;
  }

  if (drive_num == 0) /* Cart */
  {
    if (cartridge_attach_image(CARTRIDGE_CRT, game_path) < 0)
    {
      pspUiAlert("Error loading cartridge");
      return 0;
    }

    set_as_current = 1;
  }
  else if (drive_num == 1) /* Tape */
  {
    if (tape_image_attach(1, game_path) < 0)
    {
      pspUiAlert("Error loading tape image");
      return 0;
    }

    set_as_current = 1;
  }
  else /* Disk */
  {
    if (file_system_attach_disk(drive_num, game_path) < 0)
    {
      pspUiAlert("Error loading disk image");
      return 0;
    }

    /* If disk 8, set as currently loaded game */
    set_as_current = (drive_num == 8);
  }

  if (set_as_current)
  {
    /* Set as currently loaded game */
    SET_AS_CURRENT_GAME(path);
    pl_file_get_parent_directory(path, psp_game_path, sizeof(psp_game_path));
    psp_load_controls(pl_file_get_filename(psp_current_game), &current_map);

    /* Reset selected state */
    SaveStateGallery.Menu.selected = NULL;
  }

  return 1;
}

static int OnQuickloadOk(const void *browser, const void *path)
{
  if (!psp_load_game(path))
  {
    pspUiAlert("Error loading file");
    return 0;
  }

  psp_exit_menu = 1;

  SET_AS_CURRENT_GAME(path);
  pl_file_get_parent_directory(path,
                               psp_game_path,
                               sizeof(psp_game_path));
  if (!psp_load_controls((GAME_LOADED)
                           ? pl_file_get_filename(psp_current_game) : "BASIC",
                         &current_map));

  /* Autoload saved state */
  if (psp_options.autoload_slot >= 0)
  {
    const char *config_name = (GAME_LOADED)
                              ? pl_file_get_filename(psp_current_game) : "BASIC";
    pl_file_path state_file;
    snprintf(state_file, sizeof(state_file) - 1, 
             "%s%s_%02i.sta", psp_save_state_path, config_name, 
             psp_options.autoload_slot);

    /* Attempt loading saved state (don't care if fails) */
    psp_load_state(state_file);
  }

  /* Reset selected state */
  SaveStateGallery.Menu.selected = NULL;

  return 1;
}

static int OnSaveStateOk(const void *gallery, const void *item)
{
  char *path;
  const char *config_name = (GAME_LOADED) 
    ? pl_file_get_filename(psp_current_game) : "BASIC";

  path = (char*)malloc(strlen(psp_save_state_path) + strlen(config_name) + 8);
  sprintf(path, "%s%s_%02i.sta", psp_save_state_path, config_name,
    ((const pl_menu_item*)item)->id);

  if (pl_file_exists(path) && pspUiConfirm("Load state?"))
  {
    if (psp_load_state(path))
    {
      psp_exit_menu = 1;
      pl_menu_find_item_by_id(&((PspUiGallery*)gallery)->Menu,
        ((pl_menu_item*)item)->id);
      free(path);

      return 1;
    }

    pspUiAlert("ERROR: State failed to load\nSee documentation for possible reasons");
  }

  free(path);
  return 0;
}

static int OnSaveStateButtonPress(const PspUiGallery *gallery, 
                                  pl_menu_item *sel,
                                  u32 button_mask)
{
  if (button_mask & PSP_CTRL_SQUARE 
    || button_mask & PSP_CTRL_TRIANGLE
    || button_mask & PSP_CTRL_START)
  {
    char *path;
    char caption[32];
    const char *config_name = (GAME_LOADED) 
      ? pl_file_get_filename(psp_current_game) : "BASIC";
    pl_menu_item *item = pl_menu_find_item_by_id(&gallery->Menu, sel->id);

    path = (char*)malloc(strlen(psp_save_state_path) + strlen(config_name) + 8);
    sprintf(path, "%s%s_%02i.sta", psp_save_state_path, config_name, item->id);

    do /* not a real loop; flow control construct */
    {
      if (button_mask & PSP_CTRL_SQUARE)
      {
        if (pl_file_exists(path) && !pspUiConfirm("Overwrite existing state?"))
          break;

        pspUiFlashMessage("Saving, please wait ...");

        PspImage *icon;
        if (!(icon = psp_save_state(path, Screen)))
        {
          pspUiAlert("ERROR: State not saved");
          break;
        }

        SceIoStat stat;

        /* Trash the old icon (if any) */
        if (item->param && item->param != psp_blank_ss_icon)
          pspImageDestroy((PspImage*)item->param);

        /* Update icon, help text */
        item->param = icon;
        pl_menu_set_item_help_text(item, PresentSlotText);

        /* Get file modification time/date */
        if (sceIoGetstat(path, &stat) < 0)
          sprintf(caption, "ERROR");
        else
          sprintf(caption, "%02i/%02i/%02i %02i:%02i", 
            stat.st_mtime.month,
            stat.st_mtime.day,
            stat.st_mtime.year - (stat.st_mtime.year / 100) * 100,
            stat.st_mtime.hour,
            stat.st_mtime.minute);

        pl_menu_set_item_caption(item, caption);
      }
      else if (button_mask & PSP_CTRL_TRIANGLE)
      {
        if (!pl_file_exists(path) || !pspUiConfirm("Delete state?"))
          break;

        if (!pl_file_rm(path))
        {
          pspUiAlert("ERROR: State not deleted");
          break;
        }

        /* Trash the old icon (if any) */
        if (item->param && item->param != psp_blank_ss_icon)
          pspImageDestroy((PspImage*)item->param);

        /* Update icon, caption */
        item->param = psp_blank_ss_icon;
        pl_menu_set_item_help_text(item, EmptySlotText);
        pl_menu_set_item_caption(item, ((int)item->id == psp_options.autoload_slot)
            ? "Autoload" : "Empty");
      }
    } while (0);

    if (path) free(path);
    return 0;
  }

  return OnGenericButtonPress(NULL, NULL, button_mask);
}

