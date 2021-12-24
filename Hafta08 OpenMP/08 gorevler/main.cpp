#include "Yardimci.hpp"
#include<omp.h>
#include<iostream>
using namespace std;

int main()
{
	cout<<endl<<"-------------BEKLEMESIZ---------------"<<endl;
	#pragma omp parallel
	{
		#pragma omp single
		{
			cout<<"gorevler dagitiliyor: ";

			#pragma omp task
			cout<<"yaris-";
			#pragma omp task
			cout<<"araba-";
			cout<<"dagitim bitti";
		}
	}

	cout<<endl<<"-------------BEKLEMELI---------------"<<endl;

	#pragma omp parallel
	{
		#pragma omp single
		{
			cout<<"gorevler dagitiliyor: ";

			#pragma omp task
			cout<<"yaris-";
			#pragma omp task
			cout<<"araba-";
			#pragma omp taskwait
			cout<<"dagitim bitti";
		}
	}
}
// omp_single.cpp
// compile with: /openmp

