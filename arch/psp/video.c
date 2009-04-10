#include "vice.h"
#include "cmdline.h"
#include "video.h"
#include "videoarch.h"
#include "palette.h"
#include "viewport.h"
#include "keyboard.h"

#include "lib/video.h"
#include "lib/pl_perf.h"
#include "lib/ctrl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PspImage *Screen = NULL;
static float last_framerate = 0; /* AKTODO: reinitialize this */
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
}

video_canvas_t *video_canvas_create(video_canvas_t *canvas, 
    unsigned int *width, unsigned int *height, int mapped)
{
  if (!(Screen = pspImageCreateVram(512, 272, PSP_IMAGE_INDEXED)))
      return NULL;

  Screen->Viewport.X = 40;
  Screen->Viewport.Y = 40;
  Screen->Viewport.Width = 360;
  Screen->Viewport.Height = 252;

  canvas->depth = Screen->Depth;
  canvas->width = Screen->Viewport.Width;
  canvas->height = Screen->Viewport.Height;//200;

  video_canvas_set_palette(canvas, canvas->palette);

  return canvas;
}

void video_canvas_destroy(struct video_canvas_s *canvas)
{
  if (Screen) pspImageDestroy(Screen);
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

void video_arch_canvas_init(struct video_canvas_s *canvas)
{
  canvas->video_draw_buffer_callback = NULL;
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
	if (canvas->width==0) return;

  /* Update the display */
  pspVideoBegin();

  // AKTODO: Direct write to VRAM buffer
  int i, j;
  for (i = 0; i < Screen->Viewport.Height; i++)
  {
    u8 *src = (u8*)(canvas->draw_buffer->draw_buffer 
                    + i * (canvas->draw_buffer->draw_buffer_pitch) + 96);
    u8 *dest = (u8*)(Screen->Pixels + i * Screen->Width);

    for (j = 0; j < Screen->Viewport.Width; j++, dest++, src++)
      *dest = *src;
  }

  /* Draw the screen */
  pspVideoPutImage(Screen, 
                   0, 0, 
                   Screen->Viewport.Width,
                   Screen->Viewport.Height);

  {
    static char fps_display[64];
    sprintf(fps_display, " %3.02f (%.02f%%)", last_framerate, last_percent);

    int width = pspFontGetTextWidth(&PspStockFont, fps_display);
    int height = pspFontGetLineHeight(&PspStockFont);

    pspVideoFillRect(0, 0, SCR_WIDTH, height, PSP_COLOR_BLACK);
    pspVideoPrint(&PspStockFont, SCR_WIDTH - width, 0, fps_display, PSP_COLOR_WHITE);
  }

  pspVideoEnd();

SceCtrlData pad;
pspCtrlPollControls(&pad);
if (pad.Buttons&PSP_CTRL_CROSS)
keyboard_set_keyarr(7,4,1);

  /* Swap buffers */
  pspVideoSwapBuffers();

  // AKTODO: check input here
}

void ui_display_speed(float percent, float framerate, int warp_flag)
{
  last_framerate = framerate;
  last_percent = percent;
}
