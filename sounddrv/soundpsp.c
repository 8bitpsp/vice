/*
 * sounddummy.c - Implementation of the dummy sound device
 *
 * Written by
 *  Teemu Rantanen <tvr@cs.hut.fi>
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

#include <string.h>
#include <pspkernel.h>

#include "vice.h"

#include "sound.h"
#include "lib/pl_snd.h"
#include "lib.h"
#include "sfifo.h"

static sfifo_t sound_fifo;

static void psp_sound_callback(pl_snd_sample* stream, unsigned int samples, void *userdata);

static int psp_sound_init(const char *param, int *speed,
		    int *fragsize, int *fragnr, int *channels)
{
  *speed = 44100;
  *fragsize = 44100/60;
  *fragnr = 2;
  *channels = 2;

  if (sfifo_init(&sound_fifo, (*fragnr) * (*channels) * (*fragsize) + 1))
    return 1;

  pl_snd_set_callback(0, psp_sound_callback, 0);
  pl_snd_resume(0);

  return 0;
}

static int psp_sound_write(SWORD *pbuf, size_t nr)
{
  int i = 0;

  /* Convert to bytes */
  BYTE *bytes = (BYTE*)pbuf;
  nr <<= 1;

  while (nr)
  {
    if ((i = sfifo_write(&sound_fifo, bytes, nr)) < 0)
      break;
    else if (!i)
      sceKernelDelayThread(10);

    bytes += i;
    nr -= i;
  }

  return 0;
}

/* TODO: keep? */
#if 0
static int psp_sound_bufferspace(void)
{
/*
    int		amount;
    amount = sdl_inptr - sdl_outptr;
    if (amount < 0)
	amount += sdl_len;
    return sdl_len - amount;
*/
}
#endif

static void psp_sound_close(void)
{
  pl_snd_pause(0);
  sfifo_flush(&sound_fifo);
  sfifo_close(&sound_fifo);
}

static sound_device_t psp_sound =
{
    "PSP",
    psp_sound_init,
    psp_sound_write,
    NULL,
    NULL,
    NULL, //psp_sound_bufferspace,
    psp_sound_close,
    NULL,
    NULL,
    1
};

int sound_init_psp_device(void)
{
    return sound_register_device(&psp_sound);
}

static void psp_sound_callback(pl_snd_sample* stream, unsigned int samples, void *userdata)
{
  int length = samples << 1; /* 2 bits per sample */
  if (sfifo_used(&sound_fifo) <= 0)
  {
    /* Render silence if not enough sound data */
    memset(stream, 0, length);
    return;
  }

  sfifo_read(&sound_fifo, stream, length);
}

