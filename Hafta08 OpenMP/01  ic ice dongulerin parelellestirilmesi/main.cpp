#include "Yardimci.hpp"
#include<omp.h>
#include<iostream>
using namespace std;

int main()
{

 	#pragma omp parallel for num_threads(4)
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<4;j++)
		{
			#pragma omp critical
			cout<<omp_get_thread_num()<<" i,j="<<i<<","<<j<<endl;
		}
	}
	cout<<"------------------------------------"<<endl;
 	#pragma omp parallel for collapse(2) num_threads(4)
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<4;j++)
		{
			#pragma omp critical
			cout<<omp_get_thread_num()<<" i,j="<<i<<","<<j<<endl;
		}
	}
}