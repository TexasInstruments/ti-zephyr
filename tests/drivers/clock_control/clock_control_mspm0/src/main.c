/*
 * Copyright (c) 2026 Texas Instruments Incorporated
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/mspm0_clock_control.h>
#include <zephyr/ztest.h>

#define HFCLK_NODE    DT_NODELABEL(hfclk)
#define HFCLK_IN_NODE DT_NODELABEL(hfclk_in)
#define HFXT_NODE     DT_NODELABEL(hfxt)
#define HSCLK_NODE    DT_NODELABEL(hsclk)
#define LFCLK_NODE    DT_NODELABEL(lfclk)
#define LFCLK_IN_NODE DT_NODELABEL(lfclk_in)
#define LFXT_NODE     DT_NODELABEL(lfxt)
#define MCLK_NODE     DT_NODELABEL(mclk)
#define MFPCLK_NODE   DT_NODELABEL(mfpclk)
#define CANCLK_NODE   DT_NODELABEL(canclk)
#define SYSPLL_NODE   DT_NODELABEL(syspll)

#if DT_NODE_HAS_PROP(SYSPLL_NODE, clk2x_div)
#define HSCLK_SYSPLL_TAP MSPM0_CLOCK_SYSPLL_CLK2X
#elif DT_NODE_HAS_PROP(SYSPLL_NODE, clk0_div)
#define HSCLK_SYSPLL_TAP MSPM0_CLOCK_SYSPLL_CLK0
#else
#define HSCLK_SYSPLL_TAP 0xFFFFFFFF
#endif

#define HAS_HFCLK     DT_NODE_HAS_STATUS_OKAY(HFCLK_NODE)
#define HAS_HFXT      DT_NODE_HAS_STATUS_OKAY(HFXT_NODE)
#define HAS_HFCLK_IN  DT_NODE_HAS_STATUS_OKAY(HFCLK_IN_NODE)
#define HAS_HSCLK     DT_NODE_HAS_STATUS_OKAY(HSCLK_NODE)
#define HAS_SYSPLL    DT_NODE_HAS_STATUS_OKAY(SYSPLL_NODE)
#define HAS_LFXT      DT_NODE_HAS_STATUS_OKAY(LFXT_NODE)
#define HAS_LFCLK_IN  DT_NODE_HAS_STATUS_OKAY(LFCLK_IN_NODE)
#define HAS_MFPCLK    DT_NODE_HAS_STATUS_OKAY(MFPCLK_NODE)
#define HAS_CANCLK    DT_NODE_HAS_STATUS_OKAY(CANCLK_NODE)
#define MCLK_NO_LFCLK IS_ENABLED(CONFIG_SOC_SERIES_MSPM33C)

/*
 * The DT sources are what the driver initially configures each clock with, we require these default
 * clocks so that we can restore them later after the test. See clock_track() and
 * mspm0_clock_control_after()
 */
#if DT_SAME_NODE(DT_CLOCKS_CTLR(HFCLK_NODE), HFXT_NODE)
#define DEFAULT_HFCLK_SRC MSPM0_CLOCK_SRC_HFXT
#else
#define DEFAULT_HFCLK_SRC MSPM0_CLOCK_SRC_HFCLK_IN
#endif

#if DT_SAME_NODE(DT_CLOCKS_CTLR(SYSPLL_NODE), HFCLK_NODE)
#define DEFAULT_SYSPLL_SRC MSPM0_CLOCK_SRC_HFCLK
#else
#define DEFAULT_SYSPLL_SRC MSPM0_CLOCK_SRC_SYSOSC
#endif

#if DT_SAME_NODE(DT_CLOCKS_CTLR(HSCLK_NODE), HFCLK_NODE)
#define DEFAULT_HSCLK_SRC MSPM0_CLOCK_SRC_HFCLK
#elif DT_SAME_NODE(DT_CLOCKS_CTLR(HSCLK_NODE), SYSPLL_NODE)
#define DEFAULT_HSCLK_SRC MSPM0_CLOCK_SRC_SYSPLL
#else
#define DEFAULT_HSCLK_SRC MSPM0_CLOCK_SRC_SYSOSC
#endif

#if DT_SAME_NODE(DT_CLOCKS_CTLR(LFCLK_NODE), LFXT_NODE)
#define DEFAULT_LFCLK_SRC MSPM0_CLOCK_SRC_LFXT
#elif DT_SAME_NODE(DT_CLOCKS_CTLR(LFCLK_NODE), LFCLK_IN_NODE)
#define DEFAULT_LFCLK_SRC MSPM0_CLOCK_SRC_LFCLK_IN
#else
#define DEFAULT_LFCLK_SRC MSPM0_CLOCK_SRC_LFOSC
#endif

#if DT_SAME_NODE(DT_CLOCKS_CTLR(MCLK_NODE), HSCLK_NODE)
#define DEFAULT_MCLK_SRC MSPM0_CLOCK_SRC_HSCLK
#elif DT_SAME_NODE(DT_CLOCKS_CTLR(MCLK_NODE), LFCLK_NODE)
#define DEFAULT_MCLK_SRC MSPM0_CLOCK_SRC_LFCLK
#else
#define DEFAULT_MCLK_SRC MSPM0_CLOCK_SRC_SYSOSC
#endif

#if DT_SAME_NODE(DT_CLOCKS_CTLR(MFPCLK_NODE), HFCLK_NODE)
#define DEFAULT_MFPCLK_SRC MSPM0_CLOCK_SRC_HFCLK
#else
#define DEFAULT_MFPCLK_SRC MSPM0_CLOCK_SRC_SYSOSC
#endif

#if DT_SAME_NODE(DT_CLOCKS_CTLR(CANCLK_NODE), SYSPLL_NODE)
#define DEFAULT_CANCLK_SRC MSPM0_CLOCK_SRC_SYSPLL
#else
#define DEFAULT_CANCLK_SRC MSPM0_CLOCK_SRC_HFCLK
#endif

/* True only when MCLK's DT default genuinely routes through HSCLK<-SYSPLL. */
#define MCLK_DEFAULT_SOURCES_FROM_SYSPLL                                                           \
	(DT_SAME_NODE(DT_CLOCKS_CTLR(MCLK_NODE), HSCLK_NODE) &&                                    \
	 DT_SAME_NODE(DT_CLOCKS_CTLR(HSCLK_NODE), SYSPLL_NODE))

static const struct device *clk_dev = DEVICE_DT_GET(DT_NODELABEL(ckm));

static uint32_t clock_get_rate(uint32_t clk)
{
	struct mspm0_sys_clock subsys = {clk};
	uint32_t rate;
	int ret;

	ret = clock_control_get_rate(clk_dev, (clock_control_subsys_t)&subsys, &rate);
	zassert_ok(ret, "get_rate(clk=%u) failed: %d", clk, ret);

	return rate;
}

static int clock_configure(uint32_t clk, enum mspm0_clock_source src)
{
	struct mspm0_sys_clock subsys = {clk};
	return clock_control_configure(clk_dev, (clock_control_subsys_t)&subsys, &src);
}

static enum clock_control_status clock_status(uint32_t clk)
{
	struct mspm0_sys_clock subsys = {clk};

	return clock_control_get_status(clk_dev, (clock_control_subsys_t)&subsys);
}

static int clock_set_rate(uint32_t clk, uint32_t rate)
{
	struct mspm0_sys_clock subsys = {clk};

	return clock_control_set_rate(clk_dev, (clock_control_subsys_t)&subsys, &rate);
}

static int clock_get_rate_raw(uint32_t clk, uint32_t *rate)
{
	struct mspm0_sys_clock subsys = {clk};

	return clock_control_get_rate(clk_dev, (clock_control_subsys_t)&subsys, rate);
}

static enum mspm0_clock_source clock_default_source(uint32_t clk)
{
	switch (clk) {
	case MSPM0_CLOCK_HFCLK:
		return DEFAULT_HFCLK_SRC;
	case MSPM0_CLOCK_SYSPLL:
		return DEFAULT_SYSPLL_SRC;
	case MSPM0_CLOCK_HSCLK:
		return DEFAULT_HSCLK_SRC;
	case MSPM0_CLOCK_MCLK:
		return DEFAULT_MCLK_SRC;
	case MSPM0_CLOCK_MFPCLK:
		return DEFAULT_MFPCLK_SRC;
	case MSPM0_CLOCK_CANCLK:
		return DEFAULT_CANCLK_SRC;
	default:
		zassert_unreachable("no default source for clk 0x%x", clk);
		return MSPM0_CLOCK_SRC_SYSOSC;
	}
}

/*
 * Store tracked clocks in an array before restoring them using mspm0_clock_control_after().
 */
#define MAX_TRACKED_CLOCKS 4

static uint32_t tracked_clocks[MAX_TRACKED_CLOCKS];
static size_t tracked_clocks_len;

static void clock_track(uint32_t clk)
{
	zassert_true(tracked_clocks_len < ARRAY_SIZE(tracked_clocks), "tracked_clocks overflow");
	tracked_clocks[tracked_clocks_len++] = clk;
}

static const char *clock_string(uint32_t clk)
{
	switch (clk) {
	case MSPM0_CLOCK_HFCLK:
		return "HFCLK";
	case MSPM0_CLOCK_HSCLK:
		return "HSCLK";
	case MSPM0_CLOCK_MCLK:
		return "MCLK";
	case MSPM0_CLOCK_SYSPLL:
		return "SYSPLL";
	case MSPM0_CLOCK_MFPCLK:
		return "MFPCLK";
	case MSPM0_CLOCK_CANCLK:
		return "CANCLK";
	case MSPM0_CLOCK_LFCLK:
		return "LFCLK";
	default:
		return "???";
	}
}

static const char *source_string(enum mspm0_clock_source src)
{
	switch (src) {
	case MSPM0_CLOCK_SRC_SYSOSC:
		return "SYSOSC";
	case MSPM0_CLOCK_SRC_LFCLK:
		return "LFCLK";
	case MSPM0_CLOCK_SRC_LFXT:
		return "LFXT";
	case MSPM0_CLOCK_SRC_LFOSC:
		return "LFOSC";
	case MSPM0_CLOCK_SRC_LFCLK_IN:
		return "LFCLK_IN";
	case MSPM0_CLOCK_SRC_HFXT:
		return "HFXT";
	case MSPM0_CLOCK_SRC_HFCLK_IN:
		return "HFCLK_IN";
	case MSPM0_CLOCK_SRC_HFCLK:
		return "HFCLK";
	case MSPM0_CLOCK_SRC_HSCLK:
		return "HSCLK";
	case MSPM0_CLOCK_SRC_SYSPLL:
		return "SYSPLL";
	default:
		return "???";
	}
}

/*
 * Shared step for chain tests: configure one clock in the chain
 * ret = -ENOTSUP means that current clock/src combination is not supported by the driver
 * ret = 0 means the driver is configured successfully, so assert that.
 */
static bool clock_configure_chain_step(uint32_t clk, enum mspm0_clock_source src)
{
	int ret = clock_configure(clk, src);

	if (ret == -ENOTSUP) {
		return false;
	}
	clock_track(clk);
	zassert_ok(ret, "configure(%s, %s) failed: %d", clock_string(clk), source_string(src), ret);
	return true;
}

/* Shared step for all possible "configure_*" tests. Checks 0 and -ENOTSUP against whether the
 * configuration is supposed to be supported or not */
static bool clock_configure_expect(uint32_t clk, enum mspm0_clock_source src, bool supported)
{
	int ret = clock_configure(clk, src);

	if (supported) {
		zassert_ok(ret, "configure(%s, %s) failed: %d", clock_string(clk),
			   source_string(src), ret);
		return true;
	}
	zassert_equal(ret, -ENOTSUP, "configure(%s, %s) should be -ENOTSUP, got %d",
		      clock_string(clk), source_string(src), ret);
	return false;
}

/* set_rate(SYSOSC) rejected while MCLK depends on it via HSCLK; takes effect once MCLK moves to
 * SYSOSC. */
ZTEST(clock_control_mspm0, test_sysosc_set_rate)
{
	uint32_t rate;
	uint32_t new_rate;
	uint32_t readback = 0;
	uint32_t mclk_rate = 0;
	int busy_ret;
	int set_ret;
	int rb_ret = 0;
	int mclk_ret = 0;
	int restore_rate_ret;

	clock_track(MSPM0_CLOCK_MCLK);

	rate = clock_get_rate(MSPM0_CLOCK_SYSOSC);
	new_rate = (rate == MHZ(32)) ? MHZ(4) : MHZ(32);

	if (clock_configure_expect(MSPM0_CLOCK_MCLK, MSPM0_CLOCK_SRC_HSCLK, HAS_HSCLK)) {
		busy_ret = clock_set_rate(MSPM0_CLOCK_SYSOSC, new_rate);
		zassert_equal(busy_ret, -EBUSY,
			      "set_rate(SYSOSC) should be busy while MCLK uses HSCLK, got %d",
			      busy_ret);

		zassert_ok(clock_configure(MSPM0_CLOCK_MCLK, MSPM0_CLOCK_SRC_SYSOSC),
			   "failed to move MCLK onto SYSOSC");
	}

	/* Console UART clocks off this SYSOSC->MCLK->ULPCLK chain; restore SYSOSC before any
	 * zassert can abort and desync the baud rate. */
	set_ret = clock_set_rate(MSPM0_CLOCK_SYSOSC, new_rate);
	if (set_ret == 0) {
		rb_ret = clock_get_rate_raw(MSPM0_CLOCK_SYSOSC, &readback);
		mclk_ret = clock_get_rate_raw(MSPM0_CLOCK_MCLK, &mclk_rate);
	}
	restore_rate_ret = clock_set_rate(MSPM0_CLOCK_SYSOSC, rate);
	zassert_ok(clock_configure(MSPM0_CLOCK_MCLK, DEFAULT_MCLK_SRC),
		   "failed to restore MCLK to its default source");

	zassert_ok(set_ret, "set_rate(SYSOSC) failed while MCLK uses HSCLK: %d", set_ret);
	zassert_ok(restore_rate_ret, "failed to restore SYSOSC rate: %d", restore_rate_ret);
	zassert_ok(rb_ret, "get_rate(SYSOSC) after set failed: %d", rb_ret);
	zassert_equal(readback, new_rate, "SYSOSC rate did not change");
	zassert_ok(mclk_ret, "get_rate(MCLK) after set failed: %d", mclk_ret);
	zassert_equal(mclk_rate, new_rate, "MCLK did not track live SYSOSC rate");
}

/* mclk<-sysosc; unconditional -- always a legal MCLK source. */
ZTEST(clock_control_mspm0, test_mclk_sysosc_chain)
{
	int ret;

	clock_track(MSPM0_CLOCK_MCLK);

	ret = clock_configure(MSPM0_CLOCK_MCLK, MSPM0_CLOCK_SRC_SYSOSC);
	zassert_ok(ret, "configure(MCLK, SYSOSC) failed: %d", ret);
	zassert_equal(clock_get_rate(MSPM0_CLOCK_MCLK), clock_get_rate(MSPM0_CLOCK_SYSOSC),
		      "MCLK<-SYSOSC rate mismatch");
}

/* mclk<-lfclk; not offered on SoC series where MCLK's mux has no LFCLK input. */
ZTEST(clock_control_mspm0, test_mclk_lfclk_chain)
{
	int ret;

	ret = clock_configure(MSPM0_CLOCK_MCLK, MSPM0_CLOCK_SRC_LFCLK);
	if (ret == -ENOTSUP) {
		return;
	}
	clock_track(MSPM0_CLOCK_MCLK);

	zassert_ok(ret, "configure(MCLK, LFCLK) failed: %d", ret);

	zassert_equal(clock_get_rate(MSPM0_CLOCK_MCLK), clock_get_rate(MSPM0_CLOCK_LFCLK),
		      "MCLK<-LFCLK rate mismatch");
}

/*
 * we do not restore LFCLK since it is impossible to move back to LFOSC once you have switched
 */
ZTEST(clock_control_mspm0, test_lfclk_configure_sources)
{
	/* if default clock source is lfclk_in, it is not possible to switch back to lfxt */
	bool blocked_by_lfclkin = (DEFAULT_LFCLK_SRC == MSPM0_CLOCK_SRC_LFCLK_IN);

	if (clock_configure_expect(MSPM0_CLOCK_LFCLK, MSPM0_CLOCK_SRC_LFXT,
				   HAS_LFXT && !blocked_by_lfclkin)) {
		zassert_equal(clock_status(MSPM0_CLOCK_LFCLK), CLOCK_CONTROL_STATUS_ON);
	}

	clock_configure_expect(MSPM0_CLOCK_LFCLK, MSPM0_CLOCK_SRC_LFCLK_IN, HAS_LFCLK_IN);
}

/*
 * Configure SYSPLL onto every legal source. Park it on SYSOSC first and track the clock to restore
 * it later.
 */
ZTEST(clock_control_mspm0, test_syspll_configure_sources)
{
	int ret;

	if (HAS_SYSPLL) {
		clock_track(MSPM0_CLOCK_SYSPLL);
	}

	if (MCLK_DEFAULT_SOURCES_FROM_SYSPLL) {
		clock_track(MSPM0_CLOCK_MCLK);
		ret = clock_configure(MSPM0_CLOCK_MCLK, MSPM0_CLOCK_SRC_SYSOSC);
		zassert_ok(ret, "configure(MCLK, SYSOSC) failed: %d", ret);
	}

	if (clock_configure_expect(MSPM0_CLOCK_SYSPLL, MSPM0_CLOCK_SRC_HFCLK,
				   HAS_SYSPLL && HAS_HFCLK)) {
		zassert_equal(clock_status(MSPM0_CLOCK_SYSPLL), CLOCK_CONTROL_STATUS_ON,
			      "SYSPLL did not report ON after startup");
	}

	/* SYSOSC's case is unconditional whenever SYSPLL exists -- always valid. */
	if (clock_configure_expect(MSPM0_CLOCK_SYSPLL, MSPM0_CLOCK_SRC_SYSOSC, HAS_SYSPLL)) {
		zassert_equal(clock_status(MSPM0_CLOCK_SYSPLL), CLOCK_CONTROL_STATUS_ON,
			      "SYSPLL did not report ON after startup");
	}
}

/* Configure HSCLK onto every legal source; MCLK stays put so this briefly glitchless-muxes the live
 * CPU clock. */
ZTEST(clock_control_mspm0, test_hsclk_configure_sources)
{
	if (HAS_HSCLK) {
		clock_track(MSPM0_CLOCK_HSCLK);
	}

	if (clock_configure_expect(MSPM0_CLOCK_HSCLK, MSPM0_CLOCK_SRC_HFCLK,
				   HAS_HSCLK && HAS_HFCLK)) {
		zassert_equal(clock_status(MSPM0_CLOCK_HSCLK), CLOCK_CONTROL_STATUS_ON);
		zassert_equal(clock_get_rate(MSPM0_CLOCK_HSCLK), clock_get_rate(MSPM0_CLOCK_HFCLK),
			      "HSCLK and HFCLK rates are different");
	}

	/* SYSOSC and SYSPLL are mutually exclusive HSCLK sources: SYSOSC is only
	 * legal when the SoC has no SYSPLL to source HSCLK from instead.
	 */
	if (clock_configure_expect(MSPM0_CLOCK_HSCLK, MSPM0_CLOCK_SRC_SYSOSC,
				   HAS_HSCLK && !HAS_SYSPLL)) {
		zassert_equal(clock_status(MSPM0_CLOCK_HSCLK), CLOCK_CONTROL_STATUS_ON);
		zassert_equal(clock_get_rate(MSPM0_CLOCK_HSCLK), clock_get_rate(MSPM0_CLOCK_SYSOSC),
			      "HSCLK and SYSOSC rates are different");
	}

	if (clock_configure_expect(MSPM0_CLOCK_HSCLK, MSPM0_CLOCK_SRC_SYSPLL,
				   HAS_HSCLK && HAS_SYSPLL)) {
		zassert_equal(clock_status(MSPM0_CLOCK_HSCLK), CLOCK_CONTROL_STATUS_ON);
		zassert_equal(clock_get_rate(MSPM0_CLOCK_HSCLK), clock_get_rate(HSCLK_SYSPLL_TAP),
			      "HSCLK and SYSPLL-derived rates are different");
	}
}

ZTEST(clock_control_mspm0, test_canclk_configure_sources)
{
	if (HAS_CANCLK) {
		clock_track(MSPM0_CLOCK_CANCLK);
	}

	if (clock_configure_expect(MSPM0_CLOCK_CANCLK, MSPM0_CLOCK_SRC_HFCLK,
				   HAS_CANCLK && HAS_HFCLK)) {
		zassert_equal(clock_status(MSPM0_CLOCK_CANCLK), CLOCK_CONTROL_STATUS_ON);
		zassert_equal(clock_get_rate(MSPM0_CLOCK_CANCLK), clock_get_rate(MSPM0_CLOCK_HFCLK),
			      "CANCLK and HFCLK rates are different");
	}

	if (clock_configure_expect(MSPM0_CLOCK_CANCLK, MSPM0_CLOCK_SRC_SYSPLL,
				   HAS_CANCLK && HAS_SYSPLL)) {
		zassert_equal(clock_status(MSPM0_CLOCK_CANCLK), CLOCK_CONTROL_STATUS_ON);
		zassert_equal(clock_get_rate(MSPM0_CLOCK_CANCLK),
			      clock_get_rate(MSPM0_CLOCK_SYSPLL_CLK1),
			      "CANCLK and SYSPLL-derived rates are different");
	}
}

ZTEST(clock_control_mspm0, test_mfpclk_configure_sources)
{
	if (HAS_MFPCLK) {
		clock_track(MSPM0_CLOCK_MFPCLK);
	}

	if (clock_configure_expect(MSPM0_CLOCK_MFPCLK, MSPM0_CLOCK_SRC_HFCLK,
				   HAS_MFPCLK && HAS_HFCLK)) {
		zassert_equal(clock_status(MSPM0_CLOCK_MFPCLK), CLOCK_CONTROL_STATUS_ON);
		zassert_equal(clock_get_rate(MSPM0_CLOCK_MFPCLK), MHZ(4),
			      "MFPCLK rate != 4MHz for HFCLK source");
	}

	/* SYSOSC's case is unconditional whenever MFPCLK exists -- always valid. */
	if (clock_configure_expect(MSPM0_CLOCK_MFPCLK, MSPM0_CLOCK_SRC_SYSOSC, HAS_MFPCLK)) {
		zassert_equal(clock_status(MSPM0_CLOCK_MFPCLK), CLOCK_CONTROL_STATUS_ON);
		zassert_equal(clock_get_rate(MSPM0_CLOCK_MFPCLK), MHZ(4),
			      "MFPCLK rate != 4MHz for SYSOSC source");
	}
}

ZTEST(clock_control_mspm0, test_hfclk_configure_sources)
{
	if (HAS_HFCLK) {
		clock_track(MSPM0_CLOCK_HFCLK);
	}

	if (clock_configure_expect(MSPM0_CLOCK_HFCLK, MSPM0_CLOCK_SRC_HFXT,
				   HAS_HFCLK && HAS_HFXT)) {
		zassert_equal(clock_status(MSPM0_CLOCK_HFCLK), CLOCK_CONTROL_STATUS_ON);
	}

	if (clock_configure_expect(MSPM0_CLOCK_HFCLK, MSPM0_CLOCK_SRC_HFCLK_IN,
				   HAS_HFCLK && HAS_HFCLK_IN)) {
		zassert_equal(clock_status(MSPM0_CLOCK_HFCLK), CLOCK_CONTROL_STATUS_ON);
	}
}

/* mclk<-hsclk<-hfclk<-{hfxt,hfclk_in} */
ZTEST(clock_control_mspm0, test_mclk_hsclk_hfclk_chain)
{
	int ret;

	if (!clock_configure_chain_step(MSPM0_CLOCK_HSCLK, MSPM0_CLOCK_SRC_HFCLK)) {
		return;
	}

	if (!clock_configure_chain_step(MSPM0_CLOCK_MCLK, MSPM0_CLOCK_SRC_HSCLK)) {
		return;
	}

	clock_track(MSPM0_CLOCK_HFCLK);

	ret = clock_configure(MSPM0_CLOCK_HFCLK, MSPM0_CLOCK_SRC_HFXT);
	if (ret != -ENOTSUP) {
		zassert_ok(ret, "configure(HFCLK, HFXT) failed: %d", ret);
		zassert_equal(clock_get_rate(MSPM0_CLOCK_MCLK), clock_get_rate(MSPM0_CLOCK_HFCLK),
			      "MCLK<-HSCLK<-HFCLK<-HFXT rate mismatch");
	}

	ret = clock_configure(MSPM0_CLOCK_HFCLK, MSPM0_CLOCK_SRC_HFCLK_IN);
	if (ret != -ENOTSUP) {
		zassert_ok(ret, "configure(HFCLK, HFCLK_IN) failed: %d", ret);
		zassert_equal(clock_get_rate(MSPM0_CLOCK_MCLK), clock_get_rate(MSPM0_CLOCK_HFCLK),
			      "MCLK<-HSCLK<-HFCLK<-HFCLK_IN rate mismatch");
	}
}

/* mclk<-hsclk<-syspll<-hfclk<-{hfxt,hfclk_in} */
ZTEST(clock_control_mspm0, test_mclk_hsclk_syspll_hfclk_chain)
{
	int ret;

	if (!clock_configure_chain_step(MSPM0_CLOCK_SYSPLL, MSPM0_CLOCK_SRC_HFCLK)) {
		return;
	}
	zassert_equal(clock_status(MSPM0_CLOCK_SYSPLL), CLOCK_CONTROL_STATUS_ON,
		      "SYSPLL did not report ON after startup");

	if (!clock_configure_chain_step(MSPM0_CLOCK_HSCLK, MSPM0_CLOCK_SRC_SYSPLL)) {
		return;
	}

	if (!clock_configure_chain_step(MSPM0_CLOCK_MCLK, MSPM0_CLOCK_SRC_HSCLK)) {
		return;
	}

	clock_track(MSPM0_CLOCK_HFCLK);

	ret = clock_configure(MSPM0_CLOCK_HFCLK, MSPM0_CLOCK_SRC_HFXT);
	if (ret != -ENOTSUP) {
		zassert_ok(ret, "configure(HFCLK, HFXT) failed: %d", ret);

		zassert_equal(clock_get_rate(MSPM0_CLOCK_MCLK), clock_get_rate(HSCLK_SYSPLL_TAP),
			      "MCLK<-HSCLK<-SYSPLL<-HFCLK<-HFXT rate mismatch");
	}

	ret = clock_configure(MSPM0_CLOCK_HFCLK, MSPM0_CLOCK_SRC_HFCLK_IN);
	if (ret != -ENOTSUP) {
		zassert_ok(ret, "configure(HFCLK, HFCLK_IN) failed: %d", ret);

		zassert_equal(clock_get_rate(MSPM0_CLOCK_MCLK), clock_get_rate(HSCLK_SYSPLL_TAP),
			      "MCLK<-HSCLK<-SYSPLL<-HFCLK<-HFCLK_IN rate mismatch");
	}
}

/* mclk<-hsclk<-syspll<-sysosc */
ZTEST(clock_control_mspm0, test_mclk_hsclk_syspll_sysosc_chain)
{
	if (!clock_configure_chain_step(MSPM0_CLOCK_SYSPLL, MSPM0_CLOCK_SRC_SYSOSC)) {
		return;
	}
	zassert_equal(clock_status(MSPM0_CLOCK_SYSPLL), CLOCK_CONTROL_STATUS_ON,
		      "SYSPLL did not report ON after startup");

	if (!clock_configure_chain_step(MSPM0_CLOCK_HSCLK, MSPM0_CLOCK_SRC_SYSPLL)) {
		return;
	}

	if (!clock_configure_chain_step(MSPM0_CLOCK_MCLK, MSPM0_CLOCK_SRC_HSCLK)) {
		return;
	}

	zassert_equal(clock_get_rate(MSPM0_CLOCK_MCLK), clock_get_rate(HSCLK_SYSPLL_TAP),
		      "MCLK<-HSCLK<-SYSPLL<-SYSOSC rate mismatch");
}

/* mclk<-hsclk<-sysosc; only legal when the SoC has no SYSPLL to source HSCLK from. */
ZTEST(clock_control_mspm0, test_mclk_hsclk_sysosc_chain)
{
	int ret;

	if (!clock_configure_chain_step(MSPM0_CLOCK_HSCLK, MSPM0_CLOCK_SRC_SYSOSC)) {
		return;
	}

	ret = clock_configure(MSPM0_CLOCK_MCLK, MSPM0_CLOCK_SRC_HSCLK);
	zassert_ok(ret, "configure(MCLK, HSCLK) failed: %d", ret);
	clock_track(MSPM0_CLOCK_MCLK);

	zassert_equal(clock_get_rate(MSPM0_CLOCK_MCLK), clock_get_rate(MSPM0_CLOCK_SYSOSC),
		      "MCLK<-HSCLK<-SYSOSC rate mismatch");
}

ZTEST(clock_control_mspm0, test_unhandled_subsys)
{
	uint32_t rate;

	zassert_equal(clock_status(0xFFFFFFFF), CLOCK_CONTROL_STATUS_UNKNOWN);
	zassert_equal(clock_get_rate_raw(0xFFFFFFFF, &rate), -ENOTSUP);
}

static void *mspm0_clock_control_setup(void)
{
	zassert_true(device_is_ready(clk_dev), "clock control device not ready");

	return NULL;
}

/* Runs after every test, pass or fail, and restores whatever clock_track() recorded to its DT */
static void mspm0_clock_control_after(void *fixture)
{
	ARG_UNUSED(fixture);

	while (tracked_clocks_len > 0) {
		uint32_t clk = tracked_clocks[--tracked_clocks_len];
		enum mspm0_clock_source src = clock_default_source(clk);
		int ret = clock_configure(clk, src);
		zassert_ok(ret, "failed to restore clk 0x%x to its default source %d: %d", clk, src,
			   ret);
	}
}

ZTEST_SUITE(clock_control_mspm0, NULL, mspm0_clock_control_setup, NULL, mspm0_clock_control_after,
	    NULL);
