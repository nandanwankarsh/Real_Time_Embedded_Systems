#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include "test.h"



int main()
{
	FILE *fp;
	char line1[10];
	char *line1_str[2];
	int i=0,j=0,error_status;
	pthread_t threadid[num_lines];
	
	sem_init(&sem1, 0, 0);
	printf("PID is %d\n",getpid());
	
//	struct timespec now,terminate;
//	clock_gettime(CLOCK_MONOTONIC, &now);

	// int ch;

	if ((fp=fopen("readme.txt","r")) == NULL)
		printf("Cannot open file\n");


	fgets(line1, sizeof(line1), fp);
		printf("%s\n", line1);

	char * token;
	printf ("Splitting string \"%s\" into tokens:\n",line1);
	token = strtok (line1," ");
	while (token != NULL){
		printf ("%s\n",token);
		line1_str[i] = token;
		if (i==0){
			num_lines = atoi(line1_str[i]);
		}
		if (i==1){
			exec_time = atoi(line1_str[i]);
		}
			
		i++;
		token = strtok (NULL, " ");
    }
    printf("Numbere of lines %d and exec time %d \n", num_lines,exec_time);

//	terminate.tv_nsec = exec_time*1000000;

	char line[num_lines][100];

	for (i = 0; i < num_lines; i++){
		fgets(line[i], sizeof(line[i]), fp);
		printf("%s\n", line[i]);
	}

	int prio[num_lines];

	struct node *head[num_lines];
	for (i = 0; i < num_lines; i++){
		// head[i]=(struct node*)malloc(sizeof(struct node));
		// head[i]->pthread_id_l=10;
		head[i] = create(line[i]);
		prio[i] = atoi(head[i]->next->data);
		printf("Priority is %d\n",prio[i]);

	}
	for (i = 0; i < num_lines; i++){
		//printf("%s\n", head[i]->data);
		print(head[i]);
		//printf("**\n");
	}

	set_priority(prio);

	for (j = 0; j < 10; j++){
		
		pthread_mutexattr_init(&mtx_attr[j]);
	//	pthread_mutexattr_setprotocol(&mtx_attr[j],PTHREAD_PRIO_INHERIT);
	//	pthread_mutex_init(&mtx[j], &mtx_attr[j]);
	}
	//pthread_mutexattr_setprotocol(&mutex_attr,PTHREAD_PRIO_INHERIT);
	//pthread_mutexattr_setprotocol(&ap_mutex_attr,PTHREAD_PRIO_INHERIT);
	if(pthread_cond_init(&cond,NULL)<0)
		printf("Cond var main error");
	if(pthread_cond_init(&ap_cond_0,NULL)<0)
		printf("Cond var ap_0 error");
	if(pthread_cond_init(&ap_cond_1,NULL)<0)
		printf("Cond var ap_1 error");

	
	error_status = pthread_create( &thread_id1, &tattr1, &mouse_click, NULL);
	if(error_status != 0){
		printf("thread create error status");
	}

	printf("Create %d threads\n", num_lines);
	for(i=0; i<num_lines; i++) {
		error_status = pthread_create(&threadid[i], &tattr[i], &threadfunc, head[i]);
		if(error_status != 0){
					printf("thread create error status \n");
				} 	
	
	}

	

	sleep(3); 
	error_status = pthread_create( &t_thread, &tattr2 , &termination,threadid);
	broad_cond_var();

/*	if((now.tv_nsec+terminate.tv_nsec)>=1000000000){
		now.tv_nsec = (now.tv_nsec+terminate.tv_nsec)%1000000000;
		now.tv_sec++;

	}
	else{   
		now.tv_nsec = now.tv_nsec+terminate.tv_nsec;
	}
	now.tv_nsec = now.tv_nsec+terminate.tv_nsec;
	clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &now, 0);

	termination_flag = 1;
*/
//	sem_wait(&sem1);

//	for (i=0; i<num_lines; ++i) {
		//usleep(1000);
//		if(pthread_join(threadid[i], NULL)<0)
			//printf("Deadlock found %lu\n",(unsigned long)threadid[i]);
	
//	}
/*	if(pthread_join(thread_id1, NULL)<0)
		printf("termaination thread not joined");
*/
	pthread_join(t_thread, NULL);
			

	/*	pthread_cond_destroy(&cond); 
		pthread_cond_destroy(&ap_cond_0); 
		pthread_cond_destroy(&ap_cond_1); 
		pthread_mutex_destroy(&mutex);
		pthread_mutex_destroy(&ap_mutex);

		for (j = 0; j < 10; j++){
			printf("Mutex %d\n",j);
			if(pthread_mutex_destroy(&mtx[j])==EPERM)
				printf("Error mutex destroy");
				
		}

		printf("Main completed\n");
 */

	for(i=0;i<num_lines;i++){
		cleanup(head[i]);
	}
	close(fd);
	fclose(fp);

	return 0;
}
