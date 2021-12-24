#include "Yardimci.hpp"
#include<omp.h>
#include<iostream>
using namespace std;

int main()
{
	const uint64_t girisSayisi = 1UL << 24;
	vector<uint64_t> x(girisSayisi);
	vector<uint64_t> y(girisSayisi);
	vector<uint64_t> z(girisSayisi);


	TIMERSTART(ilkAtama)
	#pragma omp parallel for
	for (uint64_t  i = 0; i < girisSayisi; i++)
	{
		x[i] = i;
		y[i] = girisSayisi - i;
	}
	TIMERSTOP(ilkAtama);

	TIMERSTART(seriToplama)
	for(uint64_t i=0;i<girisSayisi;i++)
		z[i]=x[i]+y[i];
	TIMERSTOP(seriToplama)

	TIMERSTART(paralelToplama)
	#pragma omp parallel for
	for(uint64_t i=0;i<girisSayisi;i++)
		z[i]=x[i]+y[i];		
	TIMERSTOP(paralelToplama)

	TIMERSTART(paralelKontrol)
	#pragma omp parallel for
	for(uint64_t i=0;i<girisSayisi;i++)
	{
		if(z[i]-girisSayisi)
			cout<<i<<". indiste hata var"<<endl;
	}
				
	TIMERSTOP(paralelKontrol)
}