# Enable 32kHz clock signal and high-speed clock signal on test pin

The nRF54L15 enables both the 32 kHz clock signal and a divided high-speed clock signal to be made available on a pin. 

> __Note:__ This example has been tested with nRF Connect SDK v3.1.0.

## Step-by-Step Description

1) The following directories and files are required:

        clkout/       
           ├── CMakeLists.txt 
           ├── prj.conf
           ├── nrf54l15dk_nrf54l15_cpuapp.overlay
           │    └── src/main.c

> __NOTE:__ We are using here the board target _nrf54l15dk/nrf54l15/cpuapp_. This means that the project is located in Secure Domain. This is important because the divided high-speed clock can only be activated in Secure Domain!

2) Let's define a new node in the DeviceTree source file (*.DTS):

   <sup> _nrf54L15dk_nrf54l15_cpuapp.overlay_ </sup>

       / {
           testpin {
                    compatible = "gpio-leds";

                    clkoutfast: clkoutfast {
                                gpios = <&gpio1 8 GPIO_ACTIVE_HIGH>;
                    };

           };
       };

> __NOTE:__ We are defining here a new node for the testpins. This project has only the pin definition for the CLKOUTFAST pin. The 32kHz clock output does not require a DTS entry.

3) Let's add the prj.conf file:

   <sup> _prj.conf_ </sup>

       CONFIG_NRFX_GRTC=y
   
4) Now add main.c file:

   <sup> _main.c_ </sup>

       /*
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

5) Check the output signals on pins P0.04 (=> 32kHz signal) and P1.08 (=> divided high-speed clock). 
