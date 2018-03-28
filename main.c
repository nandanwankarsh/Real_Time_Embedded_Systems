 /* SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <asm_inline_gcc.h>
#include <misc/util.h>

#define SLEEP_TIME 	2000
#define PULL_UP 0
#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH)
// #define MY_STACK_SIZE 500

#define MY_STACK_SIZE 500
#define MY_PRIORITY 5
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
// u32_t nanoseconds_spent;
long long milliseconds_spent;
int period=2,duty_cycle=50;
int cnt = 1;
int count=0;


K_THREAD_STACK_DEFINE(my_stack_area, MY_STACK_SIZE);
K_THREAD_STACK_DEFINE(my_stack_area1, MY_STACK_SIZE);
K_THREAD_STACK_DEFINE(pwm_stack_area, MY_STACK_SIZE);
K_THREAD_STACK_DEFINE(msg_q_stack_thread1, MY_STACK_SIZE);
K_THREAD_STACK_DEFINE(msg_q_stack_thread2, MY_STACK_SIZE);
struct k_thread my_thread_data, my_thread_data1,msg_q_thread1,msg_q_thread2;
struct k_mutex my_mutex;
struct k_thread pwm_thread;



static struct gpio_callback callb;
struct device *dev1;
struct device *dev2;
struct device *dev3;
struct device *dev4;
struct device *dev5;
struct device *dev6;


struct data_item_type {
  char msg[12];
};

struct data_item_type data;
char __aligned(4) my_msgq_buffer[10 * sizeof(data)];
struct k_msgq my_msgq;

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

		start_time=_tsc_read();
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
			stop_time=_tsc_read();

			// printk("RDTSC count start2:%llu\n",cont_swt_end);
			
			// printk("Mutex locked for thread 2\n");
			while(i<INT_MAX/100){
				i++;
			}	

			k_mutex_unlock(&my_mutex);
			printk("Context switch overhead:%llu\n",stop_time-start_time);
			stop_time=start_time=0;
			count++;
	 }



}


void gpio_set(void);
void pwm_gen(void* a,void* b,void* c);
void msg_q_1(void* a,void* b,void* c);
void msg_q_2(void* a,void* b,void* c);

void handler(struct device *dev6, struct gpio_callback *callb, u32_t pin){
	stop_time=_tsc_read();
	printk("In handler wohoo!!! at %lld\n",stop_time-start_time) ;
}

void main(void)
{
	int i=3;
	gpio_set();
	int flag;
	// struct device *dev1;
	// struct device *dev2;
	// struct device *dev3;
	// struct device *dev4;
	// struct device *dev5;
	// struct device *dev6;
	// for(i;i<=3;i++){
			switch(i){

			case 1:

					k_mutex_init(&my_mutex);
					printk("Hello World! %s\n", CONFIG_ARCH);
					// struct device *dev1;
					// struct device *dev2;
					// struct device *dev3;

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

					break;

			case 2:

					// printk("Hello World! %s\n", CONFIG_ARCH);
					printk("Check before thread creation\n");
					k_tid_t pwm_tid = k_thread_create(&pwm_thread, pwm_stack_area,
				                                 K_THREAD_STACK_SIZEOF(pwm_stack_area),
				                                 pwm_gen,
				                                 NULL, NULL, NULL,
				                                 0, 0, K_NO_WAIT);
					
					printk("check after sleep main\n");
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
					break;

			case 3:
					gpio_init_callback(&callb,handler, BIT(3));
					gpio_add_callback(dev6, &callb);
					gpio_pin_enable_callback(dev6,3);
					k_msgq_init(&my_msgq, my_msgq_buffer, sizeof(data),5);
					k_tid_t msgq_1_tid = k_thread_create(&msg_q_thread1,msg_q_stack_thread1,
				                                 K_THREAD_STACK_SIZEOF(msg_q_stack_thread1),msg_q_1,
				                                 NULL, NULL, NULL,
				                                 -1, 0, K_NO_WAIT);
					k_tid_t msgq_2_tid = k_thread_create(&msg_q_thread2,msg_q_stack_thread2,
				                                 K_THREAD_STACK_SIZEOF(msg_q_stack_thread2),
				                                 msg_q_2,
				                                 NULL, NULL, NULL,
				                                 -1, 0, K_NO_WAIT);

					printk("In main\n");


					break;

			default:
					printk("Default Case !!\n");
					break;

		}

	// }
 	
 }

void compute(void){
unsigned long i=0;
while(i<&i){
	i++;
}

}

void msg_q_1(void* a ,void* b,void* c){
	// printk("Inside MSG_Q thread1");
	int key=irq_lock();
	int j=0;
	while(j<3){
		
		char msg_data[12]="thread 1";
		for(int i=0;i<5;i++){
			
			k_msgq_put(&my_msgq, msg_data, K_FOREVER);
		}
		// k_msgq_put(&my_msgq, msg_data, K_FOREVER);
		start_time = _tsc_read();
		printk("Check before interrupt\n");
		gpio_pin_write(dev3,4,1);
		compute();
		gpio_pin_write(dev3,4,0);
		printk("Check after interrupt\n");
		compute();
		j++;
		k_sleep(20);
		
	}
	irq_unlock(key);

}

void msg_q_2(void* a ,void* b,void* c){

	// printk("Inside MSG_Q thread2");
	int key=irq_lock();
	int j=0;
	while(j<3){
		char *data_received;
		for(int i=0;i<5;i++){
			k_msgq_get(&my_msgq,data_received, K_FOREVER);
			printk("Message received is:%s\n",data_received);
		}
		compute();
		k_sleep(20);

	}
	irq_unlock(key);
}


void pwm_gen(void* a,void* b,void* c){
		int flag;
		while (cnt<=500) {
			/* Set pin to HIGH/LOW every 1 second */
			//printk("Inside while for \n");
			// start_time = _tsc_read();
			gpio_pin_write(dev3,4,cnt % 2);
			start_time = _tsc_read();
			cnt++;
			// start_time = k_cycle_get_32();
			// start_time = _tsc_read();
			if(cnt%2==0)
			k_sleep(period*duty_cycle);
			else
			k_sleep(period*(100-duty_cycle));
			// stop_time = k_cycle_get_32();
			// stop_time = _tsc_read();
			// diff = stop_time - start_time;
			// milliseconds_spent = diff/400000;
			// //nanoseconds_spent = SYS_CLOCK_HW_CYCLES_TO_NS(diff);
			// printk("The time is %lld\n", milliseconds_spent);
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
