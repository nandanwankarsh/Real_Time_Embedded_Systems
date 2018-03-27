/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <asm_inline_gcc.h>
#include <misc/util.h>
#include <misc/printk.h>


#define SLEEP_TIME 	500

#define MY_STACK_SIZE 500
#define MY_PRIORITY 10
#define MY_PRIORITY1 8

extern void my_entry_point(void* , void *, void *);
extern void my_entry_point1(void* , void *, void *);
// u32_t start_time;
long long start_time;
// u32_t stop_time;
long long stop_time;
// u32_t diff;
long long diff;
u32_t cycles_spent;
unsigned long long cont_swt_start=0,cont_swt_end=0;
// u32_t nanoseconds_spent;
long long milliseconds_spent;
int count=0;
K_THREAD_STACK_DEFINE(my_stack_area, MY_STACK_SIZE);
K_THREAD_STACK_DEFINE(my_stack_area1, MY_STACK_SIZE);
struct k_thread my_thread_data, my_thread_data1;
struct k_mutex my_mutex;

void my_entry_point(void* a ,void* b,void* c){
	while(count <20){
		// printk("Mutex locked for thread 1\n");
		if(k_mutex_lock(&my_mutex, K_FOREVER)!=0)
			printk("Mutex error tthread 1\n");
		 // while(1){
			long long i=0;
			// printk("The thread is 1:%d\n",k_current_get());
			k_sleep(20);

			// while(i<INT_MAX/10){
			// 	i++;
			// }

		cont_swt_start=_tsc_read();
		// printk("RDTSC count start1:%llu\n",cont_swt_start);	
		k_mutex_unlock(&my_mutex);
		// printk("Mutex unlocked for thread 1\n");
		
	}
	
}

void my_entry_point1(void* a ,void* b,void* c){
	

	  while(count<20){
			long i=0;
			// printk("The thread is 2:%d\n",k_current_get());
			k_sleep(2);
			// printk("The thread is 2 sleep bahar \n");
			if(k_mutex_lock(&my_mutex, K_FOREVER)!=0)
				printk("Mutex error tthread 1\n");
			cont_swt_end=_tsc_read();

			// printk("RDTSC count start2:%llu\n",cont_swt_end);
			
			// printk("Mutex locked for thread 2\n");
			while(i<INT_MAX/100){
				i++;
			}	

			k_mutex_unlock(&my_mutex);
			printk("Context switch overhead:%llu\n",cont_swt_end-cont_swt_start);
			cont_swt_end=cont_swt_start=0;
			count++;
	 }



}

void main(void)
{

	k_mutex_init(&my_mutex);
	printk("Hello World! %s\n", CONFIG_ARCH);
		int cnt = 0,flag;
	struct device *dev1;
	struct device *dev2;
	struct device *dev3;

	k_tid_t my_tid1 = k_thread_create(&my_thread_data1, my_stack_area1,
                             K_THREAD_STACK_SIZEOF(my_stack_area1),
                             my_entry_point1,
                             NULL, NULL, NULL,
                             MY_PRIORITY1, 0, K_NO_WAIT);
	printk("Thread 2 with threadid:%d\n",(int)my_tid1);

	k_tid_t my_tid = k_thread_create(&my_thread_data, my_stack_area,
                                 K_THREAD_STACK_SIZEOF(my_stack_area),
                                 my_entry_point,	
                                 NULL, NULL, NULL,
                                 MY_PRIORITY, 0, K_NO_WAIT);
	printk("Thread 1 with threadid:%d\n",(int)my_tid);
	
	k_sleep(1000);

	// k_sleep(1000);
	
	

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
		//printk("The time is %lld\n", milliseconds_spent);

	}
}
