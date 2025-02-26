/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#ifndef ABCDK_CUDA_VIDEO_DECODER_HXX
#define ABCDK_CUDA_VIDEO_DECODER_HXX

#include "abcdk/util/option.h"
#include "abcdk/cuda/cuda.h"
#include "abcdk/cuda/frame.h"
#include "video_util.cu.hxx"

#ifdef __cuda_cuda_h__

namespace abcdk
{
    namespace cuda
    {
        namespace video
        {
            class decoder
            {
            protected:
                decoder()
                {
                }

                virtual ~decoder()
                {
                    close();
                }

            public:
                virtual void close()
                {
                }

                virtual int open(abcdk_option_t *cfg) = 0;

                virtual int sync(AVCodecContext *opt) = 0;

                virtual int update(abcdk_media_frame_t **dst, const abcdk_media_packet_t *src) = 0;
            };
        } // namespace video
    } // namespace cuda
} // namespace abcdk


#endif // __cuda_cuda_h__

#endif // ABCDK_CUDA_VIDEO_DECODER_HXX