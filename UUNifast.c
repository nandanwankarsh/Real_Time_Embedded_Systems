#define _GNU_SOURCE
#include <stdio.h>

#include <stdlib.h>
#include <math.h>

void UUnifast(int n, float U, float* unifst);

int main(){

	float u=0;
	float array[10] = {0.05, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.95};
	for (int j = 0; j < 10; j++){

		float sum = 0;	

		int i = rand()%(9 + 1 - 0) + 0;
		u = array[i];
		printf("For U == %.2f \n", u);
		

		float unifst[10];

		UUnifast(10,u, unifst);

		for (int i = 0; i < 10; i++){
			
			printf("%.3f \t", unifst[i]);
			sum += unifst[i];

		}
		printf("Sum is ======= %.2f\n", sum);

		printf("\n\n");

	}	
	return 0;
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