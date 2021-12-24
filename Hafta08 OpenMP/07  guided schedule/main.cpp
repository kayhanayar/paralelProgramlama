#include "Yardimci.hpp"
#include<omp.h>
#include<iostream>
using namespace std;

int main()
{

	uint64_t turSayisi=64;
	uint64_t toplam=0;


	TIMERSTART(statik)
 	#pragma omp parallel for num_threads(2) reduction(+:toplam) schedule(static,4)
	for(uint64_t i=0;i<turSayisi;i++)
	{
		#pragma omp critical
		cout<<"i:"<<i<<" | "<<omp_get_thread_num()<<endl;
		toplam+=i;
	}
	TIMERSTOP(statik)
 	
	TIMERSTART(guided)
	cout<<"------------------------------------"<<endl;
	string diziler[4];
 	#pragma omp parallel for num_threads(4) reduction(+:toplam) schedule(guided,8)
	for(uint64_t i=0;i<turSayisi;i++)
	{
		#pragma omp critical
		diziler[omp_get_thread_num()]+=std::to_string(i)+string(" ");
		toplam+=i;
	}	 
	TIMERSTOP(guided)
	for(int i=0;i<4;i++)
	{
		cout<<i<<":"<<diziler[i]<<endl;
	}
	///ipliklerin yapması gereken işler.
}
// omp_single.cpp
// compile with: /openmp

