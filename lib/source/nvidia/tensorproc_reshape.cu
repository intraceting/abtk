/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#include "abcdk/nvidia/tensorproc.h"
#include "../generic/tensorproc.hxx"
#include "grid.cu.hxx"

#ifdef __cuda_cuda_h__

template <typename T>
ABCDK_INVOKE_GLOBAL void _abcdk_cuda_tensorproc_reshape_2d2d(bool dst_packed, T *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
                                                             bool src_packed, T *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    size_t tid = abcdk::cuda::grid::get_tid(2, 2);

    abcdk::generic::tensorproc::reshape_kernel<T>(dst_packed, dst, dst_b, dst_w, dst_ws, dst_h, dst_c, src_packed, src, src_b, src_w, src_ws, src_h, src_c, tid);
}

template <typename T>
ABCDK_INVOKE_HOST int _abcdk_cuda_tensorproc_reshape(bool dst_packed, T *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
                                                     bool src_packed, T *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    size_t dst_total, src_total;
    uint3 dim[2];

    assert(dst != NULL && dst_b > 0 && dst_w > 0 && dst_ws > 0 && dst_h > 0 && dst_c > 0);
    assert(dst != NULL && src_b > 0 && src_w > 0 && src_ws > 0 && src_h > 0 && src_c > 0);

    assert(dst_packed ? (dst_ws >= dst_w * dst_c * sizeof(T)) : (dst_ws >= dst_w * sizeof(T)));
    assert(src_packed ? (src_ws >= src_w * src_c * sizeof(T)) : (src_ws >= src_w * sizeof(T)));

    dst_total = dst_b * dst_w * dst_h * dst_c;
    src_total = src_b * src_w * src_h * src_c;

    assert(dst_total == src_total);

    /*2D-2D*/
    abcdk::cuda::grid::make_dim_dim(dim, dst_total, 64);

    _abcdk_cuda_tensorproc_reshape_2d2d<T><<<dim[0], dim[1]>>>(dst_packed, dst, dst_b, dst_w, dst_ws, dst_h, dst_c, src_packed, src, src_b, src_w, src_ws, src_h, src_c);

    return 0;
}


__BEGIN_DECLS

int abcdk_cuda_tensorproc_reshape_8u(int dst_packed, uint8_t *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
                                     int src_packed, uint8_t *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    return _abcdk_cuda_tensorproc_reshape<uint8_t>(dst_packed, dst, dst_b, dst_w, dst_ws, dst_h, dst_c, src_packed, src, src_b, src_w, src_ws, src_h, src_c);
}

int abcdk_cuda_tensorproc_reshape_16u(int dst_packed, uint16_t *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
                                      int src_packed, uint16_t *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    return _abcdk_cuda_tensorproc_reshape<uint16_t>(dst_packed, dst, dst_b, dst_w, dst_ws, dst_h, dst_c, src_packed, src, src_b, src_w, src_ws, src_h, src_c);
}

int abcdk_cuda_tensorproc_reshape_32u(int dst_packed, uint32_t *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
                                      int src_packed, uint32_t *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    return _abcdk_cuda_tensorproc_reshape<uint32_t>(dst_packed, dst, dst_b, dst_w, dst_ws, dst_h, dst_c, src_packed, src, src_b, src_w, src_ws, src_h, src_c);
}

int abcdk_cuda_tensorproc_reshape_64u(int dst_packed, uint64_t *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
                                      int src_packed, uint64_t *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    return _abcdk_cuda_tensorproc_reshape<uint64_t>(dst_packed, dst, dst_b, dst_w, dst_ws, dst_h, dst_c, src_packed, src, src_b, src_w, src_ws, src_h, src_c);
}

int abcdk_cuda_tensorproc_reshape_32f(int dst_packed, float *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
                                      int src_packed, float *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    return _abcdk_cuda_tensorproc_reshape<float>(dst_packed, dst, dst_b, dst_w, dst_ws, dst_h, dst_c, src_packed, src, src_b, src_w, src_ws, src_h, src_c);
}

int abcdk_cuda_tensorproc_reshape_64f(int dst_packed, double *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
                                      int src_packed, double *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    return _abcdk_cuda_tensorproc_reshape<double>(dst_packed, dst, dst_b, dst_w, dst_ws, dst_h, dst_c, src_packed, src, src_b, src_w, src_ws, src_h, src_c);
}

__END_DECLS

#else //__cuda_cuda_h__


__BEGIN_DECLS

int abcdk_cuda_tensorproc_reshape_8u(int dst_packed, uint8_t *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
    int src_packed, uint8_t *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    abcdk_trace_printf(LOG_WARNING, "当前环境在构建时未包含CUDA工具。");
    return -1;
}

int abcdk_cuda_tensorproc_reshape_16u(int dst_packed, uint16_t *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
     int src_packed, uint16_t *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    abcdk_trace_printf(LOG_WARNING, "当前环境在构建时未包含CUDA工具。");
    return -1;
}

int abcdk_cuda_tensorproc_reshape_32u(int dst_packed, uint32_t *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
     int src_packed, uint32_t *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    abcdk_trace_printf(LOG_WARNING, "当前环境在构建时未包含CUDA工具。");
    return -1;
}

int abcdk_cuda_tensorproc_reshape_64u(int dst_packed, uint64_t *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
     int src_packed, uint64_t *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    abcdk_trace_printf(LOG_WARNING, "当前环境在构建时未包含CUDA工具。");
    return -1;
}

int abcdk_cuda_tensorproc_reshape_32f(int dst_packed, float *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
     int src_packed, float *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    abcdk_trace_printf(LOG_WARNING, "当前环境在构建时未包含CUDA工具。");
    return -1;
}

int abcdk_cuda_tensorproc_reshape_64f(int dst_packed, double *dst, size_t dst_b, size_t dst_w, size_t dst_ws, size_t dst_h, size_t dst_c,
     int src_packed, double *src, size_t src_b, size_t src_w, size_t src_ws, size_t src_h, size_t src_c)
{
    abcdk_trace_printf(LOG_WARNING, "当前环境在构建时未包含CUDA工具。");
    return -1;
}

__END_DECLS

#endif // __cuda_cuda_h__
