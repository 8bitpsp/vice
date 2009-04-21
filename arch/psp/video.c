#include "vice.h"
#include "cmdline.h"
#include "video.h"
#include "videoarch.h"
#include "palette.h"
#include "viewport.h"
#include "keyboard.h"
#include "lib.h"
#include "log.h"
#include "ui.h"
#include "vsync.h"
#include "raster.h"

#include "lib/video.h"
#include "lib/ctrl.h"
#include "lib/pl_gfx.h"
#include "lib/pl_vk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PspImage *Screen = NULL;
static float last_framerate = 0;
static float last_percent = 0;
static int screen_x;
static int screen_y;
static int screen_w;
static int screen_h;
static int clear_screen;
static int line_height;
static int psp_first_time = 1;

static int show_kybd_held;
static int keyboard_visible;
static pl_vk_layout psp_keyboard;

static void video_psp_display_menu();
static void video_psp_refresh_screen();
static inline void psp_keyboard_toggle(unsigned int code, int on);

typedef struct psp_ctrl_mask_to_index_map
{
  uint64_t mask;
  uint8_t  index;
} psp_ctrl_mask_to_index_map_t;

static const psp_ctrl_mask_to_index_map_t physical_to_emulated_button_map[] =
{
  /* These are shift-based (e.g. L/R are not unset when a button pressed) */
  { PSP_CTRL_LTRIGGER | PSP_CTRL_SELECT,   18 },
  { PSP_CTRL_RTRIGGER | PSP_CTRL_SELECT,   19 },
  { PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE,   20 },
  { PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS,    21 },
  { PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE,   22 },
  { PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE, 23 },
  { PSP_CTRL_RTRIGGER | PSP_CTRL_SQUARE,   24 },
  { PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS,    25 },
  { PSP_CTRL_RTRIGGER | PSP_CTRL_CIRCLE,   26 },
  { PSP_CTRL_RTRIGGER | PSP_CTRL_TRIANGLE, 27 },

  /* These are normal */
  { PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER, 16 },
  { PSP_CTRL_START    | PSP_CTRL_SELECT,   17 },
  { PSP_CTRL_ANALUP,   0 }, { PSP_CTRL_ANALDOWN,  1 },
  { PSP_CTRL_ANALLEFT, 2 }, { PSP_CTRL_ANALRIGHT, 3 },
  { PSP_CTRL_UP,   4 }, { PSP_CTRL_DOWN,  5 },
  { PSP_CTRL_LEFT, 6 }, { PSP_CTRL_RIGHT, 7 },
  { PSP_CTRL_SQUARE, 8 },  { PSP_CTRL_CROSS,     9 },
  { PSP_CTRL_CIRCLE, 10 }, { PSP_CTRL_TRIANGLE, 11 },
  { PSP_CTRL_LTRIGGER, 12 }, { PSP_CTRL_RTRIGGER, 13 },
  { PSP_CTRL_SELECT, 14 }, { PSP_CTRL_START, 15 },
  { 0, -1 }
};

void video_canvas_resize(struct video_canvas_s *canvas,
                                unsigned int width, unsigned int height)
{
  canvas->width = width;
  canvas->height = height;
}

video_canvas_t *video_canvas_create(video_canvas_t *canvas, 
    unsigned int *width, unsigned int *height, int mapped)
{
  canvas->depth = Screen->Depth;
  canvas->width = *width;
  canvas->height = *height;

  video_canvas_set_palette(canvas, canvas->palette);

  return canvas;
}

void video_canvas_destroy(struct video_canvas_s *canvas)
{
  if (Screen) { pspImageDestroy(Screen); Screen = NULL; }
  lib_free(canvas->video_draw_buffer_callback);
}

static int video_frame_buffer_alloc(video_canvas_t *canvas, 
                                    BYTE **draw_buffer, 
                                    unsigned int fb_width, 
                                    unsigned int fb_height, 
                                    unsigned int *fb_pitch);
static void video_frame_buffer_free(video_canvas_t *canvas, 
                                    BYTE *draw_buffer);
static void video_frame_buffer_clear(video_canvas_t *canvas, 
                                     BYTE *draw_buffer, 
                                     BYTE value, 
                                     unsigned int fb_width, 
                                     unsigned int fb_height, 
                                     unsigned int fb_pitch);

void video_arch_canvas_init(struct video_canvas_s *canvas)
{
  canvas->video_draw_buffer_callback
        = lib_malloc(sizeof(video_draw_buffer_callback_t));
  canvas->video_draw_buffer_callback->draw_buffer_alloc
        = video_frame_buffer_alloc;
  canvas->video_draw_buffer_callback->draw_buffer_free
        = video_frame_buffer_free;
  canvas->video_draw_buffer_callback->draw_buffer_clear
        = video_frame_buffer_clear;
}

static int video_frame_buffer_alloc(video_canvas_t *canvas, 
                                    BYTE **draw_buffer, 
                                    unsigned int fb_width, 
                                    unsigned int fb_height, 
                                    unsigned int *fb_pitch)
{
  if (!Screen)
  {
    if (!(Screen = pspImageCreateVram(512, fb_height, PSP_IMAGE_INDEXED)))
      return -1;

    Screen->Viewport.Y = 16; /* TODO: determine these values properly */
    Screen->Viewport.Height = 272;
    Screen->Viewport.Width = 384;
  }

  *fb_pitch = (Screen->Depth / 8) * Screen->Width;
  *draw_buffer = Screen->Pixels;

  return 0;
}

static void video_frame_buffer_free(video_canvas_t *canvas, 
                                    BYTE *draw_buffer)
{
}

static void video_frame_buffer_clear(video_canvas_t *canvas, 
                                     BYTE *draw_buffer, 
                                     BYTE value, 
                                     unsigned int fb_width, 
                                     unsigned int fb_height, 
                                     unsigned int fb_pitch)
{
  memset(draw_buffer, value, fb_pitch * fb_height);
}


int video_canvas_set_palette(struct video_canvas_s *canvas,
                             struct palette_s *palette)
{
  int i;
  for(i = 0; i < palette->num_entries; i++)
    Screen->Palette[i] = RGB(
      palette->entries[i].red,
      palette->entries[i].green,
      palette->entries[i].blue);

  return 0;
}

static void video_psp_display_menu()
{ 
  vsync_suspend_speed_eval();
  psp_display_menu();
  vsync_sync_reset();

  last_framerate = 0;
  last_percent = 0;

  /* Set up viewing ratios */
  float ratio;
  switch (psp_options.display_mode)
  {
  default:
  case DISPLAY_MODE_UNSCALED:
    screen_w = Screen->Viewport.Width;
    screen_h = Screen->Viewport.Height;
    break;
  case DISPLAY_MODE_FIT_HEIGHT:
    ratio = (float)PL_GFX_SCREEN_HEIGHT / (float)Screen->Viewport.Height;
    screen_w = (float)Screen->Viewport.Width * ratio - 2;
    screen_h = PL_GFX_SCREEN_HEIGHT;
    break;
  case DISPLAY_MODE_FILL_SCREEN:
    screen_w = PL_GFX_SCREEN_WIDTH - 3;
    screen_h = PL_GFX_SCREEN_HEIGHT;
    break;
  }

  screen_x = (PL_GFX_SCREEN_WIDTH / 2) - (screen_w / 2);
  screen_y = (PL_GFX_SCREEN_HEIGHT / 2) - (screen_h / 2);

  line_height = pspFontGetLineHeight(&PspStockFont);
  clear_screen = 1;
  show_kybd_held = 0;
  keyboard_visible = 0;

  keyboard_clear_keymatrix();
  video_psp_refresh_screen();
}

void input_poll()
{
  int joy_port = 2;

  /* Reset joystick */
  joystick_value[joy_port] = 0;

  /* Parse input */
  static SceCtrlData pad;
  if (pspCtrlPollControls(&pad))
  {
    if (keyboard_visible)
      pl_vk_navigate(&psp_keyboard, &pad);

    const psp_ctrl_mask_to_index_map_t *current_mapping = physical_to_emulated_button_map;
    for (; current_mapping->mask; current_mapping++)
    {
      u32 code = current_map.button_map[current_mapping->index];
      u8  on = (pad.Buttons & current_mapping->mask) == current_mapping->mask;
      if (!keyboard_visible)
      {
        if (on)
        {
          if (current_mapping->index < MAP_SHIFT_START_POS)
            /* If a button set is pressed, unset it, so it */
            /* doesn't trigger any other combination presses. */
            pad.Buttons &= ~current_mapping->mask;
          else
            /* Shift mode: Don't unset the L/R; just the rest */
            pad.Buttons &= ~(current_mapping->mask &
                             ~(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER));
        }

        if (code & KBD)
        {
          psp_keyboard_toggle(code, on);
          continue;
        }
        else if ((code & JOY) && on)
        {
          joystick_value[joy_port] |= CODE_MASK(code);
          continue;
        }
      }

      if (code & SPC)
      {
        switch (CODE_MASK(code))
        {
        case SPC_MENU:
          if (on) { video_psp_display_menu(); return; }
          break;
        case SPC_KYBD:
          if (psp_options.toggle_vk)
          {
            if (show_kybd_held != on && on)
            {
              keyboard_visible = !keyboard_visible;
              keyboard_clear_keymatrix();

              if (keyboard_visible) 
                pl_vk_reinit(&psp_keyboard);
              else clear_screen = 1;
            }
          }
          else
          {
            if (show_kybd_held != on)
            {
              keyboard_visible = on;
              if (on) 
                pl_vk_reinit(&psp_keyboard);
              else
              {
                clear_screen = 1;
                keyboard_clear_keymatrix();
              }
            }
          }

          show_kybd_held = on;
          break;
        }
      }
    }
  }
}

void video_canvas_refresh(struct video_canvas_s *canvas,
                                 unsigned int xs, unsigned int ys,
                                 unsigned int xi, unsigned int yi,
                                 unsigned int w, unsigned int h)
{
  if (canvas->width == 0)
    return;

  if (psp_first_time)
  {
    video_psp_display_menu();
    psp_first_time = 0;
  }

  video_psp_refresh_screen();
}

void video_psp_refresh_screen()
{
  /* Update the display */
  pspVideoBegin();

  /* Clear the buffer first, if necessary */
  if (clear_screen >= 0)
  {
    clear_screen--;
    pspVideoClearScreen();
  }
  else
  {
    if (psp_options.show_fps)
      pspVideoFillRect(0, 0, SCR_WIDTH, line_height, PSP_COLOR_BLACK);
  }

  /* Draw the screen */
  pl_gfx_put_image(Screen, screen_x, screen_y, screen_w, screen_h); 

  /* Draw keyboard */
  if (keyboard_visible)
    pl_vk_render(&psp_keyboard);

  if (psp_options.show_fps)
  {
    static char fps_display[64];
    sprintf(fps_display, " %3.02f (%.02f%%)", last_framerate, last_percent);

    int width = pspFontGetTextWidth(&PspStockFont, fps_display);

    pspVideoPrint(&PspStockFont, SCR_WIDTH - width, 0, fps_display, PSP_COLOR_WHITE);
  }

  pspVideoEnd();

  /* Swap buffers */
  pspVideoSwapBuffers();
}

void ui_display_speed(float percent, float framerate, int warp_flag)
{
  last_framerate = framerate;
  last_percent = percent;
}

int video_init()
{
  return 0;
}

void video_shutdown()
{
}

int video_arch_resources_init()
{
  if (!pl_vk_load(&psp_keyboard, "c64.l2", "c64keys.png", NULL, psp_keyboard_toggle))
    return 1;
  return 0;
}

void video_arch_resources_shutdown()
{
  /* Destroy keyboard */
  pl_vk_destroy(&psp_keyboard);
}

static inline void psp_keyboard_toggle(unsigned int code, int on)
{
  keyboard_set_keyarr(CKROW(code),CKCOL(code), on);
}
