#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

void sort_rm(float taskset[][3],int no_tasks,int index);
// void del(char str[], char ch);
void swap(float *a,float *b);
// void wrapper_rm(char* str_taskset,int no_tasks);
int rm(float taskset[][3],int no_tasks);
int utilization_bound(int no_tasks, float* taskset);
int time_demand(int no_tasks, float* taskset);
float min(float a,float b);
int eff_utilization_bound(float taskset[][3],int no_tasks);

long sched_rm=0,non_sched_rm=0;

void swap(float *a,float *b){
	for(int i=0;i<3;i++){
		a[i]=a[i]+b[i];
		b[i]=a[i]-b[i];
		a[i]=a[i]-b[i];

	}

}

// void wrapper_rm(char* str_taskset,int no_tasks){

// 	float taskset_mat[no_tasks][3],taskset[3*no_tasks];
// 	char *token; 
// 	int count=0;   
// 	token = strtok (str_taskset," ");
// 	for (int i=0;i<no_tasks;i++){
// 		for(int j=0;j<3;j++){
// 			taskset_mat[i][j]=atof(token);
// 			token = strtok (NULL, " ");
// 		//	printf("%f \t",taskset[i]);
// 		}
// 	}
// 	sort_rm(taskset_mat,no_tasks,1);

// 	for (int i=0;i<no_tasks;i++){
// 		for(int j=0;j<3;j++){

// 			taskset[i*3+j]=taskset_mat[i][j];
// 		}
// 	}

// 	for(int i=0;i<3*no_tasks;i++){

// 		printf("%f\t",taskset[i]);
// 	}

// }


void sort_rm(float taskset[][3],int no_tasks,int index){

	


	// for (int i = 0; i < (no_tasks); i = i+1){

	// 	for(int j=0;j< no_tasks;j++){

	// 			// if(taskset[j][index]<taskset[j+1][index])
	// 			// 	swap(taskset[j],taskset[j+1]);

	// 		printf("%f\t",taskset[i][j]);
	// 	}
	// 	 printf("\n");
	// }

	
	for (int i = 0; i < (no_tasks); i = i+1){

		for(int j=0;j< no_tasks-1;j++){

				if(taskset[j][index]>taskset[j+1][index])
					swap(taskset[j],taskset[j+1]);

			// printf("%f\t",taskset[i][j]);
		}
		// printf("\n");
	}
	// printf("After sorting\n");
	// for (int i = 0; i < (no_tasks); i = i+1){

	// 	for(int j=0;j< no_tasks;j++){

	// 			// if(taskset[j][index]<taskset[j+1][index])
	// 			// 	swap(taskset[j],taskset[j+1]);

	// 		printf("%f\t",taskset[i][j]);
	// 	}
	// 	 printf("\n");
	// }




}

// void del(char str[], char ch) {
// 	int i, j = 0;
// 	int size;
// 	char ch1;
// 	char str1[30];

// 	size = strlen(str);

// 	for (i = 0; i < size; i++) {
// 	  if (str[i] != ch) {
// 	     ch1 = str[i];
// 	     str1[j] = ch1;
// 	     j++;
// 	  }
// 	}
// 	str1[j] = '\0';
// 	strcpy(str,str1); 
// 	// printf("\ncorrected string is : %s \t %s", str1,str);
// }

float min(float a,float b){

	if(a<b)
		return a;
	else
		return b;
}

int rm(float taskset_mat[][3],int no_tasks){

	// float taskset_mat[no_tasks][3];
	// char *token; 
	float taskset[no_tasks*3];
	int count=0; 
	sched_rm=non_sched_rm=0;
	//printf("%s\n",str_taskset);  
	// token = strtok (str_taskset," ");
	// for (int i=0;i<no_tasks;i++){
	// 	for(int j=0;j<3;j++){
	// 		printf("Test Test RM %s \n",token);
	// 		// float f=atof(token); 
	// 		taskset_mat[i][j]=atof(token);
	// 		token = strtok (NULL, " ");
	// 		// printf("%f \t",taskset[i]);
	// 	}
	// }
	//printf("Test Test \n");
	for (int i=0;i<no_tasks;i++){
		for(int j=0;j<3;j++){

				taskset[i*3+j]=taskset_mat[i][j];
			}
		}

	// for(int i=0;i<3*no_tasks;i++){

	// 	printf("%f\t",taskset[i]);
	// }

	for (int i = 0; i < (3*no_tasks); i = i+3){

		if(taskset[i+1] == taskset[i+2]){

			count++;
		}
	}
	// printf("%d\n",count);
	if (count == no_tasks){

	//	printf("Deadline is equal to period for the taskset\n");

		sort_rm(taskset_mat,no_tasks,2);

		for (int i=0;i<no_tasks;i++){
			for(int j=0;j<3;j++){

				taskset[i*3+j]=taskset_mat[i][j];
			}
		}
		int u = utilization_bound(no_tasks ,taskset);
		if (u == 0){
			
			printf("U is less than 1 so sched_rmulable directly\n");
			sched_rm=1;
			non_sched_rm=0;
		}
		else if(u == 1){

			printf("U is greater than 1 so not sched_rmulable\n");
			non_sched_rm=1;
			sched_rm=0;
		}
		else{
			printf("U is greater than utilization_bound and less than 1, so need to do time demand analysis\n");

			for (int nt = 1; nt <= no_tasks; nt++){

				if (utilization_bound(nt , taskset) != 0){

					printf("Task %d utilization results are inconclusive and requires response time analysis\n", nt);

					int td = time_demand(nt, taskset);

					if(td == 0){
						printf("Task %d is schedulable after response time analysis\n\n", nt);
						sched_rm=1;
						non_sched_rm=0;
					}
					else{
						printf("Task %d is not schedulable\n\n", nt);
						non_sched_rm=1;
						sched_rm=0;
					}
				}
				else{

					printf("Task %d is sched_rmulable as U is under bound\n\n", nt );
					sched_rm=1;
					non_sched_rm=0;
				}

			}
		}

	}
	else{
		printf("Here the Deadline < period for the given taskset\n");
		sort_rm(taskset_mat,no_tasks,2);
		if(eff_utilization_bound(taskset_mat,no_tasks)>0){
			sched_rm=1;
			non_sched_rm=0;
		}
		else{
			non_sched_rm=1;
			sched_rm=0;
		}

	}

	 printf("%lu ********************************* %lu\n",sched_rm,non_sched_rm);

	if(sched_rm==1){

		return 1;
	}
	else if(non_sched_rm==1)
		return 0;


}


int eff_utilization_bound(float taskset[][3],int no_tasks){

	float total_uti=0,uti_dead_j=0.0,uti_dead_k=0.0,bound=0.0;
	int hn=0,h1=0;
	float taskset_array[3*no_tasks];

	for (int i=0;i<no_tasks;i++){
			for(int j=0;j<3;j++){

				taskset_array[i*3+j]=taskset[i][j];
			}
		}
	// printf("The array is: ");
	// for (int i = 0; i < (3*no_tasks); ++i){
		
	// 	printf(" %.3f ",taskset_array[i]);
	// }
	for (int i=0;i<no_tasks;i++){
		total_uti=taskset[i][0]/min(taskset[i][1],taskset[i][2]);
		// printf("Total Utilization for task %d\t%f\n",i,total_uti);
		for(int j=0;j<i;j++){

				
				// printf("Total Utilization for task %d %f\n",i,total );
				if((taskset[i][1]>taskset[j][1]) && (taskset[i][1] >= min(taskset[j][2],taskset[j][1]) )){

					uti_dead_j+=taskset[j][0]/min(taskset[j][1],taskset[j][2]);
					hn++;
				}
				else if((taskset[i][1]<taskset[j][1]) && (taskset[i][1] < min(taskset[j][2],taskset[j][1]))){

					uti_dead_k+=taskset[j][0]/min(taskset[i][1],taskset[i][2]);
					h1++;	
				}
			}
		bound=(i+1-h1)*(pow(2.0,(1.0/(i+1-h1))) - 1);
		// printf("No. of tasks in Hn:%d\n",hn );
		// printf("No. of tasks in H1:%d\n",h1 );
		// printf("j Utilization for task %d\t%f\n",i,uti_dead_j );
		// printf("k Utilization for task %d\t%f\n",i,uti_dead_k );
		total_uti+=uti_dead_k+uti_dead_j;
		printf("Total Utilization for task %d\t%f\n",i+1,total_uti);
		printf("Utilization  bound is:%f\n",bound);
		if ((total_uti>bound)&&(total_uti<=1)){
			printf("Requires response time analysis for task %d\n",i+1);
			if(time_demand(no_tasks,taskset_array)==0){
				sched_rm=1;
				non_sched_rm=0;
				printf("Response time analysis for task %d is done and it is schedulable\n",i+1 );
			}
			else{
				non_sched_rm=1;
				sched_rm=0;
				printf("Response time analysis for task %d is done and it is not schedulable\n",i+1 );
			}

			
		}
		else if((total_uti>bound)&&(total_uti>1)){

			non_sched_rm=1;
			sched_rm=0;
//			break;

		}
		total_uti=uti_dead_j=uti_dead_k=0.0;
		hn=h1=0;
		// printf("*********************************************************************************************************\n");
		}


		// if(non_sched_rm!=1)
		// 	sched_rm=1;

 if(non_sched_rm==1)
 	return 0;
 else 
 return 1; 

}



int utilization_bound(int no_tasks, float* taskset){

	float u=0,bound=0;
	int ret;
	for (int i = 0; i < (3*no_tasks); i = i+3){
		u += (taskset[i]/taskset[i+2]);
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
 	
	// printf("a is %.3f\n", prev_a );

	a = taskset[3*no_tasks - 3];

	while(1){

		for (int i = 0; i < 3*(no_tasks - 1); i = i+3){
			
			a += (ceil(prev_a/taskset[i+2]))*taskset[i];
			
		}
		// printf("a is %.3f \n",a );
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

	printf("Max period is %.3f and Work done is %f\n", max_period,a);

	(a <= max_period) ? (ret = 0) : (ret = 1);

	return ret;

}
