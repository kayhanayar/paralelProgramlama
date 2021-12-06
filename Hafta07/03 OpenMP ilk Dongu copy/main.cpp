#include "Yardimci.hpp"
#include<omp.h>
#include<iostream>
using namespace std;

int main()
{
	const uint64_t girisSayisi = 1UL << 20;
	vector<uint64_t> x(girisSayisi);
	vector<uint64_t> y(girisSayisi);
	vector<uint64_t> z(girisSayisi);


	
	#pragma omp parallel 
	{
		
		#pragma omp for
		for (uint64_t  i = 0; i < girisSayisi; i++)
		{
			x[i] = i;
			y[i] = girisSayisi - i;
		}
	

		
		#pragma omp  for
		for(uint64_t i=0;i<girisSayisi;i++)
			z[i]=x[i]+y[i];		



		
		
		#pragma omp  for
		for(uint64_t i=0;i<girisSayisi;i++)
		{
			if(z[i]-girisSayisi)
				cout<<i<<". indiste hata var"<<endl;
		}
					
	
	}

}
