 /* SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <asm_inline_gcc.h>

#define SLEEP_TIME 	2000

#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)

/*
#define MY_STACK_SIZE 500
#define MY_PRIORITY 5
#define MY_PRIORITY1 8

extern void my_entry_point(void* , void *, void *);
extern void my_entry_point1(void* , void *, void *);

*/
// u32_t start_time;
long long start_time;
// u32_t stop_time;
long long stop_time;
// u32_t diff;
long long diff;
u32_t cycles_spent;
// u32_t nanoseconds_spent;
long long milliseconds_spent;

static struct gpio_callback callb;
/*
K_THREAD_STACK_DEFINE(my_stack_area, MY_STACK_SIZE);
K_THREAD_STACK_DEFINE(my_stack_area1, MY_STACK_SIZE);
struct k_thread my_thread_data, my_thread_data1;

void my_entry_point(void* a ,void* b,void* c){

	while(1){
		printk("The thread is 1 and id is %ld\n", k_current_get());
		k_sleep(100);	
	}
	
}

void my_entry_point1(void* a ,void* b,void* c){
	
	while(1){
		printk("The thread is 2 and id is %ld\n", k_current_get());
		k_sleep(1000);	
	}

}
*/

void gpio_set();

void handler(struct device *dev6, struct gpio_callback *callb, u32_t pin){
	printk("In handler wohoo!!! at %d\n", k_cycle_get_32());
}

void main(void)
{
	int cnt = 0,flag;
	struct device *dev1;
	struct device *dev2;
	struct device *dev3;
	struct device *dev4;
	struct device *dev5;
	struct device *dev6;
	
	printk("Hello World! %s\n", CONFIG_ARCH);

	gpio_set();
/*
	k_tid_t my_tid = k_thread_create(&my_thread_data, my_stack_area,
                                 K_THREAD_STACK_SIZEOF(my_stack_area),
                                 my_entry_point,
                                 NULL, NULL, NULL,
                                 MY_PRIORITY, 0, K_NO_WAIT);
	k_sleep(500);

	k_tid_t my_tid1 = k_thread_create(&my_thread_data1, my_stack_area,
                                 K_THREAD_STACK_SIZEOF(my_stack_area1),
                                 my_entry_point1,
                                 NULL, NULL, NULL,
                                 MY_PRIORITY1, 0, K_NO_WAIT);
	k_sleep(500);
*/
	
	gpio_init_callback(&callb, handler, BIT(0));

	gpio_add_callback(dev6, &callb);
	gpio_pin_enable_callback(dev6, 0);


	while (1) {
		/* Set pin to HIGH/LOW every 1 second */
		//printk("Inside while for \n");
		gpio_pin_write(dev3,4,cnt % 2);
		cnt++;
		// start_time = k_cycle_get_32();
		start_time = _tsc_read();
		k_sleep(SLEEP_TIME);
		// stop_time = k_cycle_get_32();
		stop_time = _tsc_read();
		diff = stop_time - start_time;
		milliseconds_spent = diff/400000;
		//nanoseconds_spent = SYS_CLOCK_HW_CYCLES_TO_NS(diff);
		printk("The time is %lld\n", milliseconds_spent);

	}
}

void gpio_set(){
	int cnt = 0,flag;
	struct device *dev1;
	struct device *dev2;
	struct device *dev3;
	struct device *dev4;
	struct device *dev5;
	struct device *dev6;


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

	dev4 = device_get_binding("EXP0");
	if(dev4==NULL)
		printk("Returning null 4");
	flag=gpio_pin_configure(dev4,2,GPIO_DIR_OUT);
	if(flag<0)
		printk("Error in gpio_pin_configure 4");
	flag=gpio_pin_write(dev4,2,1);
	if(flag<0)
		printk("Error in gpio_pin_write 4");



	dev5 = device_get_binding("PWM0");
	if(dev5==NULL)
		printk("Returning null 5");
	flag=gpio_pin_configure(dev5,2,GPIO_DIR_OUT);
	if(flag<0)
		printk("Error in gpio_pin_configure 5");
	flag=gpio_pin_write(dev5,2,0);
	if(flag<0)
		printk("Error in gpio_pin_write 2");


		
	dev6 = device_get_binding("GPIO_CW");
	if(dev6==NULL)
		printk("Returning null 6");
	flag=gpio_pin_configure(dev6,0, GPIO_DIR_IN | EDGE);
	if(flag<0)
		printk("Error in gpio_pin_configure 6");
	// flag=gpio_pin_write(dev6,0,0); 
	// if(flag<0)
	// 	printk("Error in gpio_pin_write 6");



}
