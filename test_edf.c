#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

int num_tasks = 3;

int check_utilization(int num_tasks, float* taskset);
int busy_period(int num_tasks, float* taskset);
void del(char str[], char ch);
void edf(char* taskset,int no_tasks);
void calculate_proc_demand(float table[][3],float taskset_ref[],float busy_val,int num_lines);

void main(){

	FILE *fp;
	char line1[30];
	char *line1_str[2],*str_woeol;
	int num_lines,num_taskset;

	if ((fp=fopen("input.txt","r")) == NULL)
		printf("Cannot open file\n");

	fgets(line1, sizeof(line1), fp);
	del(line1,'\n');
	printf ("Splitting string \"%s\" into tokens:\n",line1);
	num_taskset=atoi(line1);
	// printf("No of taskset %d\n",num_taskset);
	for(int j = 0; j < num_taskset; j++){
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
		edf(holder,num_lines);
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

void edf(char* str_taskset,int no_tasks){
	
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

		int t=0,j=0,count=0;
		int u = check_utilization(num_tasks ,taskset);
		if (u == 0){
			
			printf("U is less than 1 so schedulable\n");
		}
		else{
			printf("U is greater than 1 so need to do loading factor analysis\n");

			int L = busy_period(num_tasks ,taskset);

			 printf("The busy_period is %d \n", L);

			while(t < L){
				int i = 0;
				for (i ; i < (3*num_tasks); i = i+3){
					

					if(t >= L)
						break;
					else{
						t = taskset[i+1] + j*taskset[i+2];
						  printf("Value of t is %d\n", t);
						// t[count]=t;
						count ++;
					}
				}

				j++;
			} 
			printf("here here\n");
			float load_mat[count-1][3];
			j=count=t=0;


			while(t < L){
				int i = 0;
				for (i ; i < (3*num_tasks); i = i+3){
					

					if(t >= L)
						break;
					else{
						t = taskset[i+1] + j*taskset[i+2];
						 printf("Value of t is %d\n", t);
						
						count ++;
						load_mat[count-1][0]=t;
					}
				}

				j++;
			} 

			calculate_proc_demand(load_mat,taskset,t,no_tasks);
			// for(int i=0;i<count-1;i++){

			// 	for(int j=0;j<3;j++){

			// 		printf("%f\t",load_mat[i][j] );
			// 	}
			// 	printf("\n");
			// }
			 // printf("Value of t's count is %d\n", count);

		}
	}

		


}


void calculate_proc_demand(float table[][3],float taskset_ref[],float busy_val,int num_lines){

	float sum_dead;
	int index=0;
	// printf("Busy Val is %f\n", busy_val);
	while(table[index][0]<=busy_val){

		for(int i=0;i<num_lines;i++){

			if(taskset_ref[3*i+1]<=table[index][0]){

				sum_dead+=taskset_ref[3*i];
				taskset_ref[3*i+1]=taskset_ref[3*i+1]+taskset_ref[3*i+2]	;			
			}
		}

		table[index][1]=sum_dead;
		table[index][2]=table[index][1]/table[index][0];
		// printf("Index is %f\t%f\n", table[index][0],table[index][1]);
		// printf("Index is %f\t %d\n", sum_dead,index);
		if (table[index][0]==busy_val)
			break;
		index++;

	}
	// printf("Index is %d\n", index);

	for(int i=0;i<=index;i++){

				for(int j=0;j<3;j++){

					printf("%f\t",table[i][j] );
				}
				printf("\n");
			}
			  // printf("Value of t's count is %d\n", count);






}



int busy_period(int num_tasks, float* taskset){

	int prev_L=0, L=0;

	for (int i = 0; i < 3*(num_tasks); i = i+3){
		
		prev_L += taskset[i];
	}
	// printf("prev_L is %d \n",prev_L );
	
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

