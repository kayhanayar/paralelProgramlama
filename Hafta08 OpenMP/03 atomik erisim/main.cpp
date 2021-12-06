#include "Yardimci.hpp"
#include<omp.h>
#include<iostream>
using namespace std;

int main()
{

	int turSayisi=1<<16;
	int toplam=0;

	#pragma omp parallel for num_threads(4)
	for(int i=0;i<turSayisi;i++)
	{
		toplam+=i;
	}
	cout<<"yaris durumu toplam="<<toplam<<endl;
	toplam=0;
 	#pragma omp parallel for num_threads(4)
	for(int i=0;i<turSayisi;i++)
	{
		#pragma omp atomic
		toplam+=i;
	}
	cout<<"atomic ile toplam="<<toplam<<endl;
 
}