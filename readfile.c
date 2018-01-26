#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include "test.h"


int main()
{
	FILE *fp;
	char line1[10];
	char *line1_str[2];
	int i=0,error_status;
	pthread_t threadid[num_lines];
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
	for (int i = 0; i < num_lines; i++){
		create(i,line[i]);
	}
	for (int i = 0; i < num_lines; i++){
		print(i);
		printf("**\n");
	}

	set_priority();

	printf("Create %d threads\n", num_lines);
	for(i=0; i<num_lines; i++) {
		error_status = pthread_create(&threadid[i], &tattr[i], threadfunc, &i);
		if(error_status != 0){
					printf("thread create error status \n");
				} 	
	
	}

	sleep(5); 
	broad_cond_var();


	for (i=0; i<num_lines; ++i) {
		pthread_join(threadid[i], NULL);
	
	}
	pthread_cond_destroy(&cond); 
	pthread_mutex_destroy(&mutex);

	printf("Main completed\n");




	fclose(fp);

	return 0;
}