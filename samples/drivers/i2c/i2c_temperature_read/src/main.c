/*
 * Copyright (c) 2026 Texas Instruments
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define I2C_CONTROLLER_DEV_CONFIG        (I2C_SPEED_SET(I2C_SPEED_FAST) | I2C_MODE_CONTROLLER)

#define TMP_SENSOR_ADDR					 (0x48)
/* Temperature result registers */
#define TMP10X_RESULT_REG       		 (0x0000U)

static const struct device *i2c_controller = DEVICE_DT_GET(DT_NODELABEL(main_i2c1));

int main(void)
{
	LOG_DBG("I2C temperature read");

	if(i2c_configure(i2c_controller, I2C_CONTROLLER_DEV_CONFIG)) {
		LOG_ERR("Failed to configure I2C instance");
		return 0;
	}

	if (!device_is_ready(i2c_controller)) {
        LOG_ERR("I2C controller not ready");
		return 0;
    }

	uint8_t txbuffer[1] = {TMP10X_RESULT_REG};
	uint8_t rxbuffer[2] = {0};
	struct i2c_msg msg = {0};

	msg.buf   = txbuffer;
	msg.len	  = 1;
	msg.flags = I2C_MSG_WRITE;
	if(i2c_transfer(i2c_controller, &msg, 1, TMP_SENSOR_ADDR)) {
		LOG_ERR("Device register not accessible");
		return 0;
	}
	
	uint8_t isFailed = 0;
	for(int i=0; i<10; i++) {
		rxbuffer[0] = rxbuffer[1] = 0;
		msg.buf 	= rxbuffer;
		msg.len		= 2;
		msg.flags	= I2C_MSG_READ;
		if(i2c_transfer(i2c_controller, &msg, 1, TMP_SENSOR_ADDR)) {
			LOG_ERR("Failed to read temperature");
			isFailed = 1;
			break;
		}
		else {
			/* Create 16 bit temperature */
			uint16_t temperature = ((uint16_t)rxbuffer[0] << 8) | (rxbuffer[1]);
			/*
				* 4 LSBs of temperature are 0 according to datasheet
				* since temperature is stored in 12 bits. Therefore,
				* right shift by 4 places
				*/
			temperature = temperature >> 4;
			/*
				* If the 12th bit of temperature is set '1' (equivalent to 8th bit of the first byte read),
				* then we have a 2's complement negative value which needs to be sign extended
				*/
			if(rxbuffer[0] & 0x80)
			{
				temperature |= 0xF000;
			}
			LOG_DBG("[I2C] Sample %u: %f (celcius)", i+1, temperature/16.0);
		}
	}
	if(!isFailed)
	{
		LOG_DBG("All test passed");
	}
	return 0;
}
