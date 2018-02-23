#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int num_tasks = 3;

int check_utilization(int num_tasks, float* taskset);
int busy_period(int num_tasks, float* taskset);

int main(){

	//float taskset[3*num_tasks];
	int result,count=0;

	float taskset[] = {2.0, 4.0, 6.0, 2.0, 5.0, 8.0, 3.0, 7.0, 9.0};
	//float taskset[] = {2.0, 6.0, 6.0, 2.0, 8.0, 8.0, 3.0, 9.0, 9.0};
	
	for (int i = 0; i < (3*num_tasks); i = i+3){

		if(taskset[i+1] == taskset[i+2]){

			count++;
		}
	}

	if (count == num_tasks){

		int u = check_utilization(num_tasks ,taskset);
		if (u == 0){
			
			printf("U is less than 1 so schedulable directly\n");
		}
		else
			printf("U is greater than 1 so not schedulable\n");

	}
	else{

		int t,j=0,count=0;
		int u = check_utilization(num_tasks ,taskset);
		if (u == 0){
			
			printf("U is less than 1 so schedulable\n");
		}
		else{
			printf("U is greater than 1 so need to do loading factor analysis\n");

			int L = busy_period(num_tasks ,taskset);

			printf("The busy_period is %d \n", L);

			while(t != L){
				int i = 0;
				for (i ; i < (3*num_tasks); i = i+3){
					

					if(t == L)
						break;
					else{
						t = taskset[i+1] + j*taskset[i+2];
						printf("Value of t is %d\n", t);
						count ++;
					}
				}

				j++;
			}

			printf("Value of t's count is %d\n", count);

		}
	}

	return result;
}

int busy_period(int num_tasks, float* taskset){

	int prev_L, L;

	for (int i = 0; i < 3*(num_tasks); i = i+3){
		
		prev_L += taskset[i];
	}
	printf("prev_L is %d \n",prev_L );
	
	while(1){

		for (int i = 0; i < 3*(num_tasks); i = i+3){
			
			L += (ceil(prev_L/taskset[i+2]))*taskset[i];
			
		}
		printf("L is %d \n",L );
		if(prev_L == L){
			break;
		}
		else{

			prev_L = L;
			L = 0;
		}
	}

	return L;
}

int check_utilization(int num_tasks, float* taskset){

	float u;
	int ret;
	for (int i = 0; i < (3*num_tasks); i = i+3){
		u += (taskset[i]/taskset[i+1]);
	}

	if (u <= 1){
		ret = 0;
	}
	else{
		ret = 1;
	}

	return ret;
}