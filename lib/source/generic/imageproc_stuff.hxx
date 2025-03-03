/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#ifndef ABCDK_GENERIC_IMAGEPROC_STUFF_HXX
#define ABCDK_GENERIC_IMAGEPROC_STUFF_HXX

#include "util.hxx"

namespace abcdk
{
    namespace generic
    {
        namespace imageproc
        {
            template <typename T>
            ABCDK_INVOKE_DEVICE void stuff(int channels, bool packed, T *dst, size_t width, size_t pitch, size_t height, T *scalar, size_t tid)
            {

                size_t y = tid / width;
                size_t x = tid % width;

                if (x >= width || y >= height)
                    return;

                for (size_t i = 0; i < channels; i++)
                {
                    size_t offset = abcdk::generic::util::off<T>(packed, width, pitch, height, channels, 0, x, y, i);

                    *abcdk::generic::util::ptr<T>(dst, offset) = (scalar ? scalar[i] : (T)0);
                }
            }


        } // namespace imageproc
    } //    namespace generic
} // namespace abcdk

#endif // ABCDK_GENERIC_IMAGEPROC_STUFF_HXX