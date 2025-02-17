/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#ifndef ABCDK_CUDA_JPEG_DECODER_X86_64_HXX
#define ABCDK_CUDA_JPEG_DECODER_X86_64_HXX

#include "abcdk/util/option.h"
#include "abcdk/cuda/cuda.h"
#include "abcdk/cuda/avutil.h"
#include "util.cu.hxx"
#include "jpeg_decoder.cu.hxx"

#ifdef __cuda_cuda_h__
#ifdef AVUTIL_AVUTIL_H
#ifdef __x86_64__

namespace abcdk
{
    namespace cuda
    {
        namespace jpeg
        {
            class decoder_x86_64 : public decoder
            {
            public:
                static decoder *create()
                {
                    decoder *ctx = new decoder_x86_64();
                    if (!ctx)
                        return NULL;

                    return ctx;
                }

                static void destory(decoder **ctx)
                {
                    decoder *ctx_p;

                    if (!ctx || !*ctx)
                        return;

                    ctx_p = *ctx;
                    *ctx = NULL;

                    delete (decoder_x86_64 *)ctx_p;
                }

                static int dev_malloc(void **pptr, size_t size)
                {
                    cudaError_t cuda_chk;

                    cuda_chk = cudaMalloc(pptr, size);
                    if (cuda_chk != cudaSuccess)
                        return -1;

                    return 0;
                }

                static int dev_free(void *ptr)
                {
                    cudaError_t cuda_chk;

                    cuda_chk = cudaFree(ptr);
                    if (cuda_chk != cudaSuccess)
                        return -1;

                    return 0;
                }

                static int pinned_malloc(void **pptr, size_t size, unsigned int flags)
                {
                    cudaError_t cuda_chk;

                    cuda_chk = cudaHostAlloc(pptr, size, flags);
                    if (cuda_chk != cudaSuccess)
                        return -1;

                    return 0;
                }

                static int pinned_free(void *ptr)
                {
                    cudaError_t cuda_chk;

                    cuda_chk = cudaFreeHost(ptr);
                    if (cuda_chk != cudaSuccess)
                        return -1;

                    return 0;
                }

            private:
                abcdk_option_t *m_cfg;

                cudaStream_t m_stream;
                nvjpegHandle_t m_ctx;
                nvjpegJpegState_t m_state;

            public:
                decoder_x86_64()
                {
                    m_cfg = NULL;
                    m_stream = NULL;
                    m_ctx = NULL;
                    m_state = NULL;
                }

                virtual ~decoder_x86_64()
                {
                    close();
                }

            private:
                void check_memory_leak_version()
                {
                    int cuda_major_ver = -1, cuda_minor_ver = -1;

                    cuda_major_ver = abcdk_cuda_get_runtime_version(&cuda_minor_ver);

                    if (cuda_major_ver == 11)
                    {
                        if (cuda_minor_ver >= 0 && cuda_minor_ver <= 5)
                            abcdk_trace_printf(LOG_WARNING, "CUDA11.0~CUDA11.5运行库中的nvJPEG解码器存在内存泄漏问题，谨慎使用。");
                    }
                }

            public:
                virtual void close()
                {
                   if (m_state)
                       nvjpegJpegStateDestroy(m_state);
                   m_state = NULL;
                   if (m_ctx)
                       nvjpegDestroy(m_ctx);
                   m_ctx = NULL;
                    if(m_stream)
                        cudaStreamDestroy(m_stream);
                    m_stream = NULL;

                    abcdk_option_free(&m_cfg);
                }


                virtual int open(abcdk_option_t *cfg)
                {
                    cudaError_t cuda_chk;
                    nvjpegStatus_t jpeg_chk;
                    nvjpegDevAllocator_t dev_allocator = {0};
                    nvjpegPinnedAllocator_t pinned_allocator = {0};
                    
                    assert(m_cfg == NULL);
                    
                    check_memory_leak_version();

                    m_cfg = abcdk_option_alloc("--");
                    if (!m_cfg)
                        return -1;

                    if (cfg)
                        abcdk_option_merge(m_cfg, cfg);
                    
                    cuda_chk = cudaStreamCreateWithFlags(&m_stream, cudaStreamDefault);
                    if (cuda_chk != cudaSuccess)
                        return -1;

                    dev_allocator.dev_malloc = dev_malloc;
                    dev_allocator.dev_free = dev_free;
                    pinned_allocator.pinned_malloc = pinned_malloc;
                    pinned_allocator.pinned_free = pinned_free;

                    jpeg_chk = nvjpegCreateEx(NVJPEG_BACKEND_HYBRID, &dev_allocator, &pinned_allocator, NVJPEG_FLAGS_DEFAULT, &m_ctx);
                    if (jpeg_chk != NVJPEG_STATUS_SUCCESS)
                        return -1;

                    jpeg_chk = nvjpegJpegStateCreate(m_ctx, &m_state);
                    if (jpeg_chk != NVJPEG_STATUS_SUCCESS)
                        return -1;

                    return 0;
                }

                virtual AVFrame *update(const void *src, int src_size)
                {
                    AVFrame *dst;
                    nvjpegImage_t dst_data = {0};
                    int components;
                    nvjpegChromaSubsampling_t subsampling;
                    int width[NVJPEG_MAX_COMPONENT] = {0}, height[NVJPEG_MAX_COMPONENT] = {0};
                    nvjpegStatus_t jpeg_chk;

                    assert(src != NULL && src_size > 0);

                    jpeg_chk = nvjpegGetImageInfo(m_ctx, (uint8_t *)src, src_size, &components, &subsampling, width, height);
                    if (jpeg_chk != NVJPEG_STATUS_SUCCESS)
                        return NULL;
                    
                    /*全部转成RGB和GRAY。*/
                    if (components == 3)
                        dst = abcdk_cuda_avframe_alloc(width[0], height[0], AV_PIX_FMT_RGB24, 4);//只使用第一层的宽和高。
                    else if (components == 1)
                        dst = abcdk_cuda_avframe_alloc(width[0], height[0], AV_PIX_FMT_GRAY8, 1);
                    else
                        return NULL;

                    for (int i = 0; i < 4; i++)
                    {
                        dst_data.channel[i] = dst->data[i];
                        dst_data.pitch[i] = dst->linesize[i];
                    }

                    if (components == 3)
                        jpeg_chk = nvjpegDecode(m_ctx, m_state, (uint8_t *)src, src_size, NVJPEG_OUTPUT_RGBI, &dst_data, m_stream);
                    else if (components == 1)
                        jpeg_chk = nvjpegDecode(m_ctx, m_state, (uint8_t *)src, src_size, NVJPEG_OUTPUT_Y, &dst_data, m_stream);
                    else
                        goto ERR;

                    if (jpeg_chk != NVJPEG_STATUS_SUCCESS)
                        goto ERR;

                    return dst;

                ERR:

                    av_frame_free(&dst);
                    return NULL;
                }

                virtual AVFrame *update(const void *src)
                {
                    abcdk_object_t *src_data;
                    AVFrame *dst;

                    assert(src != NULL);

                    src_data = abcdk_object_copyfrom_file(src);
                    if(!src_data)
                        return NULL;

                    dst = update(src_data->pptrs[0],src_data->sizes[0]);
                    abcdk_object_unref(&src_data);

                    return dst;
                }
            };
        } // namespace jpeg
    } // namespace cuda
} // namespace abcdk

#endif //__x86_64__
#endif // AVUTIL_AVUTIL_H
#endif // __cuda_cuda_h__

#endif // ABCDK_CUDA_JPEG_DECODER_X86_64_HXX