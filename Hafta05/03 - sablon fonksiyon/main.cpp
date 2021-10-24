#include<iostream>
#include<thread>
#include<vector>

using namespace std;
template<typename id_tur>
void merhaba_fonksiyonu(id_tur id)
{
	cout << "Merhaba ben iplik:" << id << endl;
}


int main()
{
	const int iplikSayisi = 4;
	//iplik nesnelerini tutacak vektor oluşturuluyor
	vector<thread> iplikler;

	for (int id = 0; id < iplikSayisi; id++)
	{
		//C++11 standartı ile emplace_back fonksiyon gelmiştir.
		//vektorün sonuna eleman ekler. 
		//push_back fonksiyonundan farklı olarak kurucuya birden fazla parametre verebilmektedir.
		//fonksiyonun aldığı iki parametre oluşturulacak olan iplik nesnesinin kurucusuna verilmektedir.

		iplikler.emplace_back(
			merhaba_fonksiyonu<uint64_t>, id
		);
	}
	//iplikler birleştiriliyor
	for (auto& iplik : iplikler)
		iplik.join();
}