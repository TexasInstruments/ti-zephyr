/*
 * SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/drivers/dma.h>
#include <zephyr/dt-bindings/dma/ti-mspm0-dma.h>
#include <zephyr/ztest.h>

/* needs to be a 64 bit aligned for table mode */
#define DMA_ALIGNMENT  (8)
#define DMA_TIMEOUT_MS (10)
#define FILL_PATTERN   0xDEADBEEF
#define DMA_BUF_SIZE   ROUND_UP(CONFIG_DMA_XFER_SIZE, DMA_ALIGNMENT)

static __aligned(DMA_ALIGNMENT) uint8_t rx8[DMA_BUF_SIZE];
static __aligned(DMA_ALIGNMENT) uint8_t tx8[DMA_BUF_SIZE];

static uint16_t *rx16 = (uint16_t *)rx8;

static uint32_t *rx32 = (uint32_t *)rx8;
static uint32_t *tx32 = (uint32_t *)tx8;

K_SEM_DEFINE(xfer_sem, 0, 1);

static void dma_callback(const struct device *dma_dev, void *user_data, uint32_t channel,
			 int status)
{
	k_sem_give(&xfer_sem);
}

const struct device *dma = DEVICE_DT_GET(DT_NODELABEL(tst_dma0));
static struct dma_block_config dma_block_cfg;
static struct dma_config dma_cfg = {
	.channel_direction = MEMORY_TO_MEMORY,
	.dma_callback = dma_callback,
	.block_count = 1,
	.head_block = &dma_block_cfg,
	.complete_callback_en = true,
};

static void reset_buffers(void)
{
	(void)memset(tx8, 0xAA, sizeof(tx8));
	(void)memset(rx8, 0xBB, sizeof(rx8));
}

static int do_dma_transfer(void)
{
	int ret = TC_FAIL;
	int chan_id = dma_request_channel(dma, NULL);
	if (chan_id < 0) {
		TC_PRINT("failed to request dma channel\n");
		return TC_FAIL;
	}

	if (dma_config(dma, chan_id, &dma_cfg) != 0) {
		TC_PRINT("failed to configure dma (%d)\n", chan_id);
		goto cleanup;
	}

	if (dma_start(dma, chan_id) != 0) {
		TC_PRINT("failed to start dma transfer (%d)\n", chan_id);
		goto cleanup;
	}

	if (k_sem_take(&xfer_sem, K_MSEC(DMA_TIMEOUT_MS)) != 0) {
		TC_PRINT("timed out waiting for xfer\n");
		goto cleanup;
	}

	ret = TC_PASS;

cleanup:
	k_sem_reset(&xfer_sem);
	dma_release_channel(dma, chan_id);
	return ret;
}

static int gather_mode(enum dma_addr_adj src_adj)
{
	const int entries = CONFIG_DMA_XFER_SIZE / sizeof(uint32_t);
	static uint32_t data[CONFIG_DMA_XFER_SIZE / sizeof(uint32_t)];
	uint32_t source_address;

	reset_buffers();

	switch (src_adj) {
	case DMA_ADDR_ADJ_INCREMENT:
		TC_PRINT("[GATHER] increment started...\n");
		source_address = (uint32_t)&tx32[0];

		for (int i = 0; i < entries; i++) {
			tx32[i] = (uint32_t)&data[i];
			data[i] = i;
		}

		break;
	case DMA_ADDR_ADJ_DECREMENT:
		TC_PRINT("[GATHER] decrement started...\n");
		source_address = (uint32_t)&tx32[entries - 1];

		for (int i = 0; i < entries; i++) {
			tx32[entries - i - 1] = (uint32_t)&data[i];
			data[i] = i;
		}
		break;
	default:
		TC_PRINT("Invalid adjustment %u\n", src_adj);
		return TC_FAIL;
	}

	dma_cfg.dma_slot = TI_MSPM0_DMA_SLOT(0, GATHER);

	dma_block_cfg = (struct dma_block_config){
		.block_size = CONFIG_DMA_XFER_SIZE,
		.source_address = source_address,
		.dest_address = (uint32_t)rx32,
		.source_addr_adj = src_adj,
	};

	if (do_dma_transfer() != TC_PASS) {
		return TC_FAIL;
	}

	for (int i = 0; i < entries; i++) {
		if (rx32[i] != i) {
			TC_PRINT("failed to match [%d], expected=0x%x, got=0x%x\n", i, i, rx32[i]);
			return TC_FAIL;
		}
	}

	return TC_PASS;
}

static int table_mode(enum dma_addr_adj src_adj)
{
	const int entries = CONFIG_DMA_XFER_SIZE / sizeof(uint64_t);
	uint32_t source_address;

	reset_buffers();

	switch (src_adj) {
	case DMA_ADDR_ADJ_INCREMENT:
		TC_PRINT("[TABLE] increment started...\n");
		source_address = (uint32_t)&tx32[0];
		break;
	case DMA_ADDR_ADJ_DECREMENT:
		TC_PRINT("[TABLE] decrement started...\n");
		source_address = (uint32_t)&tx32[entries * 2 - 2];
		break;
	default:
		TC_PRINT("Invalid adjustment %u\n", src_adj);
		return TC_FAIL;
	}

	for (int i = 0; i < entries; i++) {
		tx32[i * 2] = (uint32_t)&rx32[i];
		tx32[i * 2 + 1] = i;
	}

	dma_cfg.dma_slot = TI_MSPM0_DMA_SLOT(0, TABLE);

	dma_block_cfg = (struct dma_block_config){
		.block_size = CONFIG_DMA_XFER_SIZE,
		.source_address = source_address,
		.source_addr_adj = src_adj,
	};

	if (do_dma_transfer() != TC_PASS) {
		return TC_FAIL;
	}

	for (int i = 0; i < entries; i++) {
		if (rx32[i] != i) {
			TC_PRINT("failed to match [%d], expected=0x%x, got=0x%x\n", i, i, rx32[i]);
			return TC_FAIL;
		}
	}

	return TC_PASS;
}

static int fill_mode(uint8_t delta, enum dma_addr_adj src_adj, uint8_t width)
{
	int increment;

	reset_buffers();

	switch (src_adj) {
	case DMA_ADDR_ADJ_INCREMENT:
		increment = delta;
		break;
	case DMA_ADDR_ADJ_DECREMENT:
		increment = -delta;
		break;
	default:
		increment = 0;
		break;
	}

	TC_PRINT("[FILL] increment=%+d width=%u started...\n", increment, width);
	dma_cfg.dma_slot = TI_MSPM0_DMA_SLOT(0, FILL);
	dma_cfg.source_data_size = delta;
	dma_cfg.dest_data_size = width;

	dma_block_cfg = (struct dma_block_config){
		.block_size = CONFIG_DMA_XFER_SIZE,
		.source_address = FILL_PATTERN,
		.dest_address = (uint32_t)rx8,
		.source_addr_adj = src_adj,
	};

	if (do_dma_transfer() != TC_PASS) {
		return TC_FAIL;
	}

	for (int i = 0; i < CONFIG_DMA_XFER_SIZE / width; i++) {
		uint32_t expected = (FILL_PATTERN + (i * increment));
		uint32_t got;

		switch (width) {
		case 1:
			expected &= 0xff;
			got = rx8[i];
			break;
		case 2:
			expected &= 0xffff;
			got = rx16[i];
			break;
		case 4:
			got = rx32[i];
			break;
		default:
			TC_PRINT("invalid width %u\n", width);
			return TC_FAIL;
		}

		if (expected != got) {
			TC_PRINT("failed to match [%d], expected=0x%x, got=0x%x\n", i, expected,
				 got);
			return TC_FAIL;
		}
	}

	return TC_PASS;
}

ZTEST(mspm0_extended_modes, test_fill_mode)
{
	for (int width = 1; width <= 4; width *= 2) {
		zassert_true((fill_mode(1, DMA_ADDR_ADJ_NO_CHANGE, width) == TC_PASS));
		for (int delta = 1; delta <= 8; delta *= 2) {
			zassert_true((fill_mode(delta, DMA_ADDR_ADJ_INCREMENT, width) == TC_PASS));
			zassert_true((fill_mode(delta, DMA_ADDR_ADJ_DECREMENT, width) == TC_PASS));
		}
	}
}

ZTEST(mspm0_extended_modes, test_table_mode)
{
	zassert_true((table_mode(DMA_ADDR_ADJ_INCREMENT) == TC_PASS));
	zassert_true((table_mode(DMA_ADDR_ADJ_DECREMENT) == TC_PASS));
}

ZTEST(mspm0_extended_modes, test_gather_mode)
{
	zassert_true((gather_mode(DMA_ADDR_ADJ_INCREMENT) == TC_PASS));
	zassert_true((gather_mode(DMA_ADDR_ADJ_DECREMENT) == TC_PASS));
}

static void *mspm0_dma_setup(void)
{
	zassert_true(device_is_ready(dma), "DMA controller not ready");
	return NULL;
}

ZTEST_SUITE(mspm0_extended_modes, NULL, mspm0_dma_setup, NULL, NULL, NULL);
