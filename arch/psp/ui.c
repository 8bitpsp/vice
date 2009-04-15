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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lib/video.h"
#include "lib/ui.h"
#include "lib/pl_menu.h"
#include "lib/pl_file.h"
#include "lib/ctrl.h"
#include "lib/pl_psp.h"
#include "lib/pl_ini.h"
#include "lib/pl_util.h"

#define TAB_QUICKLOAD 0
#define TAB_OPTIONS   1
#define TAB_SYSTEM    2
#define TAB_MAX       TAB_SYSTEM
#define TAB_ABOUT     3
/*
#define TAB_STATE     1
#define TAB_CONTROLS  2
#define TAB_OPTIONS   3
#define TAB_SYSTEM    4
#define TAB_MAX       TAB_SYSTEM
#define TAB_ABOUT     5
*/

#define OPTION_DISPLAY_MODE  0x01
#define OPTION_FRAME_LIMITER 0x02
#define OPTION_CLOCK_FREQ    0x03
#define OPTION_SHOW_FPS      0x04
#define OPTION_CONTROL_MODE  0x05
#define OPTION_ANIMATE       0x06
#define OPTION_TOGGLE_VK     0x08

#define SYSTEM_SCRNSHOT     0x11
#define SYSTEM_RESET        0x12

static const char *QuickloadFilter[] =
      { "D64", "ZIP", '\0' }; /* TODO: add all */

/* Tab labels */
static const char *TabLabel[] = 
{
  "Game",
  "Options",
  "System",
/*
  "Save/Load",
  "Controls",
  "Options",
  "System",
*/
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
PL_MENU_OPTIONS_BEGIN(ControlModeOptions)
  PL_MENU_OPTION("\026\242\020 cancels, \026\241\020 confirms (US)", 0)
  PL_MENU_OPTION("\026\241\020 cancels, \026\242\020 confirms (Japan)", 1)
PL_MENU_OPTIONS_END
PL_MENU_OPTIONS_BEGIN(VkModeOptions)
  PL_MENU_OPTION("Display when button is held down (classic mode)", 0)
  PL_MENU_OPTION("Toggle display on and off when button is pressed", 1)
PL_MENU_OPTIONS_END

PL_MENU_ITEMS_BEGIN(SystemMenuDef)
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

PspUiSplash SplashScreen = 
{
  OnSplashRender,
  OnGenericCancel,
  OnSplashButtonPress,
  OnSplashGetStatusBarText
};
PspUiFileBrowser QuickloadBrowser = 
{
  OnGenericRender,
  OnQuickloadOk,
  OnGenericCancel,
  OnGenericButtonPress,
  QuickloadFilter,
  0
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

pl_file_path psp_current_game = {'\0'},
             psp_game_path = {'\0'},
             psp_save_state_path,
             psp_screenshot_path,
             psp_config_path;
psp_options_t psp_options;

#define CURRENT_GAME (psp_current_game)
#define GAME_LOADED (psp_current_game[0] != '\0')
#define SET_AS_CURRENT_GAME(filename) \
  strncpy(psp_current_game, filename, sizeof(psp_current_game) - 1)

static int psp_tab_index;
static PspImage *psp_menu_bg;
static int psp_exit_menu;
extern PspImage *Screen;

static void psp_load_options();
static int  psp_save_options();

int ui_init(int *argc, char **argv)
{
  /* Initialize paths */
  sprintf(psp_save_state_path, "%sstates/", pl_psp_get_app_directory());
  sprintf(psp_screenshot_path, "ms0:/PSP/PHOTO/%s/", PSP_APP_NAME);
  sprintf(psp_config_path, "%sconfig/", pl_psp_get_app_directory());

  /* Initialize menus */
  pl_menu_create(&OptionUiMenu.Menu, OptionMenuDef);
  pl_menu_create(&SystemUiMenu.Menu, SystemMenuDef);

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
  UiMetric.SelectedBgColor = COLOR(0x46,0x98,0xce,0x99);
  UiMetric.StatusBarColor = PSP_COLOR_WHITE;
  UiMetric.BrowserFileColor = PSP_COLOR_GRAY;
  UiMetric.BrowserDirectoryColor = PSP_COLOR_YELLOW;
  UiMetric.GalleryIconsPerRow = 5;
  UiMetric.GalleryIconMarginWidth = 8;
  UiMetric.MenuItemMargin = 20;
  UiMetric.MenuSelOptionBg = PSP_COLOR_BLACK;
  UiMetric.MenuOptionBoxColor = PSP_COLOR_GRAY;
  UiMetric.MenuOptionBoxBg = COLOR(0x46,0x98,0xce,0xCC);
  UiMetric.MenuDecorColor = UiMetric.SelectedColor;
  UiMetric.DialogFogColor = COLOR(0x59,0x91,0x38,0xBB);
  UiMetric.TitlePadding = 4;
  UiMetric.TitleColor = PSP_COLOR_WHITE;
  UiMetric.MenuFps = 30;
  UiMetric.TabBgColor = COLOR(0xa4,0xa4,0xa4,0xff);
  UiMetric.Animate = psp_options.animate_menu;
  UiMetric.BrowserScreenshotPath = psp_screenshot_path;
  UiMetric.BrowserScreenshotDelay = 30;

  psp_tab_index = TAB_ABOUT;

  return 0;
}

void ui_shutdown()
{
  if (psp_menu_bg) pspImageDestroy(psp_menu_bg);

  pl_menu_destroy(&OptionUiMenu.Menu);
  pl_menu_destroy(&SystemUiMenu.Menu);

  psp_save_options();
}

/**************************/
/* Helper functions       */
/**************************/
static void psp_load_options()
{
  pl_file_path path;
  snprintf(path, sizeof(path) - 1, "%s%s", pl_psp_get_app_directory(), "options.ini");

  /* Load INI */
  pl_ini_file file;
  pl_ini_load(&file, path);

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

static int psp_load_game(const char *path)
{
  return !autostart_autodetect(path, NULL, 0, AUTOSTART_MODE_RUN);
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
#if 0 //TODO
    case TAB_CONTROLS:
      psp_display_control_tab();
      break;
#endif
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
#if 0
    case TAB_STATE:
      psp_display_state_tab();
      break;
#endif
    case TAB_SYSTEM:
      pspUiOpenMenu(&SystemUiMenu, NULL);
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
    "     2009 Akop Karapetyan",
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
  if (uimenu == &ControlUiMenu)
  {
#if 0 // TODO
    /* Save to MS */
    if (psp_save_controls((GAME_LOADED)
                          ? pl_file_get_filename(psp_current_game) : "BASIC", 
                          &current_map))
      pspUiAlert("Changes saved");
    else
      pspUiAlert("ERROR: Changes not saved");
#endif
  }
  else
  {
    switch (((const pl_menu_item*)sel_item)->id)
    {
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
  }

  return 0;
}

static int OnMenuButtonPress(const struct PspUiMenu *uimenu,
                             pl_menu_item* sel_item,
                             u32 button_mask)
{
  if (uimenu == &ControlUiMenu)
  {
#if 0 // TODO
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
#endif
  }

  return OnGenericButtonPress(NULL, NULL, button_mask);
}

static int OnMenuItemChanged(const struct PspUiMenu *uimenu,
                             pl_menu_item* item,
                             const pl_menu_option* option)
{
  if (uimenu == &ControlUiMenu)
  {
#if 0 // TODO
    current_map.button_map[item->id] = (unsigned int)option->value;
#endif
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
      UiMetric.OkButton = (!psp_options.control_mode) ? PSP_CTRL_CROSS : PSP_CTRL_CIRCLE;
      UiMetric.CancelButton = (!psp_options.control_mode) ? PSP_CTRL_CIRCLE : PSP_CTRL_CROSS;
      break;
    case OPTION_ANIMATE:
      psp_options.animate_menu = (int)option->value;
      UiMetric.Animate = psp_options.animate_menu;
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
  y = SCR_HEIGHT - h - 56;

  /* Draw a small representation of the screen */
  pspVideoShadowRect(x, y, x + w - 1, y + h - 1, PSP_COLOR_BLACK, 3);
  pspVideoPutImage(Screen, x, y, w, h);
  pspVideoDrawRect(x, y, x + w - 1, y + h - 1, PSP_COLOR_GRAY);

  OnGenericRender(uiobject, item_obj);
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
#if 0 // TODO
  if (!psp_load_controls((GAME_LOADED)
                           ? pl_file_get_filename(psp_current_game) : "BASIC",
                         &current_map));
#endif

  /* Reset selected state */
  //TODO: SaveStateGallery.Menu.selected = NULL;

  return 1;
}

