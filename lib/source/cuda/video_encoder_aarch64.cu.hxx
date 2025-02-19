/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#ifndef ABCDK_CUDA_VIDEO_ENCODER_AARCH64_HXX
#define ABCDK_CUDA_VIDEO_ENCODER_AARCH64_HXX

#include "abcdk/util/option.h"
#include "abcdk/cuda/cuda.h"
#include "abcdk/cuda/avutil.h"
#include "abcdk/cuda/device.h"
#include "video_encoder.cu.hxx"
#include "video_util.cu.hxx"

#ifdef __cuda_cuda_h__
#ifdef AVUTIL_AVUTIL_H
#ifdef AVCODEC_AVCODEC_H
#ifdef __aarch64__

namespace abcdk
{
    namespace cuda
    {
        namespace video
        {
            class encoder_aarch64 : public encoder
            {
            public:
                static encoder *create()
                {
                    encoder *ctx = new encoder_aarch64();
                    if (!ctx)
                        return NULL;

                    return ctx;
                }

                static void destory(encoder **ctx)
                {
                    encoder *ctx_p;

                    if (!ctx || !*ctx)
                        return;

                    ctx_p = *ctx;
                    *ctx = NULL;

                    delete (encoder_aarch64 *)ctx_p;
                }

            private:
                CUcontext m_gpu_ctx;
                nvmpictx *m_encoder;

                abcdk_option_t *m_cfg;

            public:
                encoder_aarch64()
                {
                    m_gpu_ctx = NULL;
                    m_encoder = NULL;

                    m_cfg = NULL;
                }

                virtual ~encoder_aarch64()
                {
                    close();
                }

            protected:
                void GetSequenceParams(nvEncParam *param, std::vector<uint8_t> &seqParams)
                {
                    AVFrame *tmp;
                    std::vector<uint8_t> out;

                    /*创建一个图像，用于编码。*/
                    tmp = abcdk_cuda_avframe_alloc(param->width, param->height, AV_PIX_FMT_YUV420P, 1);
                    if (!tmp)
                        return;

                    /*在100帧内编码器总能输出扩展数据了。*/
                    for (int i = 0; i < 100; i++)
                    {
                        if (encode(tmp, out) < 0)
                            break;

                        /*取第一个数据帧就可以了。*/
                        if (out.size() > 0)
                            break;
                    }

                    av_frame_free(&tmp);

                    if (out.size() <= 0)
                        return;

                    // H264 find IDR index 0x00 0x00 0x00 0x01 0x65
                    if (param->profile == FF_PROFILE_H264_MAIN)
                    {
                        int i = 0;
                        while (1)
                        {

                            if (out.size() < i + 5)
                                break;

                            if ((out[i] != 0 || out[i + 1] != 0 || out[i + 2] != 0 || out[i + 3] != 0x01 || out[i + 4] != 0x65)) // 找到关键帧开始的地方，在这之前的数据就是编码器扩展数据。
                                i++;
                            else
                                break;
                        }

                        if (i + 5 <= out.size())
                        {
                            seqParams.resize(i);
                            memcpy(seqParams.data(), out.data(), i);
                        }
                    }
                    // H265 find IDR index 0x00 0x00 0x00 0x01 0x26 0x01
                    else if (param->profile == FF_PROFILE_HEVC_MAIN)
                    {
                        int i = 0;
                        while (1)
                        {

                            if (out.size() < i + 6)
                                break;

                            if ((out[i] != 0 || out[i + 1] != 0 || out[i + 2] != 0 || out[i + 3] != 0x01 || out[i + 4] != 0x26 || out[i + 5] != 0x01)) // 找到关键帧开始的地方，在这之前的数据就是编码器扩展数据。
                                i++;
                            else
                                break;
                        }

                        if (i + 6 <= out.size())
                        {
                            seqParams.resize(i);
                            memcpy(seqParams.data(), out.data(), i);
                        }
                    }
                }

                int encode(const AVFrame *img, std::vector<uint8_t> &out)
                {
                    int frame_height[4] = {0};
                    nvFrame frame = {0};
                    nvPacket packet = {0};
                    int chk;

                    if (!m_encoder)
                        return -1;

                    abcdk::cuda::context::robot robot(m_gpu_ctx);

                    if (img)
                    {
                        frame.payload[0] = img->data[0];
                        frame.payload[1] = img->data[1];
                        frame.payload[2] = img->data[2];

                        abcdk_avimage_fill_heights(frame_height, img->height, (enum AVPixelFormat)img->format);
                        frame.payload_size[0] = img->linesize[0] * frame_height[0];
                        frame.payload_size[1] = img->linesize[1] * frame_height[1];
                        frame.payload_size[2] = img->linesize[2] * frame_height[2];

                        frame.timestamp = img->pts;

                        chk = nvmpi_encoder_put_frame(m_encoder, &frame);
                        if (chk != 0)
                            return -1;
                    }

                    chk = nvmpi_encoder_get_packet(m_encoder, &packet);
                    if (chk != 0)
                        return -1;

                    if (packet.payload_size <= 0)
                        return 0;

                    out.clear();
                    out.insert(out.end(), &packet.payload[0], &packet.payload[packet.payload_size]);

                    return 1;
                }

            public:
                virtual void close()
                {
                    if (m_gpu_ctx)
                        cuCtxPushCurrent(m_gpu_ctx);
                    if (m_encoder)
                        nvmpi_encoder_close(m_encoder);
                    m_encoder = NULL;

                    if (m_gpu_ctx)
                        cuCtxPopCurrent(NULL);

                    abcdk_cuda_ctx_destroy(&m_gpu_ctx);

                    abcdk_option_free(&m_cfg);
                }

                virtual int open(abcdk_option_t *cfg)
                {
                    int device;

                    assert(m_cfg == NULL);

                    m_cfg = abcdk_option_alloc("--");
                    if (!m_cfg)
                        return -1;

                    if (cfg)
                        abcdk_option_merge(m_cfg, cfg);

                    device = abcdk_option_get_int(m_cfg, "--device", 0, 0);

                    m_gpu_ctx = abcdk_cuda_ctx_create(device, 0);
                    if (!m_gpu_ctx)
                        return -1;

                    return 0;
                }

                virtual int sync(AVCodecContext *opt)
                {
                    int fps, width, height, flags;
                    nvCodingType nvcodec_id;
                    std::vector<uint8_t> ext_data;
                    nvEncParam param = {0};

                    assert(opt != NULL);

                    fps = abcdk_avmatch_r2d(opt->framerate, 1);
                    width = opt->width;
                    height = opt->height;
                    flags = opt->flags;
                    nvcodec_id = (nvCodingType)codecid_ffmpeg_to_nvcodec(opt->codec_id);

                    if (fps > 1000 || fps <= 0)
                        return -1;

                    if (width <= 0 || height <= 0)
                        return -1;

                    if (NV_VIDEO_CodingH264 != nvcodec_id && NV_VIDEO_CodingHEVC != nvcodec_id)
                        return -1;

                    abcdk::cuda::context::robot robot(m_gpu_ctx);

                    param.width = width;
                    param.height = height;
                    param.bitrate = 4000000;
                    param.mode_vbr = 0;
                    param.idr_interval = fps;
                    param.iframe_interval = fps;
                    param.max_b_frames = 0;
                    param.peak_bitrate = 0;
                    param.fps_n = 1;
                    param.fps_d = fps;
                    if (nvcodec_id == NV_VIDEO_CodingH264)
                        param.profile = FF_PROFILE_H264_MAIN;
                    if (nvcodec_id == NV_VIDEO_CodingHEVC)
                        param.profile = FF_PROFILE_HEVC_MAIN;
                    param.level = -99;
                    param.capture_num = 10;
                    param.hw_preset_type = 3;
                    param.insert_spspps_idr = (flags & AVFMT_GLOBALHEADER ? 0 : 1);

                    param.qmax = -1;
                    param.qmin = -1;

                    if (flags & AVFMT_GLOBALHEADER)
                    {
                        /*创建临时对象用于生成编码器扩展数据帧。*/
                        m_encoder = nvmpi_create_encoder(nvcodec_id, &param);
                        if (m_encoder)
                        {
                            GetSequenceParams(&param, ext_data);
                            nvmpi_encoder_close(m_encoder);
                            m_encoder = NULL;
                        }
                    }

                    m_encoder = nvmpi_create_encoder(nvcodec_id, &param);
                    if (!m_encoder)
                        return -1;

                    /*输出扩展数据帧。*/
                    if (ext_data.size() > 0)
                    {
                        if (opt->extradata)
                        {
                            av_free(opt->extradata);
                            opt->extradata = NULL;
                            opt->extradata_size = 0;
                        }

                        opt->extradata = (uint8_t *)av_memdup(ext_data.data(), ext_data.size());
                        opt->extradata_size = ext_data.size();
                    }

                    return 0;
                }

                virtual int update(AVPacket **dst, const AVFrame *src)
                {
                    AVFrame *tmp_src = NULL;
                    std::vector<uint8_t> out;
                    int chk;

                    assert(dst != NULL);

                    if (!m_encoder)
                        return -1;

                    abcdk::cuda::context::robot robot(m_gpu_ctx);

                    if (src)
                    {
                        if (src->format != (int)AV_PIX_FMT_YUV420P)
                        {
                            tmp_src = abcdk_cuda_avframe_alloc(src->width, src->height, AV_PIX_FMT_YUV420P, 1);
                            if (!tmp_src)
                                return -1;

                            chk = abcdk_cuda_avframe_convert(tmp_src, src); // 转换格式。

                            if (chk == 0)
                                chk = update(dst, tmp_src);

                            av_frame_free(&tmp_src);

                            return chk;
                        }

                        chk = encode(src, out);
                        if (chk < 0)
                            return -1;
                    }
                    else 
                    {
                        chk = encode(NULL, out);
                        if (chk < 0)
                            return -1;
                    }
                    

                    if (out.size() <= 0)
                        return 0;

                    *dst = av_packet_alloc();
                    if (!*dst)
                        return -1;

                    chk = av_grow_packet(*dst, out.size());
                    if (chk != 0)
                    {
                        av_packet_free(dst);
                        return -1;
                    }

                    memcpy((*dst)->data, out.data(), out.size());

                    return 1;
                }
            };
        } // namespace video
    } // namespace cuda
} // namespace abcdk

#endif // __aarch64__
#endif // AVCODEC_AVCODEC_H
#endif // AVUTIL_AVUTIL_H
#endif // __cuda_cuda_h__

#endif // ABCDK_CUDA_VIDEO_ENCODER_AARCH64_HXX