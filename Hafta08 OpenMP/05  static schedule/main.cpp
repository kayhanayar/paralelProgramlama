#include "Yardimci.hpp"
#include<omp.h>
#include<iostream>
using namespace std;

int main()
{

	uint64_t turSayisi=13;
	uint64_t toplam=0;


	TIMERSTART(indirgeme)
 	#pragma omp parallel for num_threads(4) reduction(+:toplam) schedule(static)
	for(uint64_t i=0;i<turSayisi;i++)
	{
		#pragma omp critical
		cout<<"i:"<<i<<" | "<<omp_get_thread_num()<<endl;
		toplam+=i;
	}
	
 	TIMERSTOP(indirgeme)

	cout<<"------------------------------------"<<endl;
 	#pragma omp parallel for num_threads(4) reduction(+:toplam) schedule(static,2)
	for(uint64_t i=0;i<turSayisi;i++)
	{
		#pragma omp critical
		cout<<"i:"<<i<<" | "<<omp_get_thread_num()<<endl;
		toplam+=i;
	}	 
}