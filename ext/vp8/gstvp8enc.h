/* VP8
 * Copyright (C) 2006 David Schleef <ds@schleef.org>
 * Copyright (C) 2010 Entropy Wave Inc
 * Copyright (C) 2010 Sebastian Dröge <sebastian.droege@collabora.co.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#ifndef __GST_VP8_ENC_H__
#define __GST_VP8_ENC_H__

#include <gst/gst.h>
#include <gst/video/gstvideoencoder.h>

/* FIXME: Undef HAVE_CONFIG_H because vpx_codec.h uses it,
 * which causes compilation failures */
#ifdef HAVE_CONFIG_H
#undef HAVE_CONFIG_H
#endif

#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>

G_BEGIN_DECLS

#define GST_TYPE_VP8_ENC \
  (gst_vp8_enc_get_type())
#define GST_VP8_ENC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VP8_ENC,GstVP8Enc))
#define GST_VP8_ENC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VP8_ENC,GstVP8EncClass))
#define GST_IS_VP8_ENC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VP8_ENC))
#define GST_IS_VP8_ENC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VP8_ENC))

typedef struct _GstVP8Enc GstVP8Enc;
typedef struct _GstVP8EncClass GstVP8EncClass;

struct _GstVP8Enc
{
  GstVideoEncoder base_video_encoder;

  /* < private > */
  vpx_codec_ctx_t encoder;

  /* from downstream caps */
  int profile;

  /* properties */
  /* Rate control options */
  enum vpx_rc_mode rc_end_usage;
  unsigned int rc_target_bitrate;
  gboolean rc_target_bitrate_set;
  unsigned int rc_min_quantizer, rc_max_quantizer;

  unsigned int rc_dropframe_thresh;
  gboolean rc_resize_allowed;
  unsigned int rc_resize_up_thresh;
  unsigned int rc_resize_down_thresh;
  unsigned int rc_undershoot_pct;
  unsigned int rc_overshoot_pct;
  unsigned int rc_buf_sz;
  unsigned int rc_buf_initial_sz;
  unsigned int rc_buf_optimal_sz;

  unsigned int rc_2pass_vbr_bias_pct;
  unsigned int rc_2pass_vbr_minsection_pct;
  unsigned int rc_2pass_vbr_maxsection_pct;

  /* Global keyframe options */
  enum vpx_kf_mode kf_mode;
  unsigned int kf_max_dist;

  /* Global two-pass options */
  enum vpx_enc_pass multipass_mode;
  gchar *multipass_cache_file;
  GByteArray *first_pass_cache_content;
  vpx_fixed_buf_t last_pass_cache_content;

  /* Global temporal scalability options */
  unsigned int ts_number_layers;
  unsigned int ts_target_bitrate[VPX_TS_MAX_LAYERS];
  int n_ts_target_bitrate;
  unsigned int ts_rate_decimator[VPX_TS_MAX_LAYERS];
  int n_ts_rate_decimator;
  unsigned int ts_periodicity;
  unsigned int ts_layer_id[VPX_TS_MAX_PERIODICITY];
  int n_ts_layer_id;

  /* Global, other options */
  vpx_codec_er_flags_t error_resilient;
  unsigned int lag_in_frames;

  int threads;
#if 0
  /* Only usage 0 is defined right now */
  int usage;
#endif

  /* Encode parameter */
  gint64 deadline;

  /* Controls */
  VPX_SCALING_MODE h_scaling_mode;
  VPX_SCALING_MODE v_scaling_mode;
  int cpu_used;
  gboolean enable_auto_alt_ref;
  unsigned int noise_sensitivity;
  unsigned int sharpness;
  unsigned int static_threshold;
  vp8e_token_partitions token_partitions;
  unsigned int arnr_maxframes;
  unsigned int arnr_strength;
  unsigned int arnr_type;
  vp8e_tuning tuning;
  unsigned int cq_level;
  unsigned int max_intra_bitrate_pct;

  /* state */
  gboolean inited;

  vpx_image_t image;

  int n_frames;
  int keyframe_distance;

  GstVideoCodecState *input_state;
};

struct _GstVP8EncClass
{
  GstVideoEncoderClass base_video_encoder_class;
};

GType gst_vp8_enc_get_type (void);

G_END_DECLS

#endif /* __GST_VP8_ENC_H__ */
