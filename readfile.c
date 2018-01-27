#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include "test.h"



int main()
{
	FILE *fp;
	char line1[10];
	char *line1_str[2];
	int i=0,error_status;
	pthread_t threadid[num_lines],thread_id1;

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

	char line[num_lines][100];

	for (int i = 0; i < num_lines; i++){
		fgets(line[i], sizeof(line[i]), fp);
		printf("%s\n", line[i]);
	}

	int prio[num_lines];

	struct node *head[num_lines];
	for (int i = 0; i < num_lines; i++){
		// head[i]=(struct node*)malloc(sizeof(struct node));
		// head[i]->pthread_id_l=10;
		head[i] = create(line[i]);
		prio[i] = atoi(head[i]->next->data);
		printf("Priority is %d\n",prio[i]);

	}
	for (int i = 0; i < num_lines; i++){
		//printf("%s\n", head[i]->data);
		print(head[i]);
		//printf("**\n");
	}

	set_priority(prio);

	for (int j = 0; j < 10; j++){
		pthread_mutex_init(&mtx[j], NULL);
	}
	

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
	broad_cond_var();


	for (i=0; i<num_lines; ++i) {
		pthread_join(threadid[i], NULL);
	
	}
	pthread_join(thread_id1, NULL);
	pthread_cond_destroy(&cond); 
	pthread_mutex_destroy(&mutex);

	for (int j = 0; i < 10; j++){
		pthread_mutex_destroy(&mtx[j]);
	}
	printf("Main completed\n");




	fclose(fp);

	return 0;
}
