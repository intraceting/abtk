/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#ifndef ABCDK_CUDA_JPEG_UTIL_HXX
#define ABCDK_CUDA_JPEG_UTIL_HXX

#include "abcdk/cuda/cuda.h"
#include "abcdk/cuda/avutil.h"
#include "context_robot.cu.hxx"
#include "impl/log_streambuf.hxx"

#ifdef __aarch64__
#include "jetson/nvmpi.h"
#include "jetson/NvJpegDecoder.h"
#include "jetson/NvJpegEncoder.h"
#endif //__aarch64__

#ifdef __cuda_cuda_h__
#ifdef AVUTIL_AVUTIL_H
#ifdef AVCODEC_AVCODEC_H

namespace abcdk
{
    namespace cuda
    {
        namespace jpeg
        {

        } // namespace jpeg
    } // namespace cuda
} // namespace abcdk

#endif // AVCODEC_AVCODEC_H
#endif // AVUTIL_AVUTIL_H
#endif //__cuda_cuda_h__

#endif // ABCDK_CUDA_JPEG_UTIL_HXX