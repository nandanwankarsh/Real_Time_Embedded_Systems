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
#include<linux/input.h>
#include<fcntl.h>
#include <ctype.h>
#define EVENT_FILE_NAME "/dev/input/event2"

int num_lines;
int exec_time;

struct node{
    char *data;
    long int pthread_id_l;
    struct node *next;
};

int                 conditionMet = 0;
pthread_cond_t      cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx[10];

struct sched_param param[5],param1;
pthread_attr_t tattr[5],tattr1;

















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
  param1.sched_priority = 99;
  
  int error_status2[4];

  for(int i=0;i<num_lines;i++){
    // temp=head[i]->next;
    // prio=atoi(temp->data);
    printf("Priority is %d\n",*(prio+i));

    param[i].sched_priority=*(prio+i); 
  }

  
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

  pthread_attr_init(&tattr1);
  pthread_attr_setinheritsched(&tattr1,PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&tattr1,SCHED_FIFO);
  pthread_attr_setschedparam(&tattr1, &param1);

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
  printf("Thread %ld Executed\n",pthread_self());
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
  // temp=(struct node*)malloc(sizeof(struct node));
  temp = head;
  int iteration,mutex_no,period;
  for (int i = 0; i < 3; i++){
    if(i==2){
      period = atoi((temp->data));
    }
    temp = temp->next;
  }

  while(temp!=NULL){
    // printf("%s\n", temp->data );
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
      printf("here here\n");
      temp = temp->next; 
    }
    else{
      iteration = atoi(temp->data);
      printf("Iterations are %d\n", iteration );
      compute(iteration);
      temp = temp->next;
    }
  }

}

void compute(int iteration){
  int k, j=0; 
  for (k = 0; k < iteration; k++){
    j = j + k;
  }
}

void aperiodic_body(struct node *head){

}

void *mouse_click(){

  int  fd;
  struct input_event event;
  fd = open(EVENT_FILE_NAME, O_RDONLY);
  if (fd < 0){
      printf("failed to open input device %s: %d\n", EVENT_FILE_NAME, errno);     
  }

  while(read(fd,&event,sizeof(event))){
    //If left or right click then set termination flag
    if(event.code == 272 && event.value == 1)
      printf("Left click event detected\n");
    if(event.code == 272 && event.value == 1)
      printf("Right click event detected\n");
    
  }

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
