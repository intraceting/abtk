/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#include "abcdk/nvidia/imgproc.h"
#include "../generic/imageproc.hxx"

template <typename T>
ABCDK_INVOKE_HOST void _abcdk_torch_imgproc_stuff_1d(int channels, bool packed,
                                                     T *dst, size_t dst_w, size_t dst_ws, size_t dst_h, T *scalar)
{
    for (size_t i = 0; i < dst_w * dst_h; i++)
    {
        abcdk::generic::imageproc::stuff<T>(channels, packed, dst, dst_w, dst_ws, dst_h, scalar, i);
    }
}

template <typename T>
ABCDK_INVOKE_HOST int _abcdk_torch_imgproc_stuff(int channels, bool packed,
                                                 T *dst, size_t dst_w, size_t dst_ws, size_t dst_h, T *scalar)
{
    assert(dst != NULL && dst_w > 0 && dst_ws > 0 && dst_h > 0 && scalar != NULL);

    _abcdk_torch_imgproc_stuff_1d<T>(channels, packed, dst, dst_w, dst_ws, dst_h, scalar);

    return 0;
}

__BEGIN_DECLS

int abcdk_torch_imgproc_stuff_8u(abcdk_torch_image_t *dst, uint8_t scalar[])
{
    int dst_depth;

    assert(dst != NULL && scalar != NULL);
    assert(dst->pixfmt == ABCDK_TORCH_PIXFMT_GRAY8 ||
           dst->pixfmt == ABCDK_TORCH_PIXFMT_RGB24 ||
           dst->pixfmt == ABCDK_TORCH_PIXFMT_BGR24 ||
           dst->pixfmt == ABCDK_TORCH_PIXFMT_RGB32 ||
           dst->pixfmt == ABCDK_TORCH_PIXFMT_BGR32);

    dst_depth = abcdk_torch_pixfmt_channels(dst->pixfmt);

    return _abcdk_torch_imgproc_stuff<uint8_t>(dst_depth, true, dst->data[0], dst->width, dst->stride[0], dst->height, scalar);
}

__END_DECLS