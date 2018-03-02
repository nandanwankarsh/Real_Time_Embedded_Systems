#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LEN 256

FILE * fp;
int M=3;
float range=10;
int task_len=10;
int taskset_len=5000;

void UUnifast(int n, float U, float* unifst);
void period_range(float range_period[][3]);
void write_file(int task_len, float syn_taskset[][3]);

int main(){

	float u=0,uti_syn[task_len];
	float syn_taskset[task_len][3];
	fp = fopen ("synthetic_input.txt","w");
	fclose (fp);
	fp = fopen ("synthetic_input.txt","a+");
	fprintf (fp, "%d\n",taskset_len);
	// float wcet_syn[task_len],syn_taskset[i][2][task_len],dead_syn[task_len],uti_syn[task_len];
	float array[10] = {0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95};
	for (int j = 0; j < taskset_len; j++){

		printf("-------------------  Taskset %d -----------------\n",j);

		float sum = 0;	

		int i = rand()%(9 + 1 - 0) + 0;
		u = array[i];
		// printf("For U == %.2f \n", u);

		float unifst[task_len];

		UUnifast(task_len,u, unifst);

		// for (int i = 0; i < task_len; i++){
			
		// 	printf("%.3f \t", unifst[i]);
		// 	sum += unifst[i];

		// }

		// uti_syn[j]=sum;
		// printf("Sum is ======= %.2f\n", sum);

		period_range(syn_taskset);
		// printf("WCET obtained is \n");
		// printf("\n");
		for(int i=0;i<task_len;i++){
			syn_taskset[i][0]=syn_taskset[i][2]*unifst[i];
			// printf("WCET of %d is ",i );
			// printf("%f\t",syn_taskset[i][0]);
		}

		// printf("\n");

		for(int i=0;i<task_len;i++){
			syn_taskset[i][1]=(fmodf((float)rand(),(syn_taskset[i][0]+((syn_taskset[i][2]-syn_taskset[i][0])/2.0)+1.0-syn_taskset[i][0])+syn_taskset[i][0]));

			if(syn_taskset[i][1]>syn_taskset[i][0]){
				// printf("%f\t",syn_taskset[i][1]);
			}
			else 
				i--;
		}



		for (int i = 0; i < task_len; i++){
			// printf("Task %d ",i);
			printf("        \tWCET\t\tDline\t\tPeriod\n");
			printf("Task %d \t",i);
			printf("\t%f\t%f\t%f\n",syn_taskset[i][0],syn_taskset[i][1],syn_taskset[i][2]);
		}

		write_file(task_len, syn_taskset);

		printf("\n");

		// printf("\n\n");
	}
	

	
	// write_file(syn_taskset);

	/* close the file*/  
   fclose (fp);


	return 0;
}

void period_range(float range_period[][3]){

	for(int i=0;i<task_len/M;i++){
		// float random=(float)rand();
		range_period[i][2]=(fmodf((float)rand(),(pow(range,3)+1.0-pow(range,2)))+pow(range,1));
		// printf("period %d in range 1 %f\t",i,range_period[i][2]);
	}

	for(int i=task_len/M;i<task_len*2/M;i++){
		// float random=(float)rand();
		range_period[i][2]=(fmodf((float)rand(),(pow(range,4)+1.0-pow(range,3)))+pow(range,2));
		// printf("period %d in range 2 %f\t",i,range_period[i][2]);
	}

	for(int i=task_len*2/M;i<(task_len);i++){
		// float random=(float)rand();
		range_period[i][2]=(fmodf((float)rand(),(pow(range,5)+1.0-pow(range,4)))+pow(range,3));
		// printf("period %d in range 3%f\t",i,range_period[i][2]);
	}

}

void UUnifast(int n,float U, float* unifst){

float sumU = U,nextSumU=0,vectU=0;
// float vect_array[n];
for (int i = 0; i < (n-1); i++){

	int a = rand()%(100 + 1 - 10) + 10;
 	nextSumU = sumU*pow((a/100.0),(1.0/( n - i)));
 	vectU = sumU - nextSumU;
 	if (vectU <= 0.0009){
 		i--;
 	}
 	else{
 		unifst[i] = vectU;
	 	sumU = nextSumU;	
	}
} 

unifst[n-1] = sumU;

 return;

}

void write_file(int task_len, float syn_taskset[][3]){
   int i;

   fprintf (fp, "%d\n",task_len);

   for (int i = 0; i < task_len; i++){
			
   		fprintf (fp, "%f %f %f\n",syn_taskset[i][0],syn_taskset[i][1],syn_taskset[i][2]);

		}
 
   return 0;
}