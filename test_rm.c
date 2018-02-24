#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

void del(char str[], char ch);
void rm(char* taskset,int no_tasks);
int utilization_bound(int no_tasks, float* taskset);
int time_demand(int no_tasks, float* taskset);

void main(){

	FILE *fp;
	char line1[30];
	char *line1_str[2],*str_woeol;
	int num_lines,no_taskset;

	if ((fp=fopen("input.txt","r")) == NULL)
		printf("Cannot open file\n");

	fgets(line1, sizeof(line1), fp);
	del(line1,'\n');
	printf ("Splitting string \"%s\" into tokens:\n",line1);
	no_taskset=atoi(line1);
	// printf("No of taskset %d\n",no_taskset);
	for(int j = 0; j < no_taskset; j++){
		fgets(line1, sizeof(line1), fp);
		del(line1,'\n');	
		num_lines=atoi(line1);
		// printf("No. of tasks in taskset1 %d\n",num_lines);
		char line[num_lines][20];
		char *holder=(char*)malloc(20*num_lines);
		for (int i = 0; i < num_lines; i++){
			fgets(line[i], sizeof(line[i]), fp);
			del(line[i],'\n');
			strcat(line[i]," ");
			// printf("%s\n", line[i]);
			strcat(holder,line[i]);
			// printf("%s\n", line[i]);
		}
		// printf("%s\n", holder);
		rm(holder,num_lines);
		printf("\n");
	}
}

void del(char str[], char ch) {
	int i, j = 0;
	int size;
	char ch1;
	char str1[30];

	size = strlen(str);

	for (i = 0; i < size; i++) {
	  if (str[i] != ch) {
	     ch1 = str[i];
	     str1[j] = ch1;
	     j++;
	  }
	}
	str1[j] = '\0';
	strcpy(str,str1); 
	// printf("\ncorrected string is : %s \t %s", str1,str);
}

void rm(char* str_taskset,int no_tasks){

	float taskset[3*no_tasks];
	char *token; 
	int count=0;   
	token = strtok (str_taskset," ");
	for (int i=0;i<3*no_tasks;i++){
		taskset[i]=atof(token);
		token = strtok (NULL, " ");
		printf("%f \t",taskset[i]);
	}
	printf("\n");
	for (int i = 0; i < (3*no_tasks); i = i+3){

		if(taskset[i+1] == taskset[i+2]){

			count++;
		}
	}

	if (count == no_tasks){

	int u = utilization_bound(no_tasks ,taskset);
	if (u == 0){
		
		printf("U is less than 1 so schedulable directly\n");
	}
	else if(u == 1){

		printf("U is greater than 1 so not schedulable\n");
	}
	else
		printf("U is greater than utilization_bound and less than 1, so need to do time demand analysis\n");

		for (int nt = 1; nt <= no_tasks; nt++){

			if (utilization_bound(nt , taskset) != 0){

				printf("Task %d utilization results are inconclusive and requires response time analysis\n", nt);

				int td = time_demand(nt, taskset);

				if(td == 0){
					printf("Task %d is schedulable after response time analysis\n\n", nt);
				}
				else
					printf("Task %d is not schedulable\n\n", nt);
			}
			else{

				printf("Task %d is schedulable as U is under bound\n\n", nt );
			}

		}

	}
	else{
		printf("Do for Deadline < period\n");
	}
}

int utilization_bound(int no_tasks, float* taskset){

	float u=0,bound=0;
	int ret;
	for (int i = 0; i < (3*no_tasks); i = i+3){
		u += (taskset[i]/taskset[i+1]);
	}

	bound = no_tasks*(pow(2.0,(1.0/no_tasks)) - 1);

	printf("The bound for task %d is %.3f \n", no_tasks, bound);
	printf("And it's U is %.3f \n", u);
	if (u <= bound){
		ret = 0;
	}
	else if(u > 1){
		ret = 1;
	}
	else{
		ret = 2;
	}

	return ret;
}

int time_demand(int no_tasks, float* taskset){
	int ret; 
	float max_period=0, period=0,prev_a=0, a=0;

	for (int i = 0; i < 3*(no_tasks); i = i+3){
		
		prev_a += taskset[i];
	}

	printf("a is %.3f\n", prev_a );

	a = taskset[3*no_tasks - 3];

	while(1){

		for (int i = 0; i < 3*(no_tasks - 1); i = i+3){
			
			a += (ceil(prev_a/taskset[i+2]))*taskset[i];
			
		}
		printf("a is %.3f \n",a );
		if(prev_a == a){
			break;
		}
		else{

			prev_a = a;
			a = (int)taskset[3*no_tasks - 3];
		}
	}

	for (int i = 0; i < 3*(no_tasks); i = i+3){
		
		period = taskset[i+2];

		if(max_period >= period){
			//Do nothing
		}
		else{

			max_period = period;
		}
	}

	printf("Max period is %.3f\n", max_period);

	(a <= max_period) ? (ret = 0) : (ret = 1);

	return ret;

}
