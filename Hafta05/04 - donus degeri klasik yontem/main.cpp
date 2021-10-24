#include<iostream>
#include<thread>
#include<vector>

using namespace std;
//fonksiyon dönüş değerini parametresi içerisine yerleştiriyor
void fibo(uint64_t n,uint64_t * donus)
{
	uint64_t a0 = 0;
	uint64_t a1 = 1;
	for (int i = 0; i < n; i++)
	{
		uint64_t gecici = a0; 
		a0 = a1; 
		a1 += gecici;
	}
	*donus= a0;
}


int main()
{
	const int iplikSayisi = 32;
	//iplik nesnelerini tutacak vektor oluşturuluyor
	vector<thread> iplikler;

	vector<uint64_t> sonuclar(iplikSayisi,0);
	for (int id = 0; id < iplikSayisi; id++)
	{
		//C++11 standartı ile emplace_back fonksiyon gelmiştir.
		//vektorün sonuna eleman ekler. 
		//push_back fonksiyonundan farklı olarak kurucuya birden fazla parametre verebilmektedir.
		//fonksiyonun aldığı iki parametre oluşturulacak olan iplik nesnesinin kurucusuna verilmektedir.

		iplikler.emplace_back(
			fibo, id,&(sonuclar[id])
		);
	}
	//iplikler birleştiriliyor
	for (auto& iplik : iplikler)
		iplik.join();
	for (auto& sonuc : sonuclar)
		cout << sonuc << endl;
}