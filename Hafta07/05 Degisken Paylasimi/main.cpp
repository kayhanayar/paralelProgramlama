
#include<omp.h>
#include<iostream>
using namespace std;


int main()
{
	int i,j;
	
	//hatalı sonuc üretir
	//i ve j 
	#pragma omp parallel for 	
	for( i=0;i<4;i++)
		for( j=0;j<4;j++)
			printf("%d %d | ",i,j);
	printf("\n");
	#pragma omp parallel for private(j) 	
	for( i=0;i<4;i++)
		for( j=0;j<4;j++)
			printf("%d %d | ",i,j);
}
