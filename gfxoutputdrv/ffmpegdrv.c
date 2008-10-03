/*
 * ffmpegdrv.c - Movie driver using FFMPEG library and screenshot API.
 *
 * Written by
 *  Andreas Matthies <andreas.matthies@gmx.net>
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
#include <string.h>

#include "archdep.h"
#include "cmdline.h"
#include "ffmpegdrv.h"
#include "ffmpeglib.h"
#include "gfxoutput.h"
#include "lib.h"
#include "log.h"
#include "palette.h"
#include "resources.h"
#include "screenshot.h"
#ifdef HAS_TRANSLATION
#include "translate.h"
#endif
#include "ui.h"
#include "util.h"
#include "vsync.h"

static ffmpegdrv_codec_t avi_audio_codeclist[] = { 
    { CODEC_ID_MP2, "MP2" },
    { CODEC_ID_MP3, "MP3" },
    { CODEC_ID_PCM_S16LE, "PCM uncompressed" },
    { 0, NULL }
};

static ffmpegdrv_codec_t avi_video_codeclist[] = { 
    { CODEC_ID_MPEG4, "MPEG4 (DivX)" },
    { CODEC_ID_MPEG1VIDEO, "MPEG1" },
    { CODEC_ID_FFV1, "FFV1 (lossless)" },
    { 0, NULL }
};

ffmpegdrv_format_t ffmpegdrv_formatlist[] =
{
    { "avi", avi_audio_codeclist, avi_video_codeclist },
    { "wav", NULL, NULL },
    { "mp3", NULL, NULL },
    { "mp2", NULL, NULL },
    { NULL, NULL, NULL }
};

/* general */
static ffmpeglib_t ffmpeglib;
static AVFormatContext *ffmpegdrv_oc;
static AVOutputFormat *ffmpegdrv_fmt;
static int file_init_done;

/* audio */
static AVStream *audio_st;
static ffmpegdrv_audio_in_t ffmpegdrv_audio_in;
static int audio_init_done;
static int audio_is_open;
static unsigned char *audio_outbuf;
static int audio_outbuf_size;
static double audio_pts;

/* video */
static AVStream *video_st;
static int video_init_done;
static int video_is_open;
static AVFrame *picture, *tmp_picture;
static unsigned char *video_outbuf;
static int video_outbuf_size;
static int video_width, video_height;
static AVFrame *picture, *tmp_picture;
static double video_pts;

/* resources */
static char *ffmpeg_format = NULL;
static int format_index;
static int audio_bitrate;
static int video_bitrate;
static int audio_codec;
static int video_codec;

static int ffmpegdrv_init_file(void);

static int set_format(const char *val, void *param)
{
    int i;

    format_index = -1;
    util_string_set(&ffmpeg_format, val);
    for (i = 0; ffmpegdrv_formatlist[i].name != NULL; i++)
        if (strcmp(ffmpeg_format, ffmpegdrv_formatlist[i].name) == 0)
            format_index = i;

    if (format_index < 0)
        return -1;
    else
        return 0;
}

static int set_audio_bitrate(int val, void *param)
{
    audio_bitrate = (CLOCK)val;
    if (audio_bitrate < 16000 || audio_bitrate > 128000)
        audio_bitrate = 64000;

    return 0;
}

static int set_video_bitrate(int val, void *param)
{
    video_bitrate = (CLOCK)val;
    if (video_bitrate < 100000 || video_bitrate > 10000000)
        video_bitrate = 800000;

    return 0;
}

static int set_audio_codec(int val, void *param)
{
    audio_codec = val;
    return 0;
}

static int set_video_codec(int val, void *param)
{
    video_codec = val;
    return 0;

}

/*---------- Resources ------------------------------------------------*/
static const resource_string_t resources_string[] = {
    { "FFMPEGFormat", "avi", RES_EVENT_NO, NULL,
      &ffmpeg_format, set_format, NULL },
    { NULL }
};

static const resource_int_t resources_int[] = {
    { "FFMPEGAudioBitrate", 64000, RES_EVENT_NO, NULL,
      &audio_bitrate, set_audio_bitrate, NULL },
    { "FFMPEGVideoBitrate", 800000, RES_EVENT_NO, NULL,
      &video_bitrate, set_video_bitrate, NULL },
    { "FFMPEGAudioCodec", CODEC_ID_MP3, RES_EVENT_NO, NULL,
      &audio_codec, set_audio_codec, NULL },
    { "FFMPEGVideoCodec", CODEC_ID_MPEG4, RES_EVENT_NO, NULL,
      &video_codec, set_video_codec, NULL },
    { NULL }
};


int ffmpegdrv_resources_init(void)
{
    if (resources_register_string(resources_string) < 0)
        return -1;

    return resources_register_int(resources_int);
}
/*---------------------------------------------------------------------*/


/*---------- Commandline options --------------------------------------*/

#ifdef HAS_TRANSLATION
static const cmdline_option_t cmdline_options[] = {
    { "-ffmpegaudiobitrate", SET_RESOURCE, 1, NULL, NULL,
      "FFMPEGAudioBitrate", NULL,
      IDCLS_P_VALUE, IDCLS_SET_AUDIO_STREAM_BITRATE },
    { "-ffmpegvideobitrate", SET_RESOURCE, 1, NULL, NULL,
      "FFMPEGVideoBitrate", NULL,
      IDCLS_P_VALUE, IDCLS_SET_VIDEO_STREAM_BITRATE },
    { NULL }
};
#else
static const cmdline_option_t cmdline_options[] = {
    { "-ffmpegaudiobitrate", SET_RESOURCE, 1, NULL, NULL,
      "FFMPEGAudioBitrate", NULL,
      N_("<value>"), N_("Set bitrate for audio stream in media file") },
    { "-ffmpegvideobitrate", SET_RESOURCE, 1, NULL, NULL,
      "FFMPEGVideoBitrate", NULL,
      N_("<value>"), N_("Set bitrate for video stream in media file") },
    { NULL }
};
#endif

int ffmpegdrv_cmdline_options_init(void)
{
    return cmdline_register_options(cmdline_options);
}

/*---------------------------------------------------------------------*/



/*-----------------------*/
/* audio stream encoding */
/*-----------------------*/
static int ffmpegdrv_open_audio(AVFormatContext *oc, AVStream *st)
{
    AVCodecContext *c;
    AVCodec *codec;
    int audio_inbuf_samples;

    c = st->codec;

    /* find the audio encoder */
    codec = (*ffmpeglib.p_avcodec_find_encoder)(c->codec_id);
    if (!codec) {
        log_debug("ffmpegdrv: audio codec not found");
        return -1;
    }

    /* open it */
    if ((*ffmpeglib.p_avcodec_open)(c, codec) < 0) {
        log_debug("ffmpegdrv: could not open audio codec");
        return -1;
    }
    
    audio_is_open = 1;
    audio_outbuf_size = 10000;
    audio_outbuf = (unsigned char*)lib_malloc(audio_outbuf_size);

    /* ugly hack for PCM codecs (will be removed ASAP with new PCM
       support to compute the input frame size in samples */
    if (c->frame_size <= 1) {
        audio_inbuf_samples = audio_outbuf_size;
        switch(st->codec->codec_id) {
        case CODEC_ID_PCM_S16LE:
        case CODEC_ID_PCM_S16BE:
        case CODEC_ID_PCM_U16LE:
        case CODEC_ID_PCM_U16BE:
            audio_inbuf_samples >>= 1;
            break;
        default:
            break;
        }
    } else {
        audio_inbuf_samples = c->frame_size * c->channels;
    }
    ffmpegdrv_audio_in.buffersamples = audio_inbuf_samples;
    ffmpegdrv_audio_in.buffer = (SWORD*)lib_malloc(audio_inbuf_samples
                                                    * sizeof(SWORD));

    return 0;
}


static void ffmpegdrv_close_audio(void)
{
    if (audio_st == NULL)
        return;

    if (audio_is_open)
        (*ffmpeglib.p_avcodec_close)(audio_st->codec);

    audio_is_open = 0;
    lib_free(ffmpegdrv_audio_in.buffer);
    ffmpegdrv_audio_in.buffer = NULL;
    ffmpegdrv_audio_in.buffersamples = 0;
    if (audio_outbuf) {
        lib_free(audio_outbuf);
        audio_outbuf = NULL;
    }
}


void ffmpegdrv_init_audio(int speed, int channels,
                          ffmpegdrv_audio_in_t** audio_in)
{
    AVCodecContext *c;
    AVStream *st;

    if (ffmpegdrv_oc == NULL || ffmpegdrv_fmt == NULL)
        return;

    audio_init_done = 1;

    if (ffmpegdrv_fmt->audio_codec == CODEC_ID_NONE)
        return;

    *audio_in = &ffmpegdrv_audio_in;
    
    (*audio_in)->buffersamples = 0; /* not allocated yet */
    (*audio_in)->used = 0;

    st = (*ffmpeglib.p_av_new_stream)(ffmpegdrv_oc, 1);
    if (!st) {
        log_debug("ffmpegdrv: Could not alloc audio stream\n");
        return;
    }

    c = st->codec;
    c->codec_id = ffmpegdrv_fmt->audio_codec;
    c->codec_type = CODEC_TYPE_AUDIO;

    /* put sample parameters */
    c->bit_rate = audio_bitrate;
    c->sample_rate = speed;
    c->channels = channels;
    audio_st = st;
    audio_pts = 0;

    if (video_init_done)
        ffmpegdrv_init_file();
}


/* triggered by soundffmpegaudio->write */
void ffmpegdrv_encode_audio(ffmpegdrv_audio_in_t *audio_in)
{
    if (audio_st) {
#if FFMPEG_VERSION_INT==0x000408
        int out_size = (*ffmpeglib.p_avcodec_encode_audio)(&audio_st->codec, 
                        audio_outbuf, audio_outbuf_size, audio_in->buffer);
        /* FIXME: Some sync needed ?? */
    
        if ((*ffmpeglib.p_av_write_frame)(ffmpegdrv_oc, audio_st->index, 
                       audio_outbuf, out_size) != 0)
            log_debug("ffmpegdrv_encode_audio: Error while writing audio frame");

        audio_pts = (double)audio_st->pts.val * ffmpegdrv_oc->pts_num 
                    / ffmpegdrv_oc->pts_den;
#else
        AVPacket pkt;
        AVCodecContext *c;
        av_init_packet(&pkt);
        c = audio_st->codec;
        pkt.size = (*ffmpeglib.p_avcodec_encode_audio)(c, 
                        audio_outbuf, audio_outbuf_size, audio_in->buffer);
        pkt.pts = c->coded_frame->pts;
        pkt.flags |= PKT_FLAG_KEY;
        pkt.stream_index = audio_st->index;
        pkt.data = audio_outbuf;

        if ((*ffmpeglib.p_av_write_frame)(ffmpegdrv_oc, &pkt) != 0)
            log_debug("ffmpegdrv_encode_audio: Error while writing audio frame");

        audio_pts = (double)audio_st->pts.val * audio_st->time_base.num 
                    / audio_st->time_base.den;
#endif

    }

    audio_in->used = 0;
}


/*-----------------------*/
/* video stream encoding */
/*-----------------------*/
static int ffmpegdrv_fill_rgb_image(screenshot_t *screenshot, AVFrame *pic)
{ 
    int x, y;
    int colnum;
    int bufferoffset;
    int x_dim = screenshot->width;
    int y_dim = screenshot->height;
    int pix;

    /* center the screenshot in the video */
    bufferoffset = screenshot->x_offset 
                    + screenshot->y_offset * screenshot->draw_buffer_line_size;

    pix = 3 * ((video_width - x_dim) / 2 + (video_height - y_dim) / 2 * x_dim);

    for (y = 0; y < y_dim; y++) {
        for (x=0; x < x_dim; x++) {
            colnum = screenshot->draw_buffer[bufferoffset + x];
            pic->data[0][pix++] = screenshot->palette->entries[colnum].red;
            pic->data[0][pix++] = screenshot->palette->entries[colnum].green;
            pic->data[0][pix++] = screenshot->palette->entries[colnum].blue;
        }

        bufferoffset += screenshot->draw_buffer_line_size;
    }

    return 0;
}


static AVFrame* ffmpegdrv_alloc_picture(int pix_fmt, int width, int height)
{
    AVFrame *picture;
    unsigned char *picture_buf;
    int size;
    
    picture = (AVFrame*)lib_malloc(sizeof(AVFrame));
    memset(picture, 0, sizeof(AVFrame));

    picture->pts = AV_NOPTS_VALUE;

    size = (*ffmpeglib.p_avpicture_get_size)(pix_fmt, width, height);
    picture_buf = (unsigned char *)lib_malloc(size);
    memset(picture_buf, 0, size);
    if (!picture_buf) {
        lib_free(picture);
        return NULL;
    }
    (*ffmpeglib.p_avpicture_fill)((AVPicture *)picture, picture_buf, 
                   pix_fmt, width, height);

    return picture;
}


static int ffmpegdrv_open_video(AVFormatContext *oc, AVStream *st)
{
    AVCodec *codec;
    AVCodecContext *c;

    c = st->codec;

    /* find the video encoder */
    codec = (*ffmpeglib.p_avcodec_find_encoder)(c->codec_id);
    if (!codec) {
        log_debug("ffmpegdrv: video codec not found");
        return -1;
    }

    /* open the codec */
    if ((*ffmpeglib.p_avcodec_open)(c, codec) < 0) {
        log_debug("ffmpegdrv: could not open video codec");
        return -1;
    }

    video_is_open = 1;
    video_outbuf = NULL;
    if (!(oc->oformat->flags & AVFMT_RAWPICTURE)) {
        /* allocate output buffer */
        /* XXX: API change will be done */
        video_outbuf_size = 200000;
        video_outbuf = (unsigned char*)lib_malloc(video_outbuf_size);
    }

    /* allocate the encoded raw picture */
    picture = ffmpegdrv_alloc_picture(c->pix_fmt, c->width, c->height);
    if (!picture) {
        log_debug("ffmpegdrv: could not allocate picture");
        return -1;
    }

    /* if the output format is not RGB24, then a temporary YUV420P
       picture is needed too. It is then converted to the required
       output format */
    tmp_picture = NULL;
    if (c->pix_fmt != PIX_FMT_RGB24) {
        tmp_picture = ffmpegdrv_alloc_picture(PIX_FMT_RGB24, 
                                                c->width, c->height);
        if (!tmp_picture) {
            log_debug("ffmpegdrv: could not allocate temporary picture");
            return -1;
        }
    }
    return 0;
}


static void ffmpegdrv_close_video(void)
{
    if (video_st == NULL)
        return;

    if (video_is_open)
        (*ffmpeglib.p_avcodec_close)(video_st->codec);

    video_is_open = 0;
    if (video_outbuf != NULL)
        lib_free(video_outbuf);
    if (picture) {
        lib_free(picture->data[0]);
        lib_free(picture);
        picture = NULL;
    }
    if (tmp_picture) {
        lib_free(tmp_picture->data[0]);
        lib_free(tmp_picture);
        tmp_picture = NULL;
    }
}


static void ffmpegdrv_init_video(screenshot_t *screenshot)
{
    AVCodecContext *c;
    AVStream *st;

    if (ffmpegdrv_oc == NULL || ffmpegdrv_fmt == NULL)
        return;

     video_init_done = 1;

     if (ffmpegdrv_fmt->video_codec == CODEC_ID_NONE)
        return;

    st = (*ffmpeglib.p_av_new_stream)(ffmpegdrv_oc, 0);
    if (!st) {
        log_debug("ffmpegdrv: Could not alloc video stream\n");
        return;
    }

    c = st->codec;
    c->codec_id = ffmpegdrv_fmt->video_codec;
    c->codec_type = CODEC_TYPE_VIDEO;

    /* put sample parameters */
    c->bit_rate = video_bitrate;
    /* resolution should be a multiple of 16 */
    video_width = c->width = (screenshot->width + 15) & ~0xf; 
    video_height = c->height = (screenshot->height + 15) & ~0xf;
    /* frames per second */
    c->time_base.den = (int)(vsync_get_refresh_frequency() + 0.5);
    c->time_base.num = 1;
    c->gop_size = 12; /* emit one intra frame every twelve frames at most */
    c->pix_fmt = PIX_FMT_YUV420P;

    /* FFV1 isn't strict standard compliant */
    if (c->codec_id == CODEC_ID_FFV1) {
        c->strict_std_compliance = -1;
        c->pix_fmt = PIX_FMT_RGBA32;
    }

    video_st = st;
    video_pts = 0;

    if (audio_init_done)
        ffmpegdrv_init_file();
}


static int ffmpegdrv_init_file(void)
{
    if (!video_init_done || !audio_init_done)
        return 0;

    if ((*ffmpeglib.p_av_set_parameters)(ffmpegdrv_oc, NULL) < 0) {
        log_debug("ffmpegdrv: Invalid output format parameters");
            return -1;
    }

    (*ffmpeglib.p_dump_format)(ffmpegdrv_oc, 0, ffmpegdrv_oc->filename, 1);

    if (video_st && (ffmpegdrv_open_video(ffmpegdrv_oc, video_st) < 0)) {
#ifdef HAS_TRANSLATION
        ui_error(translate_text(IDGS_FFMPEG_CANNOT_OPEN_VSTREAM));
#else
        ui_error(_("ffmpegdrv: Cannot open video stream"));
#endif
        screenshot_stop_recording();
        return -1;
    }
    if (audio_st && (ffmpegdrv_open_audio(ffmpegdrv_oc, audio_st) < 0)) {
#ifdef HAS_TRANSLATION
        ui_error(translate_text(IDGS_FFMPEG_CANNOT_OPEN_ASTREAM));
#else
        ui_error(_("ffmpegdrv: Cannot open audio stream"));
#endif
        screenshot_stop_recording();
        return -1;
    }

    if (!(ffmpegdrv_fmt->flags & AVFMT_NOFILE)) {
        if ((*ffmpeglib.p_url_fopen)(&ffmpegdrv_oc->pb, ffmpegdrv_oc->filename,
                            URL_WRONLY) < 0) 
        {
#ifdef HAS_TRANSLATION
            ui_error(translate_text(IDGS_FFMPEG_CANNOT_OPEN_S), ffmpegdrv_oc->filename);
#else
            ui_error(_("ffmpegdrv: Cannot open %s"), ffmpegdrv_oc->filename);
#endif
            screenshot_stop_recording();
            return -1;
        }

    }

    (*ffmpeglib.p_av_write_header)(ffmpegdrv_oc);

    log_debug("ffmpegdrv: Initialized file successfully");

    file_init_done = 1;

    return 0;
}


static int ffmpegdrv_save(screenshot_t *screenshot, const char *filename)
{
    video_st = NULL;
    audio_st = NULL;

    audio_init_done = 0;
    video_init_done = 0;
    file_init_done = 0;

    ffmpegdrv_fmt = (*ffmpeglib.p_guess_format)(ffmpeg_format, NULL, NULL);

    if (!ffmpegdrv_fmt)
        ffmpegdrv_fmt = (*ffmpeglib.p_guess_format)("mpeg", NULL, NULL);

    if (!ffmpegdrv_fmt) {
        log_debug("ffmpegdrv: Cannot find suitable output format");
        return -1;
    }

    if (format_index >= 0) {

        ffmpegdrv_format_t *format = &ffmpegdrv_formatlist[format_index];

        if (format->audio_codecs !=NULL
            && (*ffmpeglib.p_avcodec_find_encoder)(audio_codec) != NULL)
        {
            ffmpegdrv_fmt->audio_codec = audio_codec;
        }

        if (format->video_codecs !=NULL
            && (*ffmpeglib.p_avcodec_find_encoder)(video_codec) != NULL)
        {
            ffmpegdrv_fmt->video_codec = video_codec;
        }
    }

    ffmpegdrv_oc = (AVFormatContext*)lib_malloc(sizeof(AVFormatContext));
    memset(ffmpegdrv_oc, 0, sizeof(AVFormatContext));

    if (!ffmpegdrv_oc) {
        log_debug("ffmpegdrv: Cannot allocate format context");
        return -1;
    }

    ffmpegdrv_oc->oformat = ffmpegdrv_fmt;
    snprintf(ffmpegdrv_oc->filename, sizeof(ffmpegdrv_oc->filename), 
             "%s", filename);

    ffmpegdrv_init_video(screenshot);

    resources_set_string("SoundRecordDeviceName", "ffmpegaudio");
    resources_set_int("Sound", 1);

    return 0;
}


static int ffmpegdrv_close(screenshot_t *screenshot)
{
    int i;

    if (video_st)
        ffmpegdrv_close_video();
    if (audio_st)
        ffmpegdrv_close_audio();

    /* write the trailer, if any */
    if (file_init_done) {
        (*ffmpeglib.p_av_write_trailer)(ffmpegdrv_oc);
        if (!(ffmpegdrv_fmt->flags & AVFMT_NOFILE)) {
            /* close the output file */
            (*ffmpeglib.p_url_fclose)(&ffmpegdrv_oc->pb);
        }
    }
    
    /* free the streams */
    for(i = 0; i < ffmpegdrv_oc->nb_streams; i++) {
        (*ffmpeglib.p_av_free)((void *)ffmpegdrv_oc->streams[i]);
        ffmpegdrv_oc->streams[i] = NULL;
    }


    /* free the stream */
    lib_free(ffmpegdrv_oc);
    log_debug("ffmpegdrv: Closed successfully");

    resources_set_string("SoundRecordDeviceName", "");

    file_init_done = 0;

    return 0;
}


/* triggered by screenshot_record */
static int ffmpegdrv_record(screenshot_t *screenshot)
{
    AVCodecContext *c;
    int out_size;
    int ret;

    if (audio_init_done && video_init_done && !file_init_done)
        ffmpegdrv_init_file();

    if (video_st == NULL || !file_init_done)
        return 0;

    if (audio_st && video_pts > audio_pts) {
        /* drop this frame */
        return 0;
    }

    c = video_st->codec;

    if (c->pix_fmt != PIX_FMT_RGB24) {
        ffmpegdrv_fill_rgb_image(screenshot, tmp_picture);
        (*ffmpeglib.p_img_convert)((AVPicture *)picture, c->pix_fmt,
                    (AVPicture *)tmp_picture, PIX_FMT_RGB24,
                    c->width, c->height);
    } else {
        ffmpegdrv_fill_rgb_image(screenshot, picture);
    }

    if (ffmpegdrv_oc->oformat->flags & AVFMT_RAWPICTURE) {
        /* raw video case. The API will change slightly in the near
           futur for that */
#if FFMPEG_VERSION_INT==0x000408
        ret = (*ffmpeglib.p_av_write_frame)(ffmpegdrv_oc, video_st->index,
                       (unsigned char *)picture, sizeof(AVPicture));
#else
        AVPacket pkt;
        av_init_packet(&pkt);
        pkt.flags |= PKT_FLAG_KEY;
        pkt.stream_index = video_st->index;
        pkt.data = (uint8_t*)picture;
        pkt.size = sizeof(AVPicture);

        ret = (*ffmpeglib.p_av_write_frame)(ffmpegdrv_oc, &pkt);
#endif
    } else {
        /* encode the image */
        out_size = (*ffmpeglib.p_avcodec_encode_video)(c, video_outbuf, 
                                                video_outbuf_size, picture);
        /* if zero size, it means the image was buffered */
        if (out_size != 0) {
            /* write the compressed frame in the media file */
#if FFMPEG_VERSION_INT==0x000408
            ret = (*ffmpeglib.p_av_write_frame)(ffmpegdrv_oc, video_st->index,
                                        video_outbuf, out_size);
#else
            AVPacket pkt;
            av_init_packet(&pkt);
            pkt.pts = c->coded_frame->pts;
            if (c->coded_frame->key_frame)
                pkt.flags |= PKT_FLAG_KEY;
            pkt.stream_index = video_st->index;
            pkt.data = video_outbuf;
            pkt.size = out_size;
            ret = (*ffmpeglib.p_av_write_frame)(ffmpegdrv_oc, &pkt);
#endif
        } else {
            ret = 0;
        }
    }
    if (ret != 0) {
        log_debug("Error while writing video frame");
        return -1;
    }

#if FFMPEG_VERSION_INT==0x000408
    video_pts = (double)video_st->pts.val * ffmpegdrv_oc->pts_num 
                    / ffmpegdrv_oc->pts_den;
#else
    video_pts = (double)video_st->pts.val * video_st->time_base.num 
                    / video_st->time_base.den;
#endif

    return 0;
}


static int ffmpegdrv_write(screenshot_t *screenshot)
{
    return 0;
}


static gfxoutputdrv_t ffmpeg_drv[] = {
    {
        "FFMPEG",
        "FFMPEG",
        NULL,
        NULL,
        ffmpegdrv_close,
        ffmpegdrv_write,
        ffmpegdrv_save,
#ifdef FEATURE_CPUMEMHISTORY
        ffmpegdrv_record,
        NULL
#else
        ffmpegdrv_record
#endif
    },
/*
{
        "FFMPEG",
        "MPEG video (MPEG1/MP2)",
        "mpeg",
        NULL,
        ffmpegdrv_close,
        ffmpegdrv_write,
        ffmpegdrv_save,
        ffmpegdrv_record
    },
    {
        "FFMPEG",
        "MP3 audio",
        "mp3",
        NULL,
        ffmpegdrv_close,
        ffmpegdrv_write,
        ffmpegdrv_save,
        ffmpegdrv_record
    },
    {
        "FFMPEG",
        "WAV audio",
        "wav",
        NULL,
        ffmpegdrv_close,
        ffmpegdrv_write,
        ffmpegdrv_save,
        ffmpegdrv_record
    },
*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
#ifdef FEATURE_CPUMEMHISTORY
      NULL,
#endif
      NULL }
};

void gfxoutput_init_ffmpeg(void)
{
    int i = 0;

    if (ffmpeglib_open(&ffmpeglib) < 0)
        return;

    while (ffmpeg_drv[i].name != NULL)
        gfxoutput_register(&ffmpeg_drv[i++]);

    (*ffmpeglib.p_av_register_all)();
}

void ffmpegdrv_shutdown(void)
{
    ffmpeglib_close(&ffmpeglib);
    lib_free(ffmpeg_format);
}

