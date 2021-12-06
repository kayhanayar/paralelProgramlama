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
	
	auto kilit_Sayma = [&](volatile uint64_t* sayac, const auto& id)
	{
		for (uint64_t i = id; i < iterasyonSayisi; i += iplikSayisi) {
			lock_guard<mutex> kilit(kilitMutex)
			(*sayac)++;
		}
	};
	auto atomic_Sayma = [&](volatile atomic<uint64_t>* sayac, const auto& id)
	{
		for (uint64_t i = id; i < iterasyonSayisi; i += iplikSayisi) {
			(*sayac)++;
		}
	};

	TIMERSTART(mutexile)
	uint64_t sayac = 0;
	iplikler.clear();
	for (uint64_t id = 0; id < iplikSayisi; id++)
		iplikler.emplace_back(kilit_Sayma, &sayac, id);
	for (auto& iplik : iplikler)
		iplik.join();
	TIMERSTOP(mutexile)


	TIMERSTART(atomiklerile)
	atomic<uint64_t> atomicSayac(0);
	iplikler.clear();

	for (uint64_t id = 0; id < iplikSayisi; id++)
		iplikler.emplace_back(atomic_Sayma, &atomicSayac, id);
	for (auto& iplik : iplikler)
		iplik.join();

	TIMERSTOP(atomiklerile)

		cout << sayac << "," << atomicSayac << endl;
}