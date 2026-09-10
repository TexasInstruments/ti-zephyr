/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/regulator.h>
#include <zephyr/ztest.h>

static const struct device *const vref = DEVICE_DT_GET(DT_NODELABEL(vref0));

static const regulator_mode_t allowed_modes[] = {
	DT_FOREACH_PROP_ELEM_SEP(DT_NODELABEL(vref0), regulator_allowed_modes, DT_PROP_BY_IDX, (,)) };

ZTEST(regulator_vref_api, test_voltage_set_get)
{
	int32_t volt_uv = 0;

	zassert_ok(regulator_set_voltage(vref, 1400000, 1400000));
	zassert_ok(regulator_get_voltage(vref, &volt_uv));
	zassert_equal(volt_uv, 1400000);

	zassert_ok(regulator_set_voltage(vref, 2500000, 2500000));
	zassert_ok(regulator_get_voltage(vref, &volt_uv));
	zassert_equal(volt_uv, 2500000);
}

ZTEST(regulator_vref_api, test_enable_blocks_voltage_change)
{
	int32_t volt_uv = 0;

	zassert_ok(regulator_set_voltage(vref, 1400000, 1400000));
	zassert_ok(regulator_enable(vref));

	zassert_equal(regulator_set_voltage(vref, 2500000, 2500000), -EBUSY);
	zassert_ok(regulator_get_voltage(vref, &volt_uv));
	zassert_equal(volt_uv, 1400000);

	zassert_ok(regulator_disable(vref));
}

ZTEST(regulator_vref_api, test_mode_set_get)
{
	regulator_mode_t mode;

	for (size_t i = 0U; i < ARRAY_SIZE(allowed_modes); i++) {
		zassert_ok(regulator_set_mode(vref, allowed_modes[i]));
		zassert_ok(regulator_get_mode(vref, &mode));
		zassert_equal(mode, allowed_modes[i]);
	}
}

ZTEST(regulator_vref_api, test_enable_blocks_mode_change)
{
	regulator_mode_t mode;

	zassert_ok(regulator_set_mode(vref, allowed_modes[0]));
	zassert_ok(regulator_enable(vref));

	if (ARRAY_SIZE(allowed_modes) > 1U) {
		zassert_equal(regulator_set_mode(vref, allowed_modes[1]), -EBUSY);
	}

	zassert_ok(regulator_get_mode(vref, &mode));
	zassert_equal(mode, allowed_modes[0]);

	zassert_ok(regulator_disable(vref));
}

static void *setup(void)
{
	zassert_true(device_is_ready(vref));

	return NULL;
}

ZTEST_SUITE(regulator_vref_api, NULL, setup, NULL, NULL, NULL);
