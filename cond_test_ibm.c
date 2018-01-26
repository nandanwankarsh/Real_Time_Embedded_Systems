//#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>


int                 conditionMet = 0;
pthread_cond_t      cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;

struct sched_param param[5];
pthread_attr_t tattr[5];

#define NTHREADS    5

void set_priority(){

	param[0].sched_priority = 86; 
	param[1].sched_priority = 90; // " "
	param[2].sched_priority = 68; // " "
	param[3].sched_priority = 78; // " "
	param[4].sched_priority = 65;

	int error_status2[4];

	
	for(int j=0;j<5;j++){
		//thread attribute creation
		error_status2[j] = pthread_attr_init(&tattr[j]);
		if(error_status2[j] != 0){
			printf("attr_init error %d= %d\n",j,error_status2[j]);
		}

		//Thread attribute initialization
		error_status2[j] = pthread_attr_setinheritsched(&tattr[j],PTHREAD_EXPLICIT_SCHED);
		if(error_status2[j] != 0){
			printf("thread %d setinherit_sched error = %d\n",j,error_status2[j]);
		}

		//Thread policy initialization
		error_status2[j] = pthread_attr_setschedpolicy(&tattr[j],SCHED_FIFO);
		if(error_status2[j] != 0 ){
			printf("thread %d setschedpolicy error = %d\n",j,error_status2[j]);
		}
	
		//Thread priority initialization
		error_status2[j] = pthread_attr_setschedparam(&tattr[j], &param[j]);
		if(error_status2[j] != 0){
			printf("thread %d setschedparam error = %d\n", j,error_status2[j]);
		} 
	}
}
void wait_cond_var(int *i){

	pthread_mutex_lock(&mutex);
	int policy;
	printf("Thread idex %d\n",*i);
	while (!conditionMet) {
		printf("Thread %ld blocked\n",pthread_self());
		pthread_getschedparam(pthread_self(), &policy, &param[*i]);
		pthread_cond_wait(&cond, &mutex);
	}
	printf("Thread %ld Executed\n",pthread_self());
	pthread_mutex_unlock(&mutex);

}

void broad_cond_var(){

	pthread_mutex_lock(&mutex);
	conditionMet = 1;
	printf("Wake up all waiting threads...\n");
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
	printf("Wait for threads and cleanup\n");
}



void *threadfunc(void *parm)
{
	int *i;
	i=(int *)parm;
	wait_cond_var(i);
	return NULL;
}

int main()
{

	int i,error_status;
	pthread_t threadid[NTHREADS];
	set_priority();
	//printf("Enter Testcase - %s\n", argv[0]);
	printf("Create %d threads\n", NTHREADS);
	for(i=0; i<NTHREADS; i++) {
		error_status = pthread_create(&threadid[i], &tattr[i], threadfunc, &i);
		if(error_status == EPERM){
					printf("thread create error status \n");
				} 	
	
	}
	sleep(2); 
	broad_cond_var();


	for (i=0; i<NTHREADS; ++i) {
		pthread_join(threadid[i], NULL);
	
	}
	pthread_cond_destroy(&cond); 
	pthread_mutex_destroy(&mutex);

	printf("Main completed\n");
	return 0;
}
