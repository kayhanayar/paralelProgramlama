
#include<omp.h>
#include<iostream>
using namespace std;


int main()
{
	int adet = omp_get_max_threads();
	int* gecici = new int[adet];

	int i=1;
	#pragma omp parallel firstprivate(i) num_threads(adet)
	{
		int j=omp_get_thread_num();
		i+=j;
		gecici[j]=i;
	}
	for(int i=0;i<adet;i++)
		cout<<gecici[i]<<" ";
	delete[] gecici;

	#pragma omp parallel for lastprivate(i) num_threads(adet)
	for(int j=0;j<16;j++)
		i=j;
	
	cout<< i<<endl;
}
//Diğer fonksiyonların tanımlandığını 
//ve çağrıldığını varsayın
