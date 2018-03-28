 /* SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <pwm.h>
#include <asm_inline_gcc.h>
#include <pinmux.h>

#define PWM0 CONFIG_PWM_PCA9685_0_DEV_NAME
#define GPIO CONFIG_GPIO_DW_0_NAME
#define MUX CONFIG_PINMUX_NAME 
#define SLEEP_TIME 	2000

void main(void)
{
	struct device *pwm_dev;
	struct device *pinmux;

	pwm_dev = device_get_binding(PWM0);
	pinmux = device_get_binding(MUX);
	pinmux_pin_set(pinmux, 5, PINMUX_FUNC_C);

	pwm_pin_set_cycles(pwm_dev,3,4095,1000);

	printk("Hello World! %s\n", CONFIG_ARCH);

	


	while (1) {

		//Do nothing

	}
}

