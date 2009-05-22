/*
 * soundsdl.c - Implementation of the Simple Directmedia Layer sound device
 *
 * Written by
 *  Teemu Rantanen <tvr@cs.hut.fi>
 *  Daniel Aarno <macbishop@users.sourceforge.net>
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

#include <unistd.h>
#include <pspkernel.h>
#include <string.h>

#include "lib/pl_snd.h"

#include "vice.h"

#include "lib.h"
#include "sound.h"

static SWORD *sdl_buf = NULL;
static volatile int sdl_inptr = 0;
static volatile int sdl_outptr = 0;
static volatile int sdl_len = 0;

static void psp_sound_callback(pl_snd_sample* sample_stream,
                               unsigned int samples,
                               void *userdata)
{
  BYTE *stream = (BYTE*)sample_stream;
  int len = samples << 1, /* 2 bytes per sample */
      total = 0,
      amount;

  while (total < len / sizeof(SWORD))
  {
    amount = sdl_inptr - sdl_outptr;
    if (amount < 0)
      amount = sdl_len - sdl_outptr;
    if (amount + total > len / sizeof(SWORD))
      amount = len / sizeof(SWORD) - total;

    if (!amount)
    {
      memset(stream + total * sizeof(SWORD), 0, len - total * sizeof(SWORD));
      return;
    }

    memcpy(stream + total * sizeof(SWORD), 
           sdl_buf + sdl_outptr,
           amount * sizeof(SWORD));

    total += amount;
    sdl_outptr += amount;

    if (sdl_outptr == sdl_len)
      sdl_outptr = 0;
  }
}

static int psp_init(const char *param,
                    int *speed,
                    int *fragsize,
                    int *fragnr,
                    int *channels)
{
  *channels = 1;  /* No stereo capability. */
  *speed = 44100; /* PSP can only render at 44100 */

  pl_snd_set_callback(0, psp_sound_callback, 0);

  sdl_len = (*fragsize) * (*fragnr) + 1;
  sdl_inptr = sdl_outptr = 0;
  sdl_buf = lib_malloc(sizeof(SWORD)*sdl_len);

  if (!sdl_buf)
    return 1;

  pl_snd_resume(0);
  return 0;
}

static int psp_write(SWORD *pbuf, size_t nr)
{
  int	total = 0, amount;

  while (total < nr)
  {
  	amount = sdl_outptr - sdl_inptr;
	  if (amount <= 0)
	    amount = sdl_len - sdl_inptr;

	  if ((sdl_inptr + amount) % sdl_len == sdl_outptr)
	    amount--;

    if (amount <= 0)
    {
      sceKernelDelayThread(5000);
      continue;
	  }

	  if (total + amount > nr)
      amount = nr - total;

	  memcpy(sdl_buf + sdl_inptr, pbuf + total, amount * sizeof(SWORD));
	  sdl_inptr += amount;
	  total += amount;

	  if (sdl_inptr == sdl_len)
      sdl_inptr = 0;
  }

  return 0;
}

static int psp_bufferspace(void)
{
  int	amount;
  if ((amount = sdl_inptr - sdl_outptr) < 0)
	  amount += sdl_len;
  return sdl_len - amount;
}

static void psp_close(void)
{
  pl_snd_pause(0);
  lib_free(sdl_buf);
  sdl_buf = NULL;
  sdl_inptr = sdl_outptr = sdl_len = 0;
}


static sound_device_t psp_device =
{
  "psp",
  psp_init,
  psp_write,
  NULL,
  NULL,
  psp_bufferspace,
  psp_close,
  NULL,
  NULL,
  1
};

int sound_init_psp_device(void)
{
  return sound_register_device(&psp_device);
}
