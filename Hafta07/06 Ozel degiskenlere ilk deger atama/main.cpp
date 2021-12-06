
#include<omp.h>
#include<iostream>
using namespace std;


int main()
{
	int i = 1;
	
	#pragma omp parallel private(i)
	{
		printf("%d\n",i);
	}
	printf("------------\n");
	#pragma omp parallel firstprivate(i)
	{
		printf("%d\n",i);
	}	
}
