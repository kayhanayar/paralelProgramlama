#include "yardimci.hpp"
#include<thread>
using namespace std;
//128 bit veri yapısı
//64 baytlık ön bellek satırında tam sığabilecek bir yapı
struct paket
{
	uint64_t ying;
	uint64_t yang;
	paket():ying(0),yang(0) {}
};

void seriArttirma(volatile paket& p)
{
	for (uint64_t i = 0; i < 1UL << 30; i++)
	{
		p.ying++;
		p.yang++;
	}
}

void yanlisPaylasmaliArttirma(volatile paket& p)
{
	auto eval_ying = [&p]() {
		for (uint64_t i = 0; i < 1UL << 30; i++)
			p.ying++;
	};
	auto eval_yang = [&p]() {
		for (uint64_t i = 0; i < 1UL << 30; i++)
			p.yang++;
	};
	thread ying_iplik(eval_ying);
	thread yang_iplik(eval_yang);
	ying_iplik.join();
	yang_iplik.join();
}
int main()
{
	paket seriPaket;
	const uint64_t n = 1UL << 8;	//1 değeri 15 bit sola öteleniyor yani 2^15 değeri
	const uint64_t m = 1UL << 8;

	TIMERSTART(SeriArttirma)
		seriArttirma(seriPaket);
	TIMERSTOP(SeriArttirma)

	cout << seriPaket.ying << "," << seriPaket.yang << endl;
	paket paralelPaket;
	TIMERSTART(ParalelArttirma)
		yanlisPaylasmaliArttirma(paralelPaket);
	TIMERSTOP(ParalelArttirma)

	cout << paralelPaket.ying << "," << paralelPaket.yang << endl;



}