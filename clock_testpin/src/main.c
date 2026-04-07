/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <haly/nrfy_grtc.h>
#include <hal/nrf_gpio.h>

#define NODE_CLKOUT  DT_NODELABEL(clkoutfast)
#define PIN_CLKOUT   NRF_DT_GPIOS_TO_PSEL(NODE_CLKOUT, gpios)

int main(void)
{
	printk("Hello World! %s\n", CONFIG_BOARD_TARGET);

	nrfy_grtc_clkout_set(NRF_GRTC, NRF_GRTC_CLKOUT_32K, true);
	k_msleep(1000);
	nrfy_grtc_clkout_set(NRF_GRTC, NRF_GRTC_CLKOUT_32K, false);

	k_msleep(2000);

	/* Give control of p1.08 to GRTC */
	nrf_gpio_pin_control_select(PIN_CLKOUT, NRF_GPIO_PIN_SEL_GRTC);

	/* Set divider */
	uint8_t grtc_div = 250;
	nrfy_grtc_clkout_divider_set(NRF_GRTC, grtc_div);

	nrfy_grtc_clkout_set(NRF_GRTC, NRF_GRTC_CLKOUT_FAST, true);
	k_msleep(1000);
	nrfy_grtc_clkout_set(NRF_GRTC, NRF_GRTC_CLKOUT_FAST, false);

	while(1);

	return 0;
}
