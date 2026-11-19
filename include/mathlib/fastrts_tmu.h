/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief TI AM13 TMU (Trigonometric Math Unit) fast math functions.
 *
 * The TMU is a Cortex-M33 coprocessor (CP0) accessed with Arm Custom Datapath
 * Extension (CDE) instructions. These inline functions are drop-in
 * replacements for the matching libm single-precision functions. They are
 * not remapped over the libm names; call the @c *_tmu variants explicitly.
 *
 * Requires CONFIG_TI_MATHLIB_FASTRTS, which builds with the cdecp0 CPU
 * feature and enables access to coprocessor 0 at boot.
 */

#ifndef TI_ZEPHYR_INCLUDE_MATHLIB_FASTRTS_TMU_H_
#define TI_ZEPHYR_INCLUDE_MATHLIB_FASTRTS_TMU_H_

#include <stdbool.h>
#include <stdint.h>
#include <zephyr/toolchain.h>

#include <arm_cde.h>

#ifdef __cplusplus
extern "C" {
#endif

/** TMU underflow flag */
#define FASTRTS_TMU_FLAGS_LUF 0x00000001U
/** TMU overflow flag */
#define FASTRTS_TMU_FLAGS_LVF 0x00000002U

#define FASTRTS_TMU_TWO_PI  (6.28318530717959F)
#define FASTRTS_TMU_PI      (3.14159265358979F)
#define FASTRTS_TMU_HALF_PI (1.57079632679490F)
#define FASTRTS_TMU_INV_2PI (0.15915494309190F)
#define FASTRTS_TMU_LOG2E   (1.44269504F)
#define FASTRTS_TMU_LN2     (0.6931471805F)

/* The CDE intrinsics take uint32_t operands; the union converts bit-exactly. */
union fastrts_tmu_word {
	float f32;
	uint32_t u32;
};

/** @brief sinf, input in radians */
static ALWAYS_INLINE float sinf_tmu(float x)
{
	union fastrts_tmu_word in, out;

	in.f32 = x * FASTRTS_TMU_INV_2PI;
	out.u32 = __arm_vcx2_u32(0, in.u32, 0);
	return out.f32;
}

/** @brief cosf, input in radians */
static ALWAYS_INLINE float cosf_tmu(float x)
{
	union fastrts_tmu_word in, out;

	in.f32 = x * FASTRTS_TMU_INV_2PI;
	out.u32 = __arm_vcx2_u32(0, in.u32, 1);
	return out.f32;
}

/** @brief atanf, result in radians. Valid for any input. */
static ALWAYS_INLINE float atanf_tmu(float x)
{
	union fastrts_tmu_word in, out;
	bool invert = (x > 1.0f) || (x < -1.0f);

	/* The TMU atan is only valid for |x| <= 1, so fold larger inputs in with
	 * atan(x) = +/-pi/2 - atan(1/x).
	 */
	in.f32 = invert ? (1.0f / x) : x;
	out.u32 = __arm_vcx2_u32(0, in.u32, 2);
	out.f32 *= FASTRTS_TMU_TWO_PI;

	if (invert) {
		return ((x > 0.0f) ? FASTRTS_TMU_HALF_PI : -FASTRTS_TMU_HALF_PI) - out.f32;
	}
	return out.f32;
}

/** @brief atan2f, result in radians. Uses the TMU atan with quadrant correction. */
static ALWAYS_INLINE float atan2f_tmu(float y, float x)
{
	union fastrts_tmu_word ratio, atan_pu, ay, ax;
	float atan_rad;
	bool swap;

	ay.f32 = y;
	ax.f32 = x;
	ay.u32 &= 0x7FFFFFFFU;
	ax.u32 &= 0x7FFFFFFFU;

	/* Both inputs zero (or denormal) */
	if ((ax.u32 | ay.u32) < 0x00800000U) {
		return 0.0f;
	}

	/* Keep the ratio in [-1, 1] */
	swap = ay.f32 > ax.f32;
	ratio.f32 = swap ? (x / y) : (y / x);

	atan_pu.u32 = __arm_vcx2_u32(0, ratio.u32, 2);
	atan_rad = atan_pu.f32 * FASTRTS_TMU_TWO_PI;

	if (swap) {
		return (y >= 0.0f) ? (FASTRTS_TMU_HALF_PI - atan_rad)
				   : (-FASTRTS_TMU_HALF_PI - atan_rad);
	}
	if (x >= 0.0f) {
		return atan_rad;
	}
	return (y >= 0.0f) ? (atan_rad + FASTRTS_TMU_PI) : (atan_rad - FASTRTS_TMU_PI);
}

/** @brief sqrtf */
static ALWAYS_INLINE float sqrtf_tmu(float x)
{
	union fastrts_tmu_word in, out;

	in.f32 = x;
	out.u32 = __arm_vcx2_u32(0, in.u32, 3);
	return out.f32;
}

/** @brief acosf, result in radians */
static ALWAYS_INLINE float acosf_tmu(float x)
{
	/* acos(x) = atan2(sqrt(1 - x^2), x) */
	return atan2f_tmu(sqrtf_tmu(1.0f - (x * x)), x);
}

/** @brief asinf, result in radians */
static ALWAYS_INLINE float asinf_tmu(float x)
{
	/* asin(x) = atan2(x, sqrt(1 - x^2)) */
	return atan2f_tmu(x, sqrtf_tmu(1.0f - (x * x)));
}

/** @brief expf */
static ALWAYS_INLINE float expf_tmu(float x)
{
	union fastrts_tmu_word in, out;

	in.f32 = x * -FASTRTS_TMU_LOG2E;
	out.u32 = __arm_vcx2_u32(0, in.u32, 4);
	if (x > 0.0f) {
		out.f32 = 1.0f / out.f32;
	}
	return out.f32;
}

/** @brief logf (natural logarithm) */
static ALWAYS_INLINE float logf_tmu(float x)
{
	union fastrts_tmu_word in, out;

	in.f32 = x;
	/* The TMU produces log2(x) */
	out.u32 = __arm_vcx2_u32(0, in.u32, 5);
	return out.f32 * FASTRTS_TMU_LN2;
}

/** @brief Floating-point divide y / x */
static ALWAYS_INLINE float divf_tmu(float y, float x)
{
	union fastrts_tmu_word y_in, x_in, out;

	y_in.f32 = y;
	x_in.f32 = x;
	out.u32 = __arm_vcx3_u32(0, y_in.u32, x_in.u32, 0);
	return out.f32;
}

/*
 * The flag registers are hidden TMU state. The compiler treats the CDE
 * intrinsics as pure functions and may reorder, merge or drop them, which
 * breaks read-after-write ordering, so the flag accessors use volatile asm.
 */
static ALWAYS_INLINE uint32_t fastrts_tmu_flags_op(uint32_t mask_or_zero, uint32_t value)
{
	uint32_t prev;

	__asm__ volatile("cx3 p0, %0, %1, %2, #0"
			 : "=r"(prev)
			 : "r"(mask_or_zero), "r"(value)
			 : "memory");
	return prev;
}

/**
 * @brief Clear TMU status flags.
 *
 * @param flags Mask of @ref FASTRTS_TMU_FLAGS_LUF and @ref FASTRTS_TMU_FLAGS_LVF
 * @return Flag value before the clear
 */
static ALWAYS_INLINE uint32_t fastrts_tmu_clear_flags(uint32_t flags)
{
	return fastrts_tmu_flags_op(~flags, flags);
}

/**
 * @brief Set TMU status flags.
 *
 * @param flags Mask of @ref FASTRTS_TMU_FLAGS_LUF and @ref FASTRTS_TMU_FLAGS_LVF
 * @return Flag value before the set
 */
static ALWAYS_INLINE uint32_t fastrts_tmu_set_flags(uint32_t flags)
{
	return fastrts_tmu_flags_op(flags, flags);
}

/** @brief Read the TMU status flags (@ref FASTRTS_TMU_FLAGS_LUF, @ref FASTRTS_TMU_FLAGS_LVF) */
static ALWAYS_INLINE uint32_t fastrts_tmu_get_flags(void)
{
	return fastrts_tmu_flags_op(FASTRTS_TMU_FLAGS_LUF | FASTRTS_TMU_FLAGS_LVF, 0U);
}

#ifdef __cplusplus
}
#endif

#endif /* TI_ZEPHYR_INCLUDE_MATHLIB_FASTRTS_TMU_H_ */
