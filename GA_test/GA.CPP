//copyright by zhangpeng ,May 5,2007

#include <stdlib.h>
#include <time.h>
//#include <fstream.h>
#include <math.h>

#include <iostream>
using namespace std;



//-------------------------------------------
//������0��1�����ȷֲ��������
double randnum()
{
	double x;
	x = (double) rand()/RAND_MAX;
	return x;
}

//-------------------------------------------
//Ʒ�ʺ���
inline double quanlityfun(double x)
{
	double y;
	y=-(2*pow(x,2)-x-1);
//	y=-pow((x-1),2)+4;
//	y=fabs(sin(3.1415926*(x-3))/3.1415926/(x-3));
	return y;
}

//-------------------------------------------
//�Ŵ��㷨
double GA(double x1,double x2)
{
	const int M=40;//Ⱥ���С��ָȺ������ĸ�����Ŀ��һ��ȡ(20-100)
	const int N=11;//Ⱦɫ�����Ԫ��
	const int Generation=200;//�Ŵ��㷨����ֹ����������һ��ȡ(100-500)
	double probability_cross=0.7;//������ʣ�һ��ȡ(0.4-0.99)
	double probability_mutation=0.1;//������ʣ�һ��ȡ(0.0001,0.1)
	double precision=(x2-x1)/(pow(2,N)-1);//�����Ʊ���ľ���

	//----------------------------------------------------------------
	//��������㷨����ʼ����Ⱥ
	bool pop[M][N];//������Ⱥ����
	bool best_individual[N];//����һ���е����Ÿ���
	for(int i=0;i<M;i++)
	{	for(int j=0;j<N;j++)
		{
			if(randnum()>0.5)
				pop[i][j]=1;
			else
				pop[i][j]=0;				
		}		
	}
	//�Ŵ��㷨�ĺ��ĵ���
	for(int iter=0;iter<Generation;iter++)
	{
		//------------------------------------------------------------
		//ѡ�񡣼���Ⱥ����ÿһ�������Ӧ��
		double adapt[M];//����Ⱥ�����Ӧ�ȱ���
		for(int i=0;i<M;i++)//��Ӧ�ȱ�������ֵ0
			adapt[i]=0;
		for(i=0;i<M;i++)//����ֵ�Ľ���(���������Ϊʮ������)
		{	for(int j=0;j<N;j++)
			{
				if(pop[i][j]==1)
					adapt[i]=adapt[i]+pow(2,(N-1-j));
			}
			adapt[i]=adapt[i]*precision+x1;//�����Ʊ�������Ӧ��ʵ�ʵ�Ʒ�ʺ�������ֵ
			adapt[i]=quanlityfun(adapt[i]);//����Ⱥ���Ʒ�ʺ���			
		}

		//��Ӣ����������Ʒ����õ�һ�����壬��ѡ���컯��ֱ�ӽ�����һ���Ŵ�
		double adapt_best=0;//��¼��õĸ���
		int best_pos=0;//��¼��ø����λ��
		for(i=0;i<M;i++)
		{
			if(adapt_best<adapt[i])
			{adapt_best=adapt[i];best_pos=i;}
		}

		//�������Ÿ���
		for(i=0;i<N;i++)
			best_individual[i]=pop[best_pos][i];
		
		//�������̶ķ�ʽѡ�����
		double ada_sum=0;//��¼�ܵ���Ӧֵ(Ʒ������)
		double ada_temp[M];//��¼�ۼӵ���Ӧֵ(Ʒ������)
		for(i=0;i<M;i++)//��Ӧֵ����
		{
			ada_sum=ada_sum+adapt[i];
			ada_temp[i]=ada_sum;
		}
	
		bool temp_pop[M][N];//��ʱ��¼ѡ���ĸ���
		for(i=0;i<M-1;i++)//M-1����ֻѡ��ǰM�������һ���������Ÿ���
		{
			double rand_temp=randnum()*ada_sum;
			for(int j=0;j<M&&ada_temp[j]<rand_temp;j++);//���̶��㷨
			for(int k=0;k<N;k++)
				temp_pop[i][k]=pop[j][k];
		}

		for(int k=0;k<N;k++)//���Ÿ��岻��ѡ��ֱ�ӽ�����һ���Ŵ�
			temp_pop[M-1][k]=best_individual[k];
	
		for(i=0;i<M;i++)//ѡ�����ɵ�Ⱥ�帳����ʼȺ��
			for(int j=0;j<N;j++)
				pop[i][j]=temp_pop[i][j];

		//----------------------------------------------------------
		//������򽻻�
		for(i=0;i<M;i=i+2)
		{
			int cross_pos=-1;
			if(randnum()<probability_cross)//�Ը��ʵ���ʽ��������
			{
				cross_pos=int(N*randnum());//�����������λ��
			}
			if(cross_pos>0&&cross_pos<N)//���齻��λ��֮��Ļ���Ի�
			{
				for(int j=cross_pos;j<N;j++)
				{
					bool temp=pop[i][j];
					pop[i][j]=pop[i+1][j];
					pop[i+1][j]=temp;
				}
			}
		}
		//-----------------------------------------------------------
		//����������
		for(i=0;i<M;i++)
		{
			if(randnum()<probability_mutation)//����һ������ʱ�������(0��Ϊ1��1��Ϊ0)
			{
				int mutation_pos=int(N*randnum());
				pop[i][mutation_pos]==1 ? pop[i][mutation_pos]=0:pop[i][mutation_pos]=1;
			}
		}
	}
	
	//�������Ž�ı���xֵ,������
	double best_solution=0;
	for(i=0;i<N;i++)
	{
		if(best_individual[i]==1)
		{
			best_solution=best_solution+pow(2,(N-1-i));
		}
		
	}
	best_solution=best_solution*precision+x1;
	return best_solution;
}


int main()
{
	double x1, x2, x;//������Χ�ͷ��ص�����ֵ
	srand( (unsigned)time( NULL ) );//��ʼ��ʱ������
	//ofstream outf("result.txt");
	cout<<"�������������"<<endl;
	cin>>x1>>x2;
	for(int i=0;i<100;i++)
	{
		x=GA(x1,x2);
		double y=quanlityfun(x);
		cout<<"("<<x<<"   "<<y<<")"<<endl;
		//outf<<x<<"   "<<y<<endl;		
	}
	//outf<<flush;outf.close();//�ر��ļ�
	return 1;
}