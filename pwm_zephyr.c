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
#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH)

static struct gpio_callback callb;
struct device *dev1;
struct device *dev2;
struct device *dev3;
struct device *dev4;
struct device *dev5;
struct device *dev6;

// u32_t start_time;
long long start_time;
// u32_t stop_time;
long long stop_time;
// u32_t diff;
long long diff;
u32_t cycles_spent;
// u32_t nanoseconds_spent;
long long milliseconds_spent;


void gpio_set();

void handler(struct device *dev6, struct gpio_callback *callb, u32_t pin){
	printk("In handler wohoo!!! at\n");
}

void main(void)
{
	int cnt = 0,flag;
	struct device *pwm_dev;
	struct device *pinmux;

	pwm_dev = device_get_binding(PWM0);
	pinmux = device_get_binding(MUX);
	pinmux_pin_set(pinmux, 5, PINMUX_FUNC_C);

	pwm_pin_set_cycles(pwm_dev,3,4095,1000);

	gpio_set();

	printk("Hello World! %s\n", CONFIG_ARCH);




					

	gpio_init_callback(&callb,handler, BIT(3));

	gpio_add_callback(dev6, &callb);
	gpio_pin_enable_callback(dev6,3);
	int val=0;
	// while(1){
	// 	// int val=0;
	// 	gpio_pin_read(dev6,3, &val);
	// 	printk("read value:%d\n",val);
	// 	k_sleep(1000);
		
	// }
	k_sleep(10000);
	printk("End\n");

	while (1) {

		//Do nothing

	}
}

void gpio_set(){
	int cnt = 0,flag;
	// struct device *dev1;
	// struct device *dev2;
	// struct device *dev3;
	// struct device *dev4;
	// struct device *dev5;
	// struct device *dev6;
	printk("Inside gpio set\n");

	dev1 = device_get_binding("EXP0");
	if(dev1==NULL)
		printk("Returning null 1");
	flag=gpio_pin_configure(dev1,12,GPIO_DIR_OUT);
	if(flag<0)
		printk("Error in gpio_pin_configure 1");
	flag=gpio_pin_write(dev1,12,0);
	if(flag<0)
		printk("Error in gpio_pin_write 1");



	dev2 = device_get_binding("EXP1");
	if(dev2==NULL)
		printk("Returning null 2");
	flag=gpio_pin_configure(dev2,13,GPIO_DIR_OUT);
	if(flag<0)
		printk("Error in gpio_pin_configure 2");
	flag=gpio_pin_write(dev2,13,0);
	if(flag<0)
		printk("Error in gpio_pin_write 2");


		
	dev3 = device_get_binding("GPIO_0");
	if(dev3==NULL)
		printk("Returning null 3");
	flag=gpio_pin_configure(dev3,4, GPIO_DIR_OUT);
	if(flag<0)
		printk("Error in gpio_pin_configure 3");
	flag=gpio_pin_write(dev3,4,0); 
	if(flag<0)
		printk("Error in gpio_pin_write 3");

	/* Set LED pin as output */
	// gpio_pin_configure(dev, LED, GPIO_DIR_OUT);

	dev4 = device_get_binding("EXP1");
	if(dev4==NULL)
		printk("Returning null 4");
	flag=gpio_pin_configure(dev4,0,GPIO_DIR_OUT);
	if(flag<0)
		printk("Error in gpio_pin_configure 4");
	flag=gpio_pin_write(dev4,0,1);
	if(flag<0)
		printk("Error in gpio_pin_write 4");



	dev5 = device_get_binding("EXP1");
	if(dev5==NULL)
		printk("Returning null 5");
	flag=gpio_pin_configure(dev5,1,GPIO_DIR_OUT);
	if(flag<0)
		printk("Error in gpio_pin_configure 5");
	flag=gpio_pin_write(dev5,1,0);
	if(flag<0)
		printk("Error in gpio_pin_write 5");

	// GPIO_DIR_IN | EDGE | PULL_UP
		
	dev6 = device_get_binding("GPIO_0");
	if(dev6==NULL)
		printk("Returning null 6");
	flag=gpio_pin_configure(dev6,3,GPIO_DIR_IN | GPIO_INT | EDGE);
	if(flag<0)
		printk("Error in gpio_pin_configure 6");
	// flag=gpio_pin_write(dev6,0,0); 
	if(flag<0)
		printk("Error in gpio_pin_write 6");
	printk("gpio_set end\n");



}
