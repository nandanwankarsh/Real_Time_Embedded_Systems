#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

int check_utilization(int num_tasks, float* taskset);
double busy_period(int num_tasks, float* taskset);
int edf(float* taskset,int no_tasks);
void calculate_proc_demand(float table[][3],float taskset_ref[],float busy_val,int num_lines,int count_unq);
void merge(float arr[], int l, int m, int r);
void mergeSort(float arr[], int l, int r);
int removeDuplicates(float a[], int array_size);
float uti_value;


int edf(float taskset[],int no_tasks){
	long sched=0,non_sched=0; 
	int count=0;   
	
	printf("\n");
	for (int i = 0; i < (3*no_tasks); i = i+3){

		if(taskset[i+1] == taskset[i+2]){

			count++;
		}
	}

	if (count == no_tasks){

		printf("Deadline = period analysis\n");
		int u = check_utilization(no_tasks ,taskset);
		if (u == 0){
			
			 printf("Utilization is %f so schedulable after Utilization test\n", uti_value);
			sched=1;
			non_sched=0;

		}
		else{
			  printf("Utilization is %f so not schedulable\n", uti_value);
			non_sched=1;
			sched=0;
		}

	}
	else{

		printf("Deadline < period analysis\n");

		int j=0,count=0;
		double t=0;
		int u = check_utilization(no_tasks ,taskset);
		if (u == 0){
			
			 printf("Utilization %f so schedulable after Utilization test\n", uti_value);
			sched=1;
			non_sched=0;
		}
		else{
			 printf("Utilization %f so need to do loading factor analysis\n", uti_value);

			double L = busy_period(no_tasks ,taskset);

			  printf("The busy_period is %lf \n", L);        

			while(t < L){
				for (int i = 0 ; i < (3*no_tasks); i = i+3){



					if(t >= L)
						break;
					else{
						t = taskset[i+1] + j*taskset[i+2];
						   
						count ++;
					}
				}

				j++;
			} 
			
			float t_array[count-1];
			j=count=t=0;


			while(t < L){
				for (int i = 0; i < (3*no_tasks); i = i+3){
					

					if(t >= L)
						break;
					else{
						t= taskset[i+1] + j*taskset[i+2];
						// printf("Value of t is %lf\n", t);
						
						if(t<=L){
							count ++;
							t_array[count-1]=t;
						}
					}
				}

				j++;
			} 

			mergeSort(t_array,0,count-1);

			int count_unq=0;
			count_unq=removeDuplicates(t_array,count);
			float load_mat[count_unq][3];

			for(int i=0;i<=count_unq;i++){

				load_mat[i][0]=t_array[i];
			}
			

			calculate_proc_demand(load_mat,taskset,t,no_tasks,count_unq);

			printf("    t\t    |\t   h\t    |\t   u\n");

			for(int i=0;i<count_unq;i++){

				for(int j=0;j<3;j++){

					printf("%f\t",load_mat[i][j] );
				}
				printf("\n");
			}

			int k=0,l=0;

			for(int i=0;i<count_unq;i++){

				if(load_mat[i][2]>1){

					k = 1;

					if ((k==1)&&(l==0)){
						printf("\nThe first missing Deadline is %f \n", load_mat[i][0]);

						l = 1; 

					}


					non_sched=1;
					sched=0;
					
				}

			}

			 if(sched==1){

				printf("Task is Schedulable after Loading factor analysis\n");
	
			}
			else if(non_sched==1){
				printf("Task is Non Schedulable after loading factor analysis\n");
		
			}

		}

	}

	if(sched==1){

			return 1;
		}
		else{

			return 0;
		}

}
int removeDuplicates(float a[], int array_size)
 {
   int i, j;
 
   j = 0;
 
   // Remove the duplicates ...
   for (i = 1; i < array_size; i++)
   {
     if (a[i] != a[j])
     {
       j++;
       a[j] = a[i]; // Move it to the front
     }
   }
 
   // The new array size..
   array_size = (j + 1);
 
   // Return the new size...
   return(j + 1);
 }


void calculate_proc_demand(float table[][3],float taskset_ref[],float busy_val,int num_lines,int count_unq){

	float sum_dead;
	int index=0;

	while(index<count_unq && table[index][0]<=busy_val){

		for(int i=0;i<num_lines;i++){

			if(taskset_ref[3*i+1]<=table[index][0]){

				sum_dead+=taskset_ref[3*i];
				taskset_ref[3*i+1]=taskset_ref[3*i+1]+taskset_ref[3*i+2]	;			
			}
		}

		table[index][1]=sum_dead;
		table[index][2]=table[index][1]/table[index][0];

		if (table[index][0]>busy_val)
			break;
		index++;

	}

}



double busy_period(int num_tasks, float* taskset){

	double prev_L=0, L=0;

	for (int i = 0; i < 3*(num_tasks); i = i+3){
		
		prev_L += taskset[i];
	}

	while(1){

		for (int i = 0; i < 3*(num_tasks); i = i+3){
			
			L += (ceil(prev_L/taskset[i+2]))*taskset[i];
			
		}
	
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

	uti_value = u;

	if (u <= 1){
		ret = 0;
	}
	else{
		ret = 1;
	}

	return ret;
}

void merge(float arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(float arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
 
        merge(arr, l, m, r);
    }
}
