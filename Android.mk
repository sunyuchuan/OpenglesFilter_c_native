# copyright (c) 2013 Zhang Rui <bbcallen@gmail.com>
#
# This file is part of ijkPlayer.
#
# ijkPlayer is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# ijkPlayer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with ijkPlayer; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

LOCAL_PATH := $(call my-dir)

ifeq ($(USE_MTCNN_NCNN), true)
ifeq ($(MTCNN_NCNN_PREBUILT), true)
include $(CLEAR_VARS)
LOCAL_MODULE := xmmtcnn_ncnn-$(TARGET_ARCH_ABI)
LOCAL_SRC_FILES := $(LOCAL_PATH)/../xmmtcnn_ncnn/lib/libxmmtcnn_ncnn-$(TARGET_ARCH_ABI).so
include $(PREBUILT_SHARED_LIBRARY)
endif
endif

include $(CLEAR_VARS)
LOCAL_CFLAGS += -std=c99
LOCAL_LDLIBS += -llog -landroid -lOpenSLES -lEGL -lGLESv2

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(realpath $(LOCAL_PATH)/..)
LOCAL_C_INCLUDES += $(MY_APP_FFMPEG_INCLUDE_PATH)
LOCAL_C_INCLUDES += $(realpath $(LOCAL_PATH)/../ijkyuv/include)
LOCAL_C_INCLUDES += $(realpath $(LOCAL_PATH)/../ijkj4a)
LOCAL_C_INCLUDES += $(realpath $(LOCAL_PATH)/filter)

LOCAL_SRC_FILES += ijksdl_aout.c
LOCAL_SRC_FILES += ijksdl_audio.c
LOCAL_SRC_FILES += ijksdl_egl.c
LOCAL_SRC_FILES += ijksdl_error.c
LOCAL_SRC_FILES += ijksdl_mutex.c
LOCAL_SRC_FILES += ijksdl_stdinc.c
LOCAL_SRC_FILES += ijksdl_thread.c
LOCAL_SRC_FILES += ijksdl_timer.c
LOCAL_SRC_FILES += ijksdl_vout.c
LOCAL_SRC_FILES += gles2/color.c
LOCAL_SRC_FILES += gles2/common.c
LOCAL_SRC_FILES += gles2/renderer.c
LOCAL_SRC_FILES += gles2/renderer_rgb.c
LOCAL_SRC_FILES += gles2/renderer_yuv420p.c
LOCAL_SRC_FILES += gles2/renderer_yuv444p10le.c
LOCAL_SRC_FILES += gles2/shader.c
LOCAL_SRC_FILES += gles2/fsh/rgb.fsh.c
LOCAL_SRC_FILES += gles2/fsh/yuv420p.fsh.c
LOCAL_SRC_FILES += gles2/fsh/yuv444p10le.fsh.c
LOCAL_SRC_FILES += gles2/vsh/mvp.vsh.c

LOCAL_SRC_FILES += sdl_egl.c
LOCAL_SRC_FILES += xmsdl_egl_preview_renderer.c
LOCAL_SRC_FILES += xmsdl_egl_off_screen_renderer.c
LOCAL_SRC_FILES += filter/xm_filter.c
LOCAL_SRC_FILES += filter/xm_filter_factory.c
LOCAL_SRC_FILES += filter/xm_filter_texture_rotate.c
LOCAL_SRC_FILES += filter/base_filter/base_renderer.c
LOCAL_SRC_FILES += filter/base_filter/base_filter.c
LOCAL_SRC_FILES += filter/base_filter/bf_retro.c
LOCAL_SRC_FILES += filter/base_filter/bf_fairytale.c
LOCAL_SRC_FILES += filter/base_filter/bf_blackcat.c
LOCAL_SRC_FILES += filter/base_filter/bf_skinwhiten.c
LOCAL_SRC_FILES += filter/base_filter/bf_sunrise.c
LOCAL_SRC_FILES += filter/base_filter/bf_sunset.c
LOCAL_SRC_FILES += filter/base_filter/bf_sweets.c
LOCAL_SRC_FILES += filter/base_filter/bf_whitecat.c
LOCAL_SRC_FILES += filter/base_filter/bf_antique.c
LOCAL_SRC_FILES += filter/base_filter/bf_romance.c
LOCAL_SRC_FILES += filter/base_filter/bf_sakura.c
LOCAL_SRC_FILES += filter/base_filter/bf_warm.c
LOCAL_SRC_FILES += filter/base_filter/bf_nostalgia.c
LOCAL_SRC_FILES += filter/base_filter/bf_calm.c
LOCAL_SRC_FILES += filter/base_filter/bf_latte.c
LOCAL_SRC_FILES += filter/base_filter/bf_tender.c
LOCAL_SRC_FILES += filter/base_filter/bf_cool.c
LOCAL_SRC_FILES += filter/base_filter/bf_emerald.c
LOCAL_SRC_FILES += filter/base_filter/bf_evergreen.c
LOCAL_SRC_FILES += filter/base_filter/bf_crayon.c
LOCAL_SRC_FILES += filter/base_filter/bf_sketch.c
LOCAL_SRC_FILES += filter/base_filter/bf_amaro.c
LOCAL_SRC_FILES += filter/base_filter/bf_brannan.c
LOCAL_SRC_FILES += filter/base_filter/bf_brooklyn.c
LOCAL_SRC_FILES += filter/base_filter/bf_earlybird.c
LOCAL_SRC_FILES += filter/base_filter/bf_freud.c
LOCAL_SRC_FILES += filter/base_filter/bf_hefe.c
LOCAL_SRC_FILES += filter/base_filter/bf_hudson.c
LOCAL_SRC_FILES += filter/base_filter/bf_inkwell.c
LOCAL_SRC_FILES += filter/base_filter/bf_swirl.c
LOCAL_SRC_FILES += filter/base_filter/bf_sobel_edge_detection.c
LOCAL_SRC_FILES += filter/base_filter/bf_glass_sphere.c
LOCAL_SRC_FILES += filter/base_filter/bf_crosshatch.c
LOCAL_SRC_FILES += filter/base_filter/bf_laplacian.c
LOCAL_SRC_FILES += filter/base_filter/bf_vivgnette.c
LOCAL_SRC_FILES += filter/base_filter/bf_face_sticker.c
LOCAL_SRC_FILES += filter/base_filter/fsh/retro.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/fairytale.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/blackcat.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/skinwhiten.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/sunrise.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/sunset.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/sweets.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/whitecat.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/antique.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/romance.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/sakura.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/warm.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/nostalgia.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/calm.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/latte.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/tender.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/cool.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/emerald.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/evergreen.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/crayon.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/sketch.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/amaro.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/brannan.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/brooklyn.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/earlybird.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/freud.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/hefe.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/hudson.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/inkwell.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/swirl.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/sobel_edge_detection.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/glass_sphere.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/crosshatch.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/laplacian.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/vivgnette.fsh.c
LOCAL_SRC_FILES += filter/base_filter/fsh/bf_face_sticker.fsh.c
LOCAL_SRC_FILES += filter/base_filter/vsh/retro.vsh.c
LOCAL_SRC_FILES += filter/base_filter/vsh/fairytale.vsh.c
LOCAL_SRC_FILES += filter/base_filter/vsh/3x3TextureSampling.vsh.c
LOCAL_SRC_FILES += filter/time_filter/time_filter.c
LOCAL_SRC_FILES += filter/time_filter/tf_ghost.c
LOCAL_SRC_FILES += filter/time_filter/tf_retro.c
LOCAL_SRC_FILES += filter/time_filter/tf_zoom.c
LOCAL_SRC_FILES += filter/time_filter/tf_rotate.c
LOCAL_SRC_FILES += filter/time_filter/tf_fission.c
LOCAL_SRC_FILES += filter/time_filter/tf_spire.c
LOCAL_SRC_FILES += filter/time_filter/tf_sphere.c
LOCAL_SRC_FILES += filter/time_filter/tf_invert.c
LOCAL_SRC_FILES += filter/time_filter/tf_hue.c
LOCAL_SRC_FILES += filter/time_filter/tf_face_sticker.c
LOCAL_SRC_FILES += filter/time_filter/fsh/yuv420p_ghost.fsh.c
LOCAL_SRC_FILES += filter/time_filter/fsh/yuv420p_retro.fsh.c
LOCAL_SRC_FILES += filter/time_filter/fsh/yuv420p_zoom.fsh.c
LOCAL_SRC_FILES += filter/time_filter/fsh/yuv420p_spire.fsh.c
LOCAL_SRC_FILES += filter/time_filter/fsh/yuv420p_sphere.fsh.c
LOCAL_SRC_FILES += filter/time_filter/fsh/yuv420p_invert.fsh.c
LOCAL_SRC_FILES += filter/time_filter/fsh/yuv420p_hue.fsh.c
LOCAL_SRC_FILES += filter/time_filter/fsh/tf_face_sticker.fsh.c
LOCAL_SRC_FILES += filter/time_filter/vsh/ghost.vsh.c
LOCAL_SRC_FILES += filter/time_filter/vsh/zoom.vsh.c
LOCAL_SRC_FILES += filter/time_filter/vsh/rotate.vsh.c
LOCAL_SRC_FILES += filter/time_filter/vsh/face_sticker.vsh.c
LOCAL_SRC_FILES += gles2_xm/color.c
LOCAL_SRC_FILES += gles2_xm/common.c
LOCAL_SRC_FILES += gles2_xm/renderer.c
LOCAL_SRC_FILES += gles2_xm/renderer_rgb.c
LOCAL_SRC_FILES += gles2_xm/renderer_yuv420p.c
LOCAL_SRC_FILES += gles2_xm/renderer_yuv444p10le.c
LOCAL_SRC_FILES += gles2_xm/shader.c
LOCAL_SRC_FILES += gles2_xm/fsh/rgb.fsh.c
LOCAL_SRC_FILES += gles2_xm/fsh/yuv420p.fsh.c
LOCAL_SRC_FILES += gles2_xm/fsh/yuv444p10le.fsh.c
LOCAL_SRC_FILES += gles2_xm/vsh/mvp.vsh.c

LOCAL_SRC_FILES += dummy/ijksdl_vout_dummy.c

LOCAL_SRC_FILES += ffmpeg/ijksdl_vout_overlay_ffmpeg.c
LOCAL_SRC_FILES += ffmpeg/abi_all/image_convert.c

LOCAL_SRC_FILES += android/android_audiotrack.c
LOCAL_SRC_FILES += android/android_nativewindow.c
LOCAL_SRC_FILES += android/ijksdl_android_jni.c
LOCAL_SRC_FILES += android/ijksdl_aout_android_audiotrack.c
LOCAL_SRC_FILES += android/ijksdl_aout_android_opensles.c
LOCAL_SRC_FILES += android/ijksdl_codec_android_mediacodec_dummy.c
LOCAL_SRC_FILES += android/ijksdl_codec_android_mediacodec_internal.c
LOCAL_SRC_FILES += android/ijksdl_codec_android_mediacodec_java.c
LOCAL_SRC_FILES += android/ijksdl_codec_android_mediacodec.c
LOCAL_SRC_FILES += android/ijksdl_codec_android_mediadef.c
LOCAL_SRC_FILES += android/ijksdl_codec_android_mediaformat_java.c
LOCAL_SRC_FILES += android/ijksdl_codec_android_mediaformat.c
LOCAL_SRC_FILES += android/ijksdl_vout_android_nativewindow.c
LOCAL_SRC_FILES += android/ijksdl_vout_android_surface.c
LOCAL_SRC_FILES += android/ijksdl_vout_overlay_android_mediacodec.c

LOCAL_SHARED_LIBRARIES := ijkffmpeg-$(TARGET_ARCH_ABI) ijkj4a
LOCAL_STATIC_LIBRARIES := cpufeatures yuv_static

ifeq ($(USE_MTCNN_NCNN), true)
LOCAL_CFLAGS += -DUSE_MTCNN_NCNN
LOCAL_SRC_FILES += xm_mtcnn_ncnn.c
LOCAL_SHARED_LIBRARIES += xmmtcnn_ncnn-$(TARGET_ARCH_ABI)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../xmmtcnn_ncnn/include
endif

LOCAL_MODULE := ijksdl-$(TARGET_ARCH_ABI)
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/cpufeatures)
