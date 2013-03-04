
 /*
 ���Ŵ��㷨��y=x*sin(10*pi*x)+2�����ֵ  -1=<x<=2
 ��ȷ��6λС��
 pow(2,21)<3*1000000<pow(2,22)
 ����Ķ����Ƴ���Ϊ22 
 */
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <ctime>
 #include <math.h>
 
 #define N 3000000
 #define PI 3.14159265
 #define MAX(a,b) ((a)>(b)?(a):(b))
 
 #define SIZE  50
 #define MAXGEN  50
 #define P_CORSS 0.75
 #define P_MUTATION 0.05
 
 #define LEN 22
 
 typedef struct node
 {
   char x[LEN];
   double fitness,fitsum;
 }node;
 
 node cur[SIZE],next[SIZE],max,min;
 
 double randd()
 {
   return (double)rand()/RAND_MAX;
 }
 int randi(int k)
 {
   return (int)(randd()*k+0.5);
 }
 
 //���㵱ǰ��Ⱥ�и����������Ӧ�� 
 void cal_fitness()
 {
   int i,j,k;
   double d;
   for(i=0;i<SIZE;i++)
   {
     k=0;
     for(j=LEN-1;j>=0;j--) k=(k<<1)+cur[i].x[j];
     d=(double)k/N*3-1;
     cur[i].fitness=d*sin(10*PI*d)+2;
     cur[i].fitsum=i>0?(cur[i].fitness+cur[i-1].fitsum):(cur[0].fitness);
   }
 }
 
 void init()
 {
   int tmp;
   for(int i=0;i<SIZE;i++)
   {
     tmp=randi(N);
     for(int j=0;j<LEN;j++)
     {
       cur[i].x[j]=tmp%2;
       tmp=tmp>>1;
     }
   }
   cal_fitness();
 }
 
 int sel()
 {
   double p=randd();
   double sum=cur[SIZE-1].fitsum;
   for(int i=0;i<SIZE;i++)
   {
     if(cur[i].fitsum/sum>p) return i;
   }
 }
 
 //���� 
 void tran()
 {
   int i,j,pos;
   //�ҵ�ǰ��Ⱥ���Ÿ��� 
   max=cur[0];
   for(i=1;i<SIZE-1;i++)
   {
     if(cur[i].fitness>max.fitness)  max=cur[i];
   }
   for(int k=0;k<SIZE;k+=2)
   {
     //ѡ�񽻲���� 
     i=sel();
     j=sel();
     
     //ѡ�񽻲�λ�� 
     pos=randi(LEN-1);
     
     //����
     if(randd()<P_CORSS)
     {
       memcpy(next[k].x,cur[i].x,pos);
       memcpy(next[k].x+pos,cur[j].x+pos,LEN-pos);
 
       memcpy(next[k+1].x,cur[j].x,pos);
       memcpy(next[k+1].x+pos,cur[i].x+pos,LEN-pos);
     }
     else
     {
       memcpy(next[k].x,cur[i].x,LEN);
       memcpy(next[k+1].x,cur[j].x,LEN);
     }
     //����
     if(randd()<P_MUTATION)
     {
       pos=randi(LEN-1);
       next[k].x[pos]^=next[k].x[pos];
 
       pos=randi(LEN-1);
       next[k+1].x[pos]^=next[k+1].x[pos];
     }
   }
   //����һ���������� 
   min=next[0],j=0;
   for(i=1;i<SIZE-1;i++)
   {
     if(next[i].fitness<min.fitness)  min=next[i],j=i;
   }
   //����һ�������Ÿ����滻��һ����������
   next[j]=max;
   
   memcpy(cur,next,sizeof(cur));
    
   
   cal_fitness();
 }
 
 //��ӡ������Ӧ�ȺͶ����Ʊ��� 
 void print(node tmp)
 {
   printf("%.6lf",tmp.fitness);
   for(int i=0;i<LEN;i++)  printf(" %d",tmp.x[i]);
   printf("\n");
 }
 
 //��ӡ��Ⱥ
 void printcur()
 {
   for(int i=0;i<SIZE;i++) print(cur[i]);
 } 
 
 
 void GA()
 {
   int cnt=0;
   double ans;
   while(cnt++<MAXGEN)
   {
     tran();
     
     printf("%.6lf\n",max.fitness);
     printcur();
   }
   ans=cur[0].fitness;
   for(int i=1;i<SIZE;i++) ans=MAX(ans,cur[i].fitness);
   printf("%.6lf\n",ans);
 }
 
 int main()
 {
   srand((unsigned)time(NULL));
   
   init();
   GA();
   
   //system("pause");
   return 0;
 }
