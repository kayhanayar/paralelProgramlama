#include<iostream>
#include<omp.h>
using namespace std;


int main()
{
	#pragma omp parallel 
	{
		int a = omp_get_thread_num();
		int n = omp_get_num_threads();
		#pragma omp for
		for(int i=0;i<10;i++)
		{
			cout<<a<<":"<<i<<endl;
		}
	}

}