/*
 * This file is part of ABCDK.
 *
 * Copyright (c) 2025 The ABCDK project authors. All Rights Reserved.
 *
 */
#ifndef ABCDK_CUDA_IMGPROC_H
#define ABCDK_CUDA_IMGPROC_H

#include "abcdk/util/geometry.h"
#include "abcdk/cuda/cuda.h"
#include "abcdk/cuda/memory.h"

#ifdef __cuda_cuda_h__

__BEGIN_DECLS

/**
 * 图像填充。
 *
 * @return 0 成功，< 0  失败。
 */
int abcdk_cuda_imgproc_stuff_8u(int channels, int packed, uint8_t *dst, size_t width, size_t pitch, size_t height, uint8_t scalar[1]);

/**
 * 全景图像融合(从左到右)。
 *
 * @param [in] panorama_w 全景图像宽度。
 * @param [in] panorama_ws 全景图像宽度步长。
 * @param [in] panorama_h 全景图像高度。
 *
 * @param [in] compose_w 融合图像宽度。
 * @param [in] compose_ws 融合图像宽度步长。
 * @param [in] compose_h 融合图像高度。
 *
 * @param [in] overlap_x  融合图像在全景图像的左上角X坐标。
 * @param [in] overlap_y  融合图像在全景图像的左上角Y坐标。
 * @param [in] overlap_w  融合图像在全景图像中重叠宽度。
 * @param [in] optimize_seam 接缝美化。0 禁用，!0 启用。
 *
 * @return 0 成功，< 0  失败。
 */
int abcdk_cuda_imgproc_compose_8u(int channels, int packed,
                                  uint8_t *panorama, size_t panorama_w, size_t panorama_ws, size_t panorama_h,
                                  uint8_t *compose, size_t compose_w, size_t compose_ws, size_t compose_h,
                                  uint8_t scalar[], size_t overlap_x, size_t overlap_y, size_t overlap_w, int optimize_seam);

/**
 * 调整亮度。
 *
 * @note dst[z] = src[z] * alpha[z] + bate[z]
 *
 * @return 0 成功，< 0  失败。
 */
int abcdk_cuda_imgproc_brightness_8u(int channels, int packed,
                                     uint8_t *dst, size_t dst_ws, uint8_t *src, size_t src_ws,
                                     size_t w, size_t h, float *alpha, float *bate);

/**
 * 暗通道除雾。
 *
 * @note 建议：a=220,m=0.35,w=0.9
 *
 * @param [in] dack_a 暗通道像素值。
 * @param [in] dack_m 模数。
 * @param [in] dack_w 权重。
 *
 * @return 0 成功，< 0  失败。
 */
int abcdk_cuda_imgproc_defog_8u(int channels, int packed,
                                uint8_t *dst, size_t dst_ws, uint8_t *src, size_t src_ws,
                                size_t w, size_t h, uint8_t dack_a, float dack_m, float dack_w);

/**
 * 画矩形框。
 *
 * @param [in] corner 左上(x1,y1)，右下(x2,y2)。
 *
 * @return 0 成功，< 0  失败。
 */
int abcdk_cuda_imgproc_drawrect_8u(int channels, int packed,
                                   uint8_t *dst, size_t w, size_t ws, size_t h,
                                   uint8_t color[], int weight, int corner[4]);

/**
 * 缩放。
 *
 * @param [in] keep_aspect_ratio 保持纵横比例。
 * @param [in] inter_mode 插值方案。
 *
 * @return 0 成功，< 0 失败。
 */
int abcdk_cuda_imgproc_resize_8u(int channels, int packed,
                                 uint8_t *dst, size_t dst_w, size_t dst_ws, size_t dst_h, const NppiRect *dst_roi,
                                 const uint8_t *src, size_t src_w, size_t src_ws, size_t src_h, const NppiRect *src_roi,
                                 int keep_aspect_ratio, NppiInterpolationMode inter_mode);

/**
 * 变换。
 *
 * @param [in] dst_quad 目标角点。 [0][] 左上，[1][] 右上，[2][] 右下，[3][]左下。
 * @param [in] src_quad 源图角点。 [0][] 左上，[1][] 右上，[2][] 右下，[3][]左下。
 * @param [in] warp_mode 变换模式。1 透视，2 仿射。
 *
 * @return 0 成功，< 0 失败。
 */
int abcdk_cuda_imgproc_warp_8u(int channels, int packed,
                               uint8_t *dst, size_t dst_w, size_t dst_ws, size_t dst_h, const NppiRect *dst_roi, const NppiPoint dst_quad[4],
                               const uint8_t *src, size_t src_w, size_t src_ws, size_t src_h, const NppiRect *src_roi, const NppiPoint src_quad[4],
                               int warp_mode, NppiInterpolationMode inter_mode);

/**
 * 重映射。
 *
 * @return 0 成功，< 0 失败。
 */
int abcdk_cuda_imgproc_remap_8u(int channels, int packed,
                                uint8_t *dst, size_t dst_w, size_t dst_ws, size_t dst_h, const NppiRect *dst_roi,
                                const uint8_t *src, size_t src_w, size_t src_ws, size_t src_h, const NppiRect *src_roi,
                                const float *xmap, size_t xmap_ws, const float *ymap, size_t ymap_ws,
                                NppiInterpolationMode inter_mode);

__END_DECLS

#endif //__cuda_cuda_h__

#endif // ABCDK_CUDA_IMGPROC_H