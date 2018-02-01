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
#include <linux/input.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>
#include <semaphore.h>
#include <sys/types.h>
#define EVENT_FILE_NAME "/dev/input/event18"

int num_lines;
int exec_time;

struct node{
    char *data;
    long int pthread_id_l;
    struct node *next;
};

int                 fd,conditionMet = 0, event0=0,event1=0,termination_flag,event0_count,event1_count;
pthread_cond_t      cond,ap_cond_0,ap_cond_1;
//pthread_cond_t      ap_cond_0  = PTHREAD_COND_INITIALIZER;
//pthread_cond_t      ap_cond_1  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t     ap_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t     mtx[10];

struct sched_param param[5],param1,param2;
pthread_attr_t tattr[5],tattr1,tattr2;
sem_t sem0,sem1;






// **************************************************************************************************************************************************
struct node* create(char *s);
void print(struct node* head);
void set_priority(int* prio);
void wait_cond_var();
void broad_cond_var();
void *threadfunc(void *parm);
void periodic_body();
void compute(int iteration);
void aperiodic_body(struct node *head); 
void *mouse_click();
void *termination();
void join();
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
void set_priority(int* prio){

  // struct node *temp;
  // temp=(struct node*)malloc(sizeof(struct node));
  // int prio;

  // param[0].sched_priority = 87; 
  // param[1].sched_priority = 90; 
  // param[2].sched_priority = 71; 
  // param[3].sched_priority = 78; 
  // param[4].sched_priority = 65;
  param1.sched_priority = 95;
  param2.sched_priority = 96;

  int error_status2[num_lines];

  for(int i=0;i<num_lines;i++){
    // temp=head[i]->next;
    // prio=atoi(temp->data);
    printf("Priority is %d\n",*(prio+i));

    param[i].sched_priority=*(prio+i); 
  }
  pthread_attr_init(&tattr1);
  pthread_attr_setinheritsched(&tattr1,PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&tattr1,SCHED_FIFO);
  pthread_attr_setschedparam(&tattr1, &param1);

  pthread_attr_init(&tattr2);
  pthread_attr_setinheritsched(&tattr2,PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&tattr2,SCHED_FIFO);
  pthread_attr_setschedparam(&tattr2, &param2);

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
void wait_cond_var(){

  pthread_mutex_lock(&mutex);
  // int policy;
  // printf("Thread idex %d\n",*i);
  while (!conditionMet) {
  printf("Thread %ld blocked\n",pthread_self());
  //pthread_getschedparam(pthread_self(), &policy, &param[*i]);
  pthread_cond_wait(&cond, &mutex);
  }
  //printf("Thread %ld Executed\n",pthread_self());
  pthread_mutex_unlock(&mutex);

}

void broad_cond_var(){

  pthread_mutex_lock(&mutex);
  conditionMet = 1;
  printf("Wake up all waiting threads...\n");
  pthread_cond_broadcast(&cond);
  pthread_mutex_unlock(&mutex);
//  printf("Wait for threads and cleanup\n");
}

void *threadfunc(void *parm){
  struct node *head;
  head=(struct node *)parm;
  wait_cond_var();
  head->pthread_id_l = pthread_self();

  //printf("%lu\n", head->pthread_id_l);  

  // while(1){
  //   int k, j=0; 
  //   for (k = 0; k < 250000; k++){
  //     j = j + k;
  //   }
  //   printf("Thread %ld Executed  with id in Linked list %lu \n",pthread_self(),head->pthread_id_l);
  //   sleep(1);
  // }

  if(*(head->data)=='A')
  aperiodic_body(head);
    else
  periodic_body(head);
  
  return NULL;
}

 void periodic_body(struct node *head){

  struct node* temp;
  struct timespec next_time,period_time;
  // temp=(struct node*)malloc(sizeof(struct node));
  temp = head;
  int iteration,mutex_no,period;
  for (int i = 0; i < 3; i++){
  if(i==2){
      period = atoi((temp->data));
    }
  temp = temp->next;
  }

  // printf("Period is %d \n", period );

  clock_gettime(CLOCK_MONOTONIC, &next_time);

  period_time.tv_nsec = period*1000000;

  // printf("The period is %lu \n", period_time.tv_nsec);

  while(termination_flag==0){

    while((temp!=NULL)){
    // printf("%s\n", temp->data);
      if (isalpha(*(temp->data))){
        if (*(temp->data)=='L'){
            mutex_no = atoi((temp->data+1));
            printf("Mutex %d locked\n",mutex_no );
            pthread_mutex_lock(&mtx[mutex_no]);
        }
        else{
            mutex_no = atoi((temp->data+1));
            printf("Mutex %d unlocked\n",mutex_no );
            pthread_mutex_unlock(&mtx[mutex_no]);
        }
        //printf("%s\n", (temp->data));
        // printf("here here\n");
          temp = temp->next; 
        }
      else{
        iteration = atoi(temp->data);
        printf("Iterations are %d\n", iteration );
        compute(iteration);
        // if(termination_flag==1){
        //   break;
        // }  
        temp = temp->next;
      }
    }

    printf("Task body done for %lu \n", pthread_self() );
    temp = head->next->next->next;

    // Implementing periodicity
    if((next_time.tv_nsec+period_time.tv_nsec)>=1000000000){
      next_time.tv_nsec = (next_time.tv_nsec+period_time.tv_nsec)%1000000000;
      next_time.tv_sec++;

    }
    else{   
      next_time.tv_nsec = next_time.tv_nsec+period_time.tv_nsec;
    }


    printf("The next time for thread id %lu is %lu \n", pthread_self(), next_time.tv_nsec);
    if (termination_flag==0)
    {
      clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_time, 0);
    }
    
  }

  free(temp);
  return;

}

void compute(int iteration){
  int k, j=0;
  for (k = 0; k < iteration; k++){
    j = j + k;
    if (termination_flag==1){
      join();  
    }
  }
}

void join(){
  for (int i=0; i<num_lines; i++){
    if (!(pthread_equal(threadid[i],pthread_self()))){
      pthread_join(threadid[i], NULL);
    }  
  }

}

void aperiodic_body(struct node *head){

  
    
  printf("In aperiodic body\n");
//  atoi(head->next->next->data)==0?sem_wait(&sem0):sem_wait(&sem1);
  while(termination_flag==0){
  
    if(atoi(head->next->next->data)==0){
      pthread_mutex_lock(&ap_mutex);
      //while (!event0 && event0_count<3) {
            printf("Aperiodic Thread %ld blocked for event %d\n",pthread_self(),atoi(head->next->next->data));
            if(pthread_cond_wait(&ap_cond_0, &ap_mutex)<0)
          printf("ap_cond_0 error\n");
      //}
      //sleep(0.5);
      event0_count++;
      event0=0;
      printf("******Thread %ld Executed\n",pthread_self());
        pthread_mutex_unlock(&ap_mutex);
      }
    else if(atoi(head->next->next->data)==1){
      pthread_mutex_lock(&ap_mutex);
      //while (!event1 && event1_count<3) {
            printf("Aperiodic Thread %ld blocked for event %d\n",pthread_self(),atoi(head->next->next->data));
            if(pthread_cond_wait(&ap_cond_1, &ap_mutex)<0)
          printf("ap_cond_1 error\n");
        
      //}
      //sleep(0.5);
      event1_count++;
      event1=0;
      printf("*****Thread %ld Executed\n",pthread_self());
        pthread_mutex_unlock(&ap_mutex);
    }

    }

    // while((event0 == 1)||(event1 == 1)){

    //   printf("HERE HERE\n");

    //   if (event0==1){
    //     printf("In event 0\n");
    //     event0 = 0;
    //   }
    //   if (event1==1){
    //     printf("In event 1\n");
    //     event1 = 0;
    //   }

    // }
    printf("Event detected %d for thread %lu*********************************************************************************\n",atoi(head->next->next->data),pthread_self());
    printf("End loop\n");

}

void *mouse_click(){

  // int  fd;
  struct input_event event;
  fd = open(EVENT_FILE_NAME, O_RDONLY);
  if (fd < 0){
  printf("failed to open input device %s: %d\n", EVENT_FILE_NAME, errno);     
  }

  while((termination_flag==0) && (read(fd,&event,sizeof(event)))){
  //If left or right click then set termination flag
  if(event.code == 272 && event.value == 0 && conditionMet>0){
    printf("Left click event detected\n\n");
    //    sem_post(&sem0);
    pthread_mutex_lock(&ap_mutex);
    event0 = 1;
    printf("Wake up all left click waiting threads...\n");
    pthread_cond_broadcast(&ap_cond_0);
    pthread_mutex_unlock(&ap_mutex);
  //  sleep(0.5);
    // break;
  }
  if(event.code == 273 && event.value == 0 && conditionMet >0){
    printf("Right click event detected\n\n");
  //sem_post(&sem1);
    pthread_mutex_lock(&ap_mutex);
    event1 = 1;
    printf("Wake up all right click waiting threads...\n");
    pthread_cond_broadcast(&ap_cond_1);
    pthread_mutex_unlock(&ap_mutex);
  //  sleep(0.5);
  // break;
  }


  }

  printf("End loop\n");

  return NULL;
}

void *termination(){
  struct timespec now,terminate;
  clock_gettime(CLOCK_MONOTONIC, &now);

  terminate.tv_nsec = exec_time*1000000;
  now.tv_nsec = now.tv_nsec+terminate.tv_nsec;
  clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &now, 0);
  termination_flag = 1;

  return NULL;

}

struct node* create(char *s)
{
//      int n2=5;  /*n2 is the number of nodes in a single linked list*/
//      char check;
      char *token;
    //  printf ("Splitting string \"%s\" into tokens:\n",s);    
      token = strtok (s," "); 
      //printf("Thread data %lu\n",head->pthread_id_l);
      struct node *temp;
      struct node *head;
      head=NULL;
      temp = (struct node*)malloc(sizeof(struct node));
      while(token != NULL)
      {
              
            // printf ("%s\n",token);
            
            if(head==NULL)
            {
               temp=(struct node*)malloc(sizeof(struct node));
               temp->data=token;
               temp->next=NULL;
               head=temp;
               printf("%s\n", head->data);
            }
            else
            {
              temp->next=(struct node*)malloc(sizeof(struct node));
               temp=temp->next;
               temp->data=token;
               printf("%s\n", temp->data);
               temp->next=NULL;
            }

            token = strtok (NULL, " ");
     }

     printf("Creation complete\n");

     return head;
}

void print(struct node* head)
{
     struct node *temp;
     static int i=1;
     temp = (struct node*)malloc(sizeof(struct node));
     temp=head;
     // printf("%s\n", head->data);
     while(temp!=NULL)
     {
         // printf("in while\n");
          printf("%s->",temp->data);
          temp=temp->next;
     }
     printf("Linked List %d is completed\n",i);
     i++;
}
