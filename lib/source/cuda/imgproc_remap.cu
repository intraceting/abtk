/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#include "abcdk/cuda/imgproc.h"

#ifdef __cuda_cuda_h__

int abcdk_cuda_imgproc_remap_8u(int channels, int packed,
                                uint8_t *dst, size_t dst_w, size_t dst_ws, size_t dst_h, const NppiRect *dst_roi,
                                const uint8_t *src, size_t src_w, size_t src_ws, size_t src_h, const NppiRect *src_roi,
                                const float *xmap, size_t xmap_ws, const float *ymap, size_t ymap_ws,
                                NppiInterpolationMode inter_mode)
{
    NppiSize tmp_dst_size = {0}, tmp_src_size = {0};
    NppiRect tmp_src_roi = {0};
    NppStatus npp_chk = NPP_NOT_IMPLEMENTED_ERROR;

    assert(channels == 1 || channels == 3 || channels == 4);
    assert(dst != NULL && dst_w > 0 && dst_ws > 0 && dst_h > 0);
    assert(src != NULL && src_w > 0 && src_ws > 0 && src_h > 0);
    assert(xmap != NULL && xmap_ws > 0);
    assert(ymap != NULL && ymap_ws > 0);

    tmp_dst_size.width = dst_w;
    tmp_dst_size.height = dst_h;

    tmp_src_size.width = src_w;
    tmp_src_size.height = src_h;

    tmp_src_roi.x = (src_roi ? src_roi->x : 0);
    tmp_src_roi.y = (src_roi ? src_roi->y : 0);
    tmp_src_roi.width = (src_roi ? src_roi->width : src_w);
    tmp_src_roi.height = (src_roi ? src_roi->height : src_h);

    if (channels == 1)
    {
        npp_chk = nppiRemap_8u_C1R(src, tmp_src_size, src_ws, tmp_src_roi,
                                   xmap, xmap_ws, ymap, ymap_ws,
                                   dst, dst_ws, tmp_dst_size,
                                   inter_mode);
    }
    else if (channels == 3)
    {
        npp_chk = nppiRemap_8u_C3R(src, tmp_src_size, src_ws, tmp_src_roi,
                                   xmap, xmap_ws, ymap, ymap_ws,
                                   dst, dst_ws, tmp_dst_size,
                                   inter_mode);
    }
    else if (channels == 4)
    {
        npp_chk = nppiRemap_8u_C4R(src, tmp_src_size, src_ws, tmp_src_roi,
                                   xmap, xmap_ws, ymap, ymap_ws,
                                   dst, dst_ws, tmp_dst_size,
                                   inter_mode);
    }

    if (npp_chk != NPP_SUCCESS)
        return -1;

    return 0;
}

#endif //__cuda_cuda_h__