#include "vice.h"
#include "cmdline.h"
#include "video.h"
#include "videoarch.h"
#include "palette.h"
#include "viewport.h"

#include "lib/video.h"
#include "lib/pl_perf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PspImage *Screen = NULL;
static pl_perf_counter FpsCounter;

/* Video-related command-line options.  */
static const cmdline_option_t cmdline_options[] = {
    { NULL }
};

int video_init_cmdline_options(void)
{
    return cmdline_register_options(cmdline_options);
}

void fullscreen_capability(cap_fullscreen_t *cap_fullscreen)
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

  Screen->Viewport.X = 0;
  Screen->Viewport.Y = 0;
  Screen->Viewport.Width = 384;
  Screen->Viewport.Height = Screen->Height;

  canvas->depth = Screen->Depth;
  canvas->width = 320;
  canvas->height = 200;

  video_canvas_set_palette(canvas, canvas->palette);

  pl_perf_init_counter(&FpsCounter);

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
  /* Update the display */
  pspVideoBegin();

  // AKTODO: Direct write to VRAM buffer
  int i, j;
  for (i = 0; i < 200; i++)
  {
    u8 *src = (u8*)(canvas->draw_buffer->draw_buffer 
                    + i * (canvas->draw_buffer->draw_buffer_pitch) + 96);
    u8 *dest = (u8*)(Screen->Pixels + i * Screen->Width);

    for (j = 0; j < 320; j++, dest++, src++)
      *dest = *src;
  }

  /* Draw the screen */
  pspVideoPutImage(Screen, 
                   0, 0, 
                   Screen->Viewport.Width,
                   Screen->Viewport.Height);

/* TODO */
  {
    static char fps_display[64];
    sprintf(fps_display, " %3.02f ", pl_perf_update_counter(&FpsCounter));

    int width = pspFontGetTextWidth(&PspStockFont, fps_display);
    int height = pspFontGetLineHeight(&PspStockFont);

    pspVideoFillRect(SCR_WIDTH - width, 0, SCR_WIDTH, height, PSP_COLOR_BLACK);
    pspVideoPrint(&PspStockFont, SCR_WIDTH - width, 0, fps_display, PSP_COLOR_WHITE);
  }
/* */

  pspVideoEnd();

  /* Swap buffers */
  pspVideoSwapBuffers();

  // AKTODO: check input here
}
