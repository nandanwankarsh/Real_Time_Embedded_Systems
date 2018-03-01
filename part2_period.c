#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int M=3;
float range=10;
int taskset_len=100;

void period_range(float* range_period);
// float period_range2();
// float period_range3();

int main(){

float range[taskset_len];
period_range(range);
return 0;

}

void period_range(float* range_period){

	for(int i=0;i<taskset_len/M;i++){
		float random=(float)rand();
		range_period[i]=(fmodf(random,(pow(range,2)+1.0-pow(range,1)))+pow(range,1));
		printf("%f \t %d\n",range_period[i],i);
	}

	for(int i=taskset_len/M;i<taskset_len*2/M;i++){
		float random=(float)rand();
		range_period[i]=(fmodf(random,(pow(range,3)+1.0-pow(range,2)))+pow(range,2));
		printf("%f \t %d\n",range_period[i],i);
	}

	for(int i=taskset_len*2/M;i<(taskset_len);i++){
		float random=(float)rand();
		range_period[i]=(fmodf(random,(pow(range,4)+1.0-pow(range,3)))+pow(range,3));
		printf("%f \t %d\n",range_period[i],i);
	}

}