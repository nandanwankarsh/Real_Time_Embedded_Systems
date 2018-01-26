// #include <stdio.h>
// #include <string.h>

// int main ()
// {
//   char str[] ="P 10 200 200 L2 100 L3 500 U3 100 L4 60 U4 200 U2 200";
//   char * pch;
//   printf ("Splitting string \"%s\" into tokens:\n",str);
//   pch = strtok (str," ");
//   while (pch != NULL)
//   {
//     printf ("%s\n",pch);
//     pch = strtok (NULL, " ");
//   }
//   return 0;
// }

#include <stdio.h>
#include <stdlib.h>


int num_lines;
int exec_time;

struct node{
    char *data;
    struct node *next;
}*head[5];

int                 conditionMet = 0;
pthread_cond_t      cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;

struct sched_param param[5];
pthread_attr_t tattr[5];

















// **************************************************************************************************************************************************
void create(int count,char *s);
void print(int count);
void set_priority();
void wait_cond_var(int *i);
void broad_cond_var();
void *threadfunc(void *parm);

// int main()
// {
//     int i,n;
//     n=5;  /*n is the total number of nodes */
//     for(i=0;i<n;i++)
//     {
//          head[i]=NULL;
//          create(i);
//          print(i);
//          printf("\n\n");
//      }
//     return 0;
// }
void set_priority(){


	struct node *temp;
	temp=(struct node*)malloc(sizeof(struct node));
	int prio;
	
/*	param[0].sched_priority = 110; 
	param[1].sched_priority = 99; // " "
	param[2].sched_priority = 0; // " "
	param[3].sched_priority = 78; // " "
	param[4].sched_priority = 65;
*/


	for(int i=0;i<num_lines;i++){
		temp=head[i]->next;
		prio=atoi(temp->data);
		printf("Priority is %d\n",prio);
		param[i].sched_priority=prio;	

	}
	int error_status2[4];


	for(int j=0;j<num_lines;j++){
	//thread attribute creation
		error_status2[j] = pthread_attr_init(&tattr[j]);
		if(error_status2[j] != 0){
			printf("attr_init error %d= %d\n",j,error_status2[j]);
		}

	//Thread attribute initialization
		error_status2[j] = pthread_attr_setinheritsched(&tattr[j],PTHREAD_EXPLICIT_SCHED);
		if(error_status2[j] != 0){
			printf("thread %d setinherit_sched error = %d\n",j,error_status2[j]);
		}

	//Thread policy initialization
		error_status2[j] = pthread_attr_setschedpolicy(&tattr[j],SCHED_FIFO);
		if(error_status2[j] != 0 ){
			printf("thread %d setschedpolicy error = %d\n",j,error_status2[j]);
		}

	//Thread priority initialization
		error_status2[j] = pthread_attr_setschedparam(&tattr[j], &param[j]);
		if(error_status2[j] != 0){
			printf("thread %d setschedparam error = %d\n", j,error_status2[j]);
		} 
	}
}
void wait_cond_var(int *i){

	pthread_mutex_lock(&mutex);
	int policy;
	printf("Thread idex %d\n",*i);
	while (!conditionMet) {
		printf("Thread %ld blocked\n",pthread_self());
		pthread_getschedparam(pthread_self(), &policy, &param[*i]);
		pthread_cond_wait(&cond, &mutex);
	}
	printf("Thread %ld Executed\n",pthread_self());
	pthread_mutex_unlock(&mutex);

}

void broad_cond_var(){

	pthread_mutex_lock(&mutex);
	conditionMet = 1;
	printf("Wake up all waiting threads...\n");
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
	printf("Wait for threads and cleanup\n");
}

void *threadfunc(void *parm){
	int *i;
	i=(int *)parm;
	wait_cond_var(i);
	// if(*(head[*i].data)=='A')
	//   aperiodic_body();
	// else
	//   periodic_body();

	return NULL;
}

void create(int count,char *s)
{
//      int n2=5;  /*n2 is the number of nodes in a single linked list*/
//      char check;
      char *token;
      printf ("Splitting string \"%s\" into tokens:\n",s);    
      token = strtok (s," "); 
      struct node *temp;
      while(token != NULL)
      {
              
            printf ("%s\n",token);
            
            if(head[count]==NULL)
            {
               temp=(struct node*)malloc(sizeof(struct node));
               temp->data=token;
               temp->next=NULL;
               head[count]=temp;
            }
            else
            {
              temp->next=(struct node*)malloc(sizeof(struct node));
               temp=temp->next;
               temp->data=token;
               temp->next=NULL;
            }

            token = strtok (NULL, " ");
     }
}

void print(int count)
{
     struct node *temp;
     temp=head[count];
     while(temp!=NULL)
     {
          printf("%s->",temp->data);
          temp=temp->next;
     }
}
