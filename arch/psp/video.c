#include "vice.h"
#include "cmdline.h"
#include "video.h"
#include "videoarch.h"
#include "palette.h"
#include "viewport.h"
#include "keyboard.h"
#include "lib.h"

#include "lib/video.h"
#include "lib/pl_perf.h"
#include "lib/ctrl.h"
#include "lib/pl_gfx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PspImage *Screen = NULL;
static float last_framerate = 0; /* TODO: reinitialize this */
static float last_percent = 0;   /* when reentering menu */

/* Video-related command-line options.  */
static const cmdline_option_t cmdline_options[] = {
    { NULL }
};

int video_init_cmdline_options(void)
{
    return cmdline_register_options(cmdline_options);
}

void fullscreen_capability()//cap_fullscreen_t *cap_fullscreen)
{
}

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
  if (Screen) pspImageDestroy(Screen);
  lib_free(canvas->video_draw_buffer_callback);
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
    return 0;
}

void video_arch_resources_shutdown()
{
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
    Screen = pspImageCreateVram(512, fb_height, PSP_IMAGE_INDEXED);

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

void video_canvas_refresh(struct video_canvas_s *canvas,
                                 unsigned int xs, unsigned int ys,
                                 unsigned int xi, unsigned int yi,
                                 unsigned int w, unsigned int h)
{
	if (canvas->width == 0)
    return;

  /* Update the display */
  pspVideoBegin();

  {
    int height = pspFontGetLineHeight(&PspStockFont);
    pspVideoFillRect(0, 0, SCR_WIDTH, height, PSP_COLOR_BLACK);
  }

  /* Draw the screen */
  pl_gfx_put_image(Screen, 
                   0, 0, 
                   Screen->Viewport.Width,
                   Screen->Viewport.Height);

  {
    static char fps_display[64];
    sprintf(fps_display, " %3.02f (%.02f%%)", last_framerate, last_percent);

    int width = pspFontGetTextWidth(&PspStockFont, fps_display);

    pspVideoPrint(&PspStockFont, SCR_WIDTH - width, 0, fps_display, PSP_COLOR_WHITE);
  }

  pspVideoEnd();

SceCtrlData pad;
pspCtrlPollControls(&pad);
if (pad.Buttons&PSP_CTRL_CROSS)
keyboard_set_keyarr(7,4,1);

  /* Swap buffers */
  pspVideoSwapBuffers();
}

void ui_display_speed(float percent, float framerate, int warp_flag)
{
  last_framerate = framerate;
  last_percent = percent;
}
