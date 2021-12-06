
#include<omp.h>
#include<iostream>
using namespace std;


int main()
{
	int max=6;

	#pragma omp parallel for num_threads(4)
	for(int i=1;i<=max;i++)
		for(int j =1;j<=max;j++)
		{
			#pragma omp critical
            {
 				cout<<omp_get_thread_num()<<":"<<i<<","<<j<<endl;
            }
		}

	cout<<"------------------------"<<endl;
	#pragma omp parallel for collapse(2) num_threads(4)
	for(int i=1;i<=max;i++)
		for(int j =1;j<=max;j++)
		{
			#pragma omp critical
            {
 				cout<<omp_get_thread_num()<<":"<<i<<","<<j<<endl;
            }
		}
	cout<<"------------------------"<<endl;
	omp_set_nested(1);
	#pragma omp parallel for num_threads(4)
	for(int i=1;i<=max;i++)
		#pragma omp parallel for num_threads(4)
		for(int j =1;j<=max;j++)
		{
			 #pragma omp critical
                {
 					cout<<omp_get_thread_num()<<":"<<i<<","<<j<<endl;
                }
			
		}
		
}
