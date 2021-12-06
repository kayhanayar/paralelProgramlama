#include<iostream>
#include<omp.h>
using namespace std;


int main()
{
	#pragma omp parallel
	{
		int i = omp_get_thread_num();
		int n = omp_get_num_threads();
		cout << "iplik " << i << "dan Merhaba " << endl;
		cout << "iplik sayisi:" << n << endl;
	}

}