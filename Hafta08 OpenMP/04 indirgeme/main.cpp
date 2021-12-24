#include "Yardimci.hpp"
#include<omp.h>
#include<iostream>
using namespace std;

int main()
{

	uint64_t turSayisi=1<<30;
	uint64_t toplam=0;

	TIMERSTART(kritikZaman)
 	#pragma omp parallel for num_threads(4)
	for(uint64_t i=0;i<turSayisi;i++)
	{
		#pragma omp critical
		toplam+=i;
	}
	cout<<"kritik bolge ile toplam="<<toplam<<endl;
	TIMERSTOP(kritikZaman)

	toplam=0;
 	TIMERSTART(atomik)
	 #pragma omp parallel for num_threads(4)
	for(uint64_t i=0;i<turSayisi;i++)
	{
		#pragma omp atomic
		toplam+=i;
	}
	cout<<"atomic ile toplam="<<toplam<<endl;
	TIMERSTOP(atomik)

	toplam=0;
	TIMERSTART(indirgeme)
 	#pragma omp parallel for num_threads(4) reduction(+:toplam)
	for(uint64_t i=0;i<turSayisi;i++)
	{
		toplam+=i;
	}
	cout<<"indirgeme ile toplam="<<toplam<<endl;
 	TIMERSTOP(indirgeme)
}