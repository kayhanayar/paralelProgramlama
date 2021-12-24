#include "Yardimci.hpp"
#include<omp.h>
#include<iostream>
#include<functional>
#include<cmath>
using namespace std;

class Dugum
{
public:
	uint64_t deger;
	Dugum* sol;
	Dugum* sag;
	Dugum(){};
	Dugum(int deger)
	{
		this->deger= deger;
		this->sol=0;
		this->sag=0;
	}
	Dugum(int deger,Dugum* sol,Dugum* sag)
	{
		this->deger=deger;
		this->sol = sol;
		this->sag = sag;
	}

	void dolas(auto fonksiyon,bool paralelOlsunmu=false)
	{
		
		if(sol)
		{
			#pragma omp task if(paralelOlsunmu)
			sol->dolas(fonksiyon,paralelOlsunmu);
		}
		fonksiyon(deger);

		if(sag)
		{
			#pragma omp task if(paralelOlsunmu)
			sag->dolas(fonksiyon,paralelOlsunmu);
		}
	}



};



int main()
{
	//tam dengeli bir ağacın yüksekliği
	const uint64_t m = 15;
	//tam dengeli bir ağacın içerisindeki eleman sayısı
	const uint64_t n = (1UL<<m)-1;
	//her Görev içerisindeki tur sayısı
	const uint64_t turSayisi = 1UL<<12;


	Dugum dugumler[n];

	for(uint64_t i = (n-1);i>0;--i)
	{
		if(i>((n/2)-1))
		{
			//en alttaki düğümler çocukları yok
			dugumler[i]= Dugum(i);
			
		}
		else
		{
			dugumler[i] = Dugum(i,&dugumler[2*i+1],&dugumler[2*i+2]);
		
		}
	}

	Dugum agac= Dugum(0,&dugumler[1],&dugumler[2]);
	dugumler[0]= agac;

	TIMERSTART(toplama)
	uint64_t toplam=0;
	
	auto toplaFonksiyonu = 	[&](uint64_t& val){
								toplam+=val;
							};	
	agac.dolas(toplaFonksiyonu);
	std::cout<<"toplam:"<<toplam<<std::endl;
	TIMERSTOP(toplama)


	auto ustAlmaFonksiyonu = [](uint64_t& deger){
		for(uint64_t i=0;i<turSayisi;++i)
		{
			deger=std::pow(deger,1.1);
		}
	};

	TIMERSTART(seri)
	agac.dolas(ustAlmaFonksiyonu);
	TIMERSTOP(seri)

	TIMERSTART(paralel)
	#pragma omp parallel
	{
		#pragma omp single
		{
			#pragma omp task
			agac.dolas(ustAlmaFonksiyonu,true);
		}
	}
	
	TIMERSTOP(paralel)
}
// omp_single.cpp
// compile with: /openmp

