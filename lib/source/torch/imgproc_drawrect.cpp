/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#include "abcdk/nvidia/imgproc.h"
#include "../generic/imageproc.hxx"

template <typename T>
ABCDK_INVOKE_HOST void _abcdk_torch_imgproc_drawrect_1d(int channels, bool packed,
                                                        T *dst, size_t w, size_t ws, size_t h,
                                                        T *color, int weight, int *corner)
{

    // #pragma omp parallel
    for (size_t i = 0; i < w * h; i++)
    {
        abcdk::generic::imageproc::drawrect<T>(channels, packed, dst, w, ws, h, color, weight, corner, i);
    }
}

template <typename T>
ABCDK_INVOKE_HOST int _abcdk_torch_imgproc_drawrect(int channels, bool packed,
                                                    T *dst, size_t w, size_t ws, size_t h,
                                                    T *color, int weight, int *corner)
{
    assert(dst != NULL && w > 0 && ws > 0 && h > 0);
    assert(color != NULL && weight > 0 && corner != NULL);

    _abcdk_torch_imgproc_drawrect_1d<T>(channels, packed, dst, w, ws, h, color, weight, corner);

    return 0;
}

__BEGIN_DECLS

int abcdk_torch_imgproc_drawrect_8u(abcdk_torch_image_t *dst, uint8_t color[], int weight, int corner[4])
{
    int dst_depth;

    assert(dst != NULL && color != NULL  && weight > 0 && corner != NULL);
    assert(dst->pixfmt == ABCDK_TORCH_PIXFMT_GRAY8 ||
           dst->pixfmt == ABCDK_TORCH_PIXFMT_RGB24 ||
           dst->pixfmt == ABCDK_TORCH_PIXFMT_BGR24 ||
           dst->pixfmt == ABCDK_TORCH_PIXFMT_RGB32 ||
           dst->pixfmt == ABCDK_TORCH_PIXFMT_BGR32);

    dst_depth = abcdk_torch_pixfmt_channels(dst->pixfmt);

    return _abcdk_torch_imgproc_drawrect<uint8_t>(dst_depth, true, dst->data[0], dst->width, dst->stride[0], dst->height, color, weight, corner);
}

__END_DECLS