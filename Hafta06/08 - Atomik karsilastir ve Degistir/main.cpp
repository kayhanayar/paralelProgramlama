#include "yardimci.hpp"
#include<iomanip>
#include<mutex>
#include <atomic>
using namespace std;


int main()
{
	mutex kilitMutex;
	vector<thread> iplikler;
	const uint64_t iplikSayisi = 10;
	const uint64_t iterasyonSayisi = 100'000'000;
	
	auto yanlisMax = [&](volatile atomic<uint64_t>* sayac, const auto& id)
	{
		for (uint64_t i = id; i < iterasyonSayisi; i += iplikSayisi) {
			
			if(i>*sayac)
				(*sayac)=i;
		}
	};
	auto dogruMax = [&](volatile atomic<uint64_t>* sayac, const auto& id)
	{
		for (uint64_t i = id; i < iterasyonSayisi; i += iplikSayisi) {
			auto onceki = sayac->load();
			while(onceki<i&&
				!sayac->compare_exchange_weak(onceki,i)){}
		}
	};

	TIMERSTART(yanlisMax)
	atomic<uint64_t> yanlisSayac(0);
	iplikler.clear();

	for (uint64_t id = 0; id < iplikSayisi; id++)
		iplikler.emplace_back(yanlisMax, &yanlisSayac, id);
	for (auto& iplik : iplikler)
		iplik.join();

	TIMERSTOP(yanlisMax)



	TIMERSTART(dogruMax)
	atomic<uint64_t> dogruSayac(0);
	iplikler.clear();

	for (uint64_t id = 0; id < iplikSayisi; id++)
		iplikler.emplace_back(dogruMax, &dogruSayac, id);
	for (auto& iplik : iplikler)
		iplik.join();

	TIMERSTOP(dogruMax)

		cout << "yanlisMax():" << yanlisSayac << endl;
	cout<<"dogruMax():"<< dogruSayac << endl;
}