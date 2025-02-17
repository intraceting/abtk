/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#ifndef ABCDK_CUDA_JPEG_ENCODER_X86_64_HXX
#define ABCDK_CUDA_JPEG_ENCODER_X86_64_HXX

#include "abcdk/util/option.h"
#include "abcdk/cuda/cuda.h"
#include "abcdk/cuda/avutil.h"
#include "util.cu.hxx"
#include "jpeg_encoder.cu.hxx"

#ifdef __cuda_cuda_h__
#ifdef AVUTIL_AVUTIL_H
#ifdef __x86_64__

namespace abcdk
{
    namespace cuda
    {
        namespace jpeg
        {
            class encoder_x86_64 : public encoder
            {
            public:
                static encoder *create()
                {
                    encoder *ctx = new encoder_x86_64();
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

                    delete (encoder_x86_64 *)ctx_p;
                }

            private:
                abcdk_option_t *m_cfg;

                cudaStream_t m_stream;
                nvjpegHandle_t m_ctx;
                nvjpegEncoderState_t m_state;
                nvjpegEncoderParams_t m_params;

            public:
                encoder_x86_64()
                {
                    m_cfg = NULL;
                    m_stream = NULL;
                    m_ctx = NULL;
                    m_state = NULL;
                    m_params = NULL;
                }

                virtual ~encoder_x86_64()
                {
                    close();
                }

            public:
                virtual void close()
                {
                    if (m_params)
                        nvjpegEncoderParamsDestroy(m_params);
                    m_params = NULL;
                    if (m_state)
                        nvjpegEncoderStateDestroy(m_state);
                    m_state = NULL;
                    if (m_ctx)
                        nvjpegDestroy(m_ctx);
                    m_ctx = NULL;
                    if (m_stream)
                        cudaStreamDestroy(m_stream);
                    m_stream = NULL;

                    abcdk_option_free(&m_cfg);
                }

                virtual int open(abcdk_option_t *cfg)
                {
                    int quality;
                    cudaError_t cuda_chk;
                    nvjpegStatus_t jpeg_chk;

                    assert(m_cfg == NULL);

                    m_cfg = abcdk_option_alloc("--");
                    if (!m_cfg)
                        return -1;

                    if (cfg)
                        abcdk_option_merge(m_cfg, cfg);

                    cuda_chk = cudaStreamCreateWithFlags(&m_stream, cudaStreamDefault);
                    if (cuda_chk != cudaSuccess)
                        return -1;

                    jpeg_chk = nvjpegCreateSimple(&m_ctx);
                    if (jpeg_chk != NVJPEG_STATUS_SUCCESS)
                        return -1;

                    quality = abcdk_option_get_int(m_cfg, "--quality", 0, 99);
                    quality = ABCDK_CLAMP(quality, 1, 99);

                    nvjpegEncoderStateCreate(m_ctx, &m_state, m_stream);
                    nvjpegEncoderParamsCreate(m_ctx, &m_params, m_stream);
                    nvjpegEncoderParamsSetQuality(m_params, quality, m_stream);
                    nvjpegEncoderParamsSetSamplingFactors(m_params, NVJPEG_CSS_420, m_stream);

                    return 0;
                }

                virtual abcdk_object_t *update(const AVFrame *src)
                {
                    AVFrame *tmp_src = NULL;
                    abcdk_object_t *dst;
                    size_t dst_size = 0;
                    nvjpegImage_t src_data = {0};
                    nvjpegStatus_t jpeg_chk;
                    int chk;

                    if (src->format != (int)AV_PIX_FMT_RGB24 && src->format != (int)AV_PIX_FMT_BGR24)
                    {
                        tmp_src = abcdk_avframe_alloc(src->width, src->height, AV_PIX_FMT_RGB24, 4);
                        if (!tmp_src)
                            return NULL;

                        chk = abcdk_cuda_avframe_convert(tmp_src, src); // 转换格式。

                        if (chk == 0)
                            dst = update(tmp_src);

                        av_frame_free(&tmp_src);

                        return dst;
                    }

                    src_data.channel[0] = src->data[0];
                    src_data.pitch[0] = src->linesize[0];
                    
                    if (src->format == (int)AV_PIX_FMT_RGB24)
                        jpeg_chk = nvjpegEncodeImage(m_ctx, m_state, m_params, &src_data, NVJPEG_INPUT_RGBI, src->width, src->height, m_stream);
                    else if (src->format == (int)AV_PIX_FMT_BGR24)
                        jpeg_chk = nvjpegEncodeImage(m_ctx, m_state, m_params, &src_data, NVJPEG_INPUT_BGRI, src->width, src->height, m_stream);

                    if(jpeg_chk != NVJPEG_STATUS_SUCCESS)
                        return NULL;

                    jpeg_chk = nvjpegEncodeRetrieveBitstream(m_ctx, m_state, NULL, &dst_size, m_stream);
                    if(jpeg_chk != NVJPEG_STATUS_SUCCESS)
                        return NULL;

           
                    dst = abcdk_object_alloc2(dst_size);
                    if(!dst)
                        return NULL;

                    jpeg_chk = nvjpegEncodeRetrieveBitstream(m_ctx, m_state,dst->pptrs[0], &dst_size, m_stream);
                    if(jpeg_chk != NVJPEG_STATUS_SUCCESS)
                        goto ERR;

                    return dst;

                ERR:
                    abcdk_object_unref(&dst);

                    return NULL;
                }

                virtual int update(const char *dst, const AVFrame *src)
                {
                    abcdk_object_t *dst_data;
                    ssize_t save_chk;

                    assert(dst != NULL && src != NULL);

                    dst_data = update(src);
                    if (!dst_data)
                        return -1;

                    truncate(dst, 0);

                    save_chk = abcdk_save(dst, dst_data->pptrs[0], dst_data->sizes[0], 0);
                    if (save_chk != dst_data->sizes[0])
                    {
                        abcdk_object_unref(&dst_data);
                        return -1;
                    }

                    abcdk_object_unref(&dst_data);
                    return 0;
                }
            };
        } // namespace jpeg
    } // namespace cuda
} // namespace abcdk

#endif //__x86_64__
#endif //AVUTIL_AVUTIL_H
#endif // __cuda_cuda_h__

#endif // ABCDK_CUDA_JPEG_ENCODER_X86_64_HXX