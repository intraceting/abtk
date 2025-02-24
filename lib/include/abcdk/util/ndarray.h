/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2021 The ABCDK project authors. All Rights Reserved.
 *
 */
#ifndef ABCDK_UTIL_NDARRAY_H
#define ABCDK_UTIL_NDARRAY_H

#include "abcdk/util/general.h"
#include "abcdk/util/object.h"

__BEGIN_DECLS

/**简单的多维数组。*/
typedef struct _abcdk_ndarray
{
    /**格式。*/
    int fmt;

    /**块数量。*/
    size_t block;

    /**宽(每块)。*/
    size_t width;
    
    /**高(每块)。*/
    size_t height;

    /**深(每块)。*/
    size_t depth;

    /**宽占用的字节数量(包括对齐字节)。*/
    size_t stride;

    /**单元格占用的字节数量。*/
    size_t cell;

    /**数据指针。*/
    void *data;

    /**数据长度。*/
    size_t size;

    /**缓存。*/
    abcdk_object_t *buf;

    /**硬件环境。*/
    abcdk_object_t *hw_ctx;
} abcdk_ndarray_t;

/**多维数组常量。*/
typedef enum _abcdk_ndarray_constant
{
    /** NCHW (AAA...BBB...CCC)*/
    ABCDK_NDARRAY_NCHW = 1,
#define ABCDK_NDARRAY_NCHW ABCDK_NDARRAY_NCHW

    /** NHWC (ABC...ABC...ABC)*/
    ABCDK_NDARRAY_NHWC = 2,
#define ABCDK_NDARRAY_NHWC ABCDK_NDARRAY_NHWC

    /** 水平翻转。*/
    ABCDK_NDARRAY_FLIP_H = 1,
#define ABCDK_NDARRAY_FLIP_H ABCDK_NDARRAY_FLIP_H

    /** 垂直翻转。*/
    ABCDK_NDARRAY_FLIP_V = 2,
#define ABCDK_NDARRAY_FLIP_V ABCDK_NDARRAY_FLIP_V
}abcdk_ndarray_constant_t;

/**
 * 计算占用空间(字节)。
*/
size_t abcdk_ndarray_size(abcdk_ndarray_t *ctx);

/**
 * 设置宽度步长(字节)。
 * 
 * @param align 对齐字节。
*/
void abcdk_ndarray_set_stride(abcdk_ndarray_t *ctx,size_t align);

/**
 * 计算坐标的偏移量(字节)。
 */
size_t abcdk_ndarray_offset(abcdk_ndarray_t *ctx, size_t n, size_t x, size_t y, size_t z, int flag);

/**
 * 计算坐标的偏移地址。
*/
void *abcdk_ndarray_seek(abcdk_ndarray_t *ctx, size_t n, size_t x, size_t y, size_t z, int flag);

/**释放。*/
void abcdk_ndarray_free(abcdk_ndarray_t **ctx);

/**创建。*/
abcdk_ndarray_t *abcdk_ndarray_alloc(int fmt, size_t block, size_t width, size_t height, size_t depth, size_t cell, size_t align, int no_space);

/**
 * 多维数组复制。
 *
 * @note 仅数组数据。
 *
 * @return 0 成功，< 0 失败。
 */
int abcdk_ndarray_copy(abcdk_ndarray_t *dst, const abcdk_ndarray_t *src);

/**
 * 多维数组克隆。
 *
 * @note 仅数组数据。
 */
abcdk_ndarray_t *abcdk_ndarray_clone(const abcdk_ndarray_t *src);

/**
 * 多维数组克隆。
 *
 * @note 仅数组数据。
 */
abcdk_ndarray_t *abcdk_ndarray_clone2(const uint8_t *src_data, const int src_stride,
                                      int fmt, size_t block, size_t width, size_t height, size_t depth, size_t cell);

__END_DECLS

#endif // ABCDK_UTIL_NDARRAY_H