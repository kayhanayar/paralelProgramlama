#include<iostream>
#include<thread>
#include<vector>
#include<future>
using namespace std;

uint64_t fibo(uint64_t n)
{
	uint64_t a0 = 0;
	uint64_t a1 = 1;
	
	for (int i = 0; i < n; i++)
	{
		uint64_t gecici = a0;
		a0 = a1;
		a1 += gecici;
	}
	return a0;
}


int main()
{
	const int iplikSayisi = 32;
	vector<thread> iplikler;

	vector<future<uint64_t>> sonuclar;

	for (int id = 0; id < iplikSayisi; id++)
	{

		packaged_task<uint64_t(uint64_t)> gorev(fibo);
		//ipliklrt async fonksiyonu tarafaından otomatik olarak çalıştırılmaktadır.

		sonuclar.emplace_back(			
			async(launch::async,fibo,id)
		);
		

	}

	for (auto& sonuc : sonuclar)
		cout << sonuc.get() << endl;

}