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
struct node{
    char data[10];
    struct node *next;
}*head[5];
void create(int count);
void print(int count);
int main()
{
    int i,n;
    n=5;  /*n is the total number of nodes */
    for(i=0;i<n;i++)
    {
         head[i]=NULL;
         create(i);
         print(i);
         printf("\n\n");
     }
    return 0;
}
void create(int count)
{
      int n2=5;  /*n2 is the number of nodes in a single linked list*/
      int j;
      struct node *temp;
      for(j=0;j<5;j++)
      {
             if(head[count]==NULL)
             {
                 temp=(struct node*)malloc(sizeof(struct node));
                 temp->data=j+5+count;
                 temp->next=NULL;
                 head[count]=temp;
             }
             else
             {
                temp->next=(struct node*)malloc(sizeof(struct node));
                 temp=temp->next;
                 temp->data=j+5+count;
                 temp->next=NULL;
             }
     }
}
void print(int count)
{
     struct node *temp;
     temp=head[count];
     while(temp!=NULL)
     {
          printf("%d->",temp->data);
          temp=temp->next;
     }
}