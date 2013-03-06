//copyright by zhangpeng ,May 5,2007

#include <stdlib.h>
#include <time.h>
//#include <fstream.h>
#include <math.h>

#include <iostream>
using namespace std;



//-------------------------------------------
//产生（0，1）均匀分布的随机数
double randnum()
{
	double x;
	x = (double) rand()/RAND_MAX;
	return x;
}

//-------------------------------------------
//品质函数
inline double quanlityfun(double x)
{
	double y;
	y=-(2*pow(x,2)-x-1);
//	y=-pow((x-1),2)+4;
//	y=fabs(sin(3.1415926*(x-3))/3.1415926/(x-3));
	return y;
}

//-------------------------------------------
//遗传算法
double GA(double x1,double x2)
{
	const int M=40;//群体大小，指群体包含的个体数目，一般取(20-100)
	const int N=11;//染色体的码元数
	const int Generation=200;//遗传算法的终止迭代代数，一般取(100-500)
	double probability_cross=0.7;//交叉概率，一般取(0.4-0.99)
	double probability_mutation=0.1;//变异概率，一般取(0.0001,0.1)
	double precision=(x2-x1)/(pow(2,N)-1);//二进制编码的精度

	//----------------------------------------------------------------
	//采用随机算法，初始化种群
	bool pop[M][N];//定义种群变量
	bool best_individual[N];//保存一代中的最优个体
	for(int i=0;i<M;i++)
	{	for(int j=0;j<N;j++)
		{
			if(randnum()>0.5)
				pop[i][j]=1;
			else
				pop[i][j]=0;				
		}		
	}
	//遗传算法的核心迭代
	for(int iter=0;iter<Generation;iter++)
	{
		//------------------------------------------------------------
		//选择。计算群体中每一个体的适应度
		double adapt[M];//定义群体的适应度变量
		for(int i=0;i<M;i++)//适应度变量赋初值0
			adapt[i]=0;
		for(i=0;i<M;i++)//个体值的解码(二进制码变为十进制码)
		{	for(int j=0;j<N;j++)
			{
				if(pop[i][j]==1)
					adapt[i]=adapt[i]+pow(2,(N-1-j));
			}
			adapt[i]=adapt[i]*precision+x1;//二进制编码所对应的实际的品质函数变量值
			adapt[i]=quanlityfun(adapt[i]);//计算群体的品质函数			
		}

		//精英操作，保留品质最好的一个个体，不选择不异化，直接进入下一轮遗传
		double adapt_best=0;//记录最好的个体
		int best_pos=0;//记录最好个体的位置
		for(i=0;i<M;i++)
		{
			if(adapt_best<adapt[i])
			{adapt_best=adapt[i];best_pos=i;}
		}

		//保存最优个体
		for(i=0;i<N;i++)
			best_individual[i]=pop[best_pos][i];
		
		//采用轮盘赌方式选择个体
		double ada_sum=0;//记录总的适应值(品质因数)
		double ada_temp[M];//记录累加的适应值(品质因数)
		for(i=0;i<M;i++)//适应值计算
		{
			ada_sum=ada_sum+adapt[i];
			ada_temp[i]=ada_sum;
		}
	
		bool temp_pop[M][N];//临时记录选出的个体
		for(i=0;i<M-1;i++)//M-1代表只选择前M个，最后一个留给最优个体
		{
			double rand_temp=randnum()*ada_sum;
			for(int j=0;j<M&&ada_temp[j]<rand_temp;j++);//轮盘赌算法
			for(int k=0;k<N;k++)
				temp_pop[i][k]=pop[j][k];
		}

		for(int k=0;k<N;k++)//最优个体不经选择直接进入下一轮遗传
			temp_pop[M-1][k]=best_individual[k];
	
		for(i=0;i<M;i++)//选择后组成的群体赋给初始群体
			for(int j=0;j<N;j++)
				pop[i][j]=temp_pop[i][j];

		//----------------------------------------------------------
		//个体基因交换
		for(i=0;i<M;i=i+2)
		{
			int cross_pos=-1;
			if(randnum()<probability_cross)//以概率的形式交换基因
			{
				cross_pos=int(N*randnum());//随机产生交换位置
			}
			if(cross_pos>0&&cross_pos<N)//两组交换位置之后的基因对换
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
		//个体基因变异
		for(i=0;i<M;i++)
		{
			if(randnum()<probability_mutation)//满足一定概率时基因变异(0变为1，1变为0)
			{
				int mutation_pos=int(N*randnum());
				pop[i][mutation_pos]==1 ? pop[i][mutation_pos]=0:pop[i][mutation_pos]=1;
			}
		}
	}
	
	//计算最优解的变量x值,并返回
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
	double x1, x2, x;//变量范围和返回的最优值
	srand( (unsigned)time( NULL ) );//初始化时间种子
	//ofstream outf("result.txt");
	cout<<"请输入变量区间"<<endl;
	cin>>x1>>x2;
	for(int i=0;i<100;i++)
	{
		x=GA(x1,x2);
		double y=quanlityfun(x);
		cout<<"("<<x<<"   "<<y<<")"<<endl;
		//outf<<x<<"   "<<y<<endl;		
	}
	//outf<<flush;outf.close();//关闭文件
	return 1;
}
