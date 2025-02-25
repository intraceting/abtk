/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2021 The ABCDK project authors. All Rights Reserved.
 *
 */
#ifndef ABCDK_MEDIA_PIXFMT_H
#define ABCDK_MEDIA_PIXFMT_H

#include "abcdk/util/general.h"
#include "abcdk/util/object.h"

__BEGIN_DECLS

/**常量。*/
typedef enum _abcdk_pixfmt_constant
{
    ABCDK_PIXFMT_NONE = -1,
#define ABCDK_PIXFMT_NONE ABCDK_PIXFMT_NONE

    ABCDK_PIXFMT_YUV420P,
#define ABCDK_PIXFMT_YUV420P ABCDK_PIXFMT_YUV420P

    ABCDK_PIXFMT_I420,
#define ABCDK_PIXFMT_I420 ABCDK_PIXFMT_I420

    ABCDK_PIXFMT_YV12,
#define ABCDK_PIXFMT_YV12 ABCDK_PIXFMT_YV12

    ABCDK_PIXFMT_NV12,
#define ABCDK_PIXFMT_NV12 ABCDK_PIXFMT_NV12

    ABCDK_PIXFMT_NV21,
#define ABCDK_PIXFMT_NV21 ABCDK_PIXFMT_NV21

    ABCDK_PIXFMT_YUV422P,
#define ABCDK_PIXFMT_YUV422P ABCDK_PIXFMT_YUV422P

    ABCDK_PIXFMT_YUYV,
#define ABCDK_PIXFMT_YUYV ABCDK_PIXFMT_YUYV

    ABCDK_PIXFMT_UYVY,
#define ABCDK_PIXFMT_UYVY ABCDK_PIXFMT_UYVY

    ABCDK_PIXFMT_NV16,
#define ABCDK_PIXFMT_NV16 ABCDK_PIXFMT_NV16

    ABCDK_PIXFMT_YUV444P,
#define ABCDK_PIXFMT_YUV444P ABCDK_PIXFMT_YUV444P

    ABCDK_PIXFMT_NV24,
#define ABCDK_PIXFMT_NV24 ABCDK_PIXFMT_NV24

    ABCDK_PIXFMT_YUV411P,
#define ABCDK_PIXFMT_YUV411P ABCDK_PIXFMT_YUV411P

    ABCDK_PIXFMT_RGB24,
#define ABCDK_PIXFMT_RGB24 ABCDK_PIXFMT_RGB24

    ABCDK_PIXFMT_RGB32,
#define ABCDK_PIXFMT_RGB32 ABCDK_PIXFMT_RGB32

    ABCDK_PIXFMT_BGR24,
#define ABCDK_PIXFMT_BGR24 ABCDK_PIXFMT_BGR24

    ABCDK_PIXFMT_BGR32,
#define ABCDK_PIXFMT_BGR32 ABCDK_PIXFMT_BGR32

    ABCDK_PIXFMT_GRAY8,
#define ABCDK_PIXFMT_GRAY8 ABCDK_PIXFMT_GRAY8

    ABCDK_PIXFMT_GRAYF32,
#define ABCDK_PIXFMT_GRAYF32 ABCDK_PIXFMT_GRAYF32

} abcdk_pixfmt_constant_t;

__END_DECLS

#endif // ABCDK_MEDIA_PIXFMT_H