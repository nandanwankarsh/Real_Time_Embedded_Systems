#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

void sort_dm(float taskset[][3],int no_tasks,int index);
void swap_dm(float *a,float *b);
int dm(float taskset[][3],int no_tasks);
int utilization_bound_dm(int no_tasks, float* taskset);
int time_demand_dm(int no_tasks, float* taskset);
float min_dm(float a,float b);

long sched_dm=0,non_sched_dm=0;

void swap_dm(float *a,float *b){
	for(int i=0;i<3;i++){
		a[i]=a[i]+b[i];
		b[i]=a[i]-b[i];
		a[i]=a[i]-b[i];

	}

}


void sort_dm(float taskset[][3],int no_tasks,int index){

	
	for (int i = 0; i < (no_tasks); i = i+1){

		for(int j=0;j< no_tasks-1;j++){

				if(taskset[j][index]>taskset[j+1][index])
					swap_dm(taskset[j],taskset[j+1]);
		}
		
	}

}


float min_dm(float a,float b){

	if(a<b)
		return a;
	else
		return b;
}

int dm(float taskset_mat[][3],int no_tasks){

	float taskset[3*no_tasks]; 
	int count=0;   
	sched_dm=non_sched_dm=0;
	
	for (int i=0;i<no_tasks;i++){
		for(int j=0;j<3;j++){

				taskset[i*3+j]=taskset_mat[i][j];
			}
		}


	for (int i = 0; i < (3*no_tasks); i = i+3){

		if(taskset[i+1] == taskset[i+2]){

			count++;
		}
	}

	if (count == no_tasks){

		printf("Deadline is equal to period for the taskset\n");

		sort_dm(taskset_mat,no_tasks,1);

		for (int i=0;i<no_tasks;i++){
			for(int j=0;j<3;j++){

				taskset[i*3+j]=taskset_mat[i][j];
			}
		}
		int u = utilization_bound_dm(no_tasks ,taskset);
		if (u == 0){
			
			printf("U is less than 1 so schedulable directly\n");
			sched_dm=1;
		}
		else if(u == 1){

			printf("U is greater than 1 so not schedulable\n");
			non_sched_dm=1;
		}
		else{
			printf("U is ater than utilization_bound_dm and less than 1, so need to do time demand analysis\n");

			for (int nt = 1; nt <= no_tasks; nt++){

				if (utilization_bound_dm(nt , taskset) != 0){

					printf("Task %d utilization results are inconclusive and requires response time analysis\n", nt);

					int td = time_demand_dm(nt, taskset);

					if(td == 0){
						printf("Task %d is schedulable after response time analysis\n\n", nt);
						sched_dm=1;
					}
					else{
						printf("Task %d is not schedulable\n\n", nt);
						non_sched_dm=1;
					}

				}
				else{

					printf("Task %d is schedulable as U is under bound\n\n", nt );
					sched_dm=1;
				}

			}
		}

	}
	else{
		printf("Here the Deadline < period for the given taskset\n");
		sort_dm(taskset_mat,no_tasks,1);
		for (int i=0;i<no_tasks;i++){
			for(int j=0;j<3;j++){

				taskset[i*3+j]=taskset_mat[i][j];
			}
		}
		int u = utilization_bound_dm(no_tasks ,taskset);
		if (u == 0){
			
			printf("U is less than 1 so schedulable directly\n");
			sched_dm=1;
		}
		else if(u == 1){

			printf("U is greater than 1 so not schedulable\n");
			non_sched_dm=1;
		}
		else{
			printf("U is greater than utilization_bound_dm and less than 1, so need to do time demand analysis\n");

			for (int nt = 1; nt <= no_tasks; nt++){

				if (utilization_bound_dm(nt , taskset) != 0){

					printf("Task %d utilization results are inconclusive and requires response time analysis\n", nt);

					int td = time_demand_dm(nt, taskset);

					if(td == 0){
						printf("Task %d is schedulable after response time analysis\n\n", nt);
						sched_dm=1;
					}
					else
						printf("Task %d is not schedulable\n\n", nt);
						non_sched_dm=1;
				}
				else{

					printf("Task %d is schedulable as U is under bound\n\n", nt );
					sched_dm=1;
				}

			}
		}

	}

if(sched_dm==1){

	return 1;
}
else
	return 0;
}



int utilization_bound_dm(int no_tasks, float* taskset){

	float u=0,bound=0;
	int ret;
	for (int i = 0; i < (3*no_tasks); i = i+3){
		u += (taskset[i]/taskset[i+1]);
	}

	bound = no_tasks*(pow(2.0,(1.0/no_tasks)) - 1);

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

int time_demand_dm(int no_tasks, float* taskset){
	int ret; 
	float max_period=0, period=0,prev_a=0, a=0;

	for (int i = 0; i < 3*(no_tasks); i = i+3){
		
		prev_a += taskset[i];
	}
 	

	a = taskset[3*no_tasks - 3];

	while(1){

		for (int i = 0; i < 3*(no_tasks - 1); i = i+3){
			
			a += (ceil(prev_a/taskset[i+2]))*taskset[i];
			
		}

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

	(a <= max_period) ? (ret = 0) : (ret = 1);

	return ret;

}
