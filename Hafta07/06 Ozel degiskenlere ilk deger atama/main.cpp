
#include<omp.h>
#include<iostream>
using namespace std;


int main()
{
	int i = 1;
	int j = 0;

	#pragma omp parallel for
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				#pragma omp critical
				cout<<i<<" "<<j<<":"<<omp_get_thread_num()<<endl;
			}
		}
		

	printf("------------\n");
	#pragma omp parallel firstprivate(i)
	{
		printf("%d\n",i);
	}	
}
