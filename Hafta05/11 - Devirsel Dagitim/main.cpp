#include "yardimci.hpp"
using namespace std;

void init(
	vector<uint64_t>& A,
	vector<uint64_t>& x,
	uint64_t m,
	uint64_t n
)
{
	for (uint64_t satir = 0; satir < m; satir++)
	{
		for (uint64_t sutun = 0; sutun < n; sutun++)
		{
			A[satir * n + sutun] = satir >= sutun ? 1 : 0;
		}
	}
	
	for (uint64_t sutun = 0; sutun < n; sutun++)
	{
		x[sutun] = sutun;
	}
}
void seri_carpim(
	vector<uint64_t >& A,
	vector<uint64_t >& x,
	vector<uint64_t >& sonucVector,
	uint64_t m,
	uint64_t n
)
{
	for (uint64_t satir = 0; satir < m; satir++)
	{
		uint64_t toplam = 0;
		for (uint64_t sutun = 0; sutun < n; sutun++)
		{
			toplam += A[satir * n + sutun] * x[sutun];
		}
		sonucVector[satir] = toplam;
	}


}
void devirsel_paralel_carpim(
	vector<uint64_t>& A,
	vector<uint64_t>& x,
	vector<uint64_t>& sonucVector,
	uint64_t m,
	uint64_t n,
	uint64_t iplikSayisi =8
)
{

	auto iplikFonksiyonu = [&](const uint64_t& id)
	{
		for (uint64_t satir = id; satir < m; satir+=iplikSayisi)
		{
			uint64_t toplam = 0;
			for (uint64_t sutun = 0; sutun < n; sutun++)
				toplam += A[satir * n + sutun] * x[sutun];

			sonucVector[satir] = toplam;
		}

	};

	vector<thread> iplikler;
	for (uint64_t id = 0; id < iplikSayisi; id++)
		iplikler.emplace_back(iplikFonksiyonu, id);

	for (auto& iplik : iplikler)
		iplik.join();
}

int main()
{
	const uint64_t n = 1UL << 8;	//1 değeri 15 bit sola öteleniyor yani 2^15 değeri
	const uint64_t m = 1UL << 8;
	TIMERSTART(TumProgram)

	TIMERSTART(YerAlma)
	vector<uint64_t> A(m * n);
	vector<uint64_t> x( n);
	vector<uint64_t> sonucVector(m);
	TIMERSTOP(YerAlma)

	TIMERSTART(ilkAtama)
	init(A, x, m, n);
	TIMERSTOP(ilkAtama)


	TIMERSTART(carpma)
	seri_carpim(A, x,sonucVector, m, n);
	TIMERSTOP(carpma)

	TIMERSTART(paralelCarpma)
	blok_paralel_carpim(A, x, sonucVector, m, n);
	TIMERSTOP(paralelCarpma)
	
	TIMERSTOP(TumProgram)
		
}