#include<iostream>
#include<thread>
#include<vector>
#include<future>
#include <cstdint>
#include <chrono>

#define TIMERSTART(label)                                                  \
        std::chrono::time_point<std::chrono::system_clock> a##label, b##label; \
        a##label = std::chrono::system_clock::now();
#define TIMERSTOP(label)                                                   \
        b##label = std::chrono::system_clock::now();                           \
        std::chrono::duration<double> delta##label = b##label-a##label;        \
        std::cout << "# elapsed time ("<< #label <<"): "                       \
                  << delta##label.count()  << "s" << std::endl;        

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
	vector<uint64_t>& A,
	vector<uint64_t>& x,
	vector<uint64_t>& sonucVector,
	uint64_t m,
	uint64_t n
)
{
	for (uint64_t satir = 0; satir < m; satir++)
	{
		uint64_t toplam = 0;
		for (uint64_t sutun = 0; sutun < n; sutun++)
		{
			toplam+=A[satir * n + sutun] * x[sutun];
		}
		sonucVector[satir] = toplam;
	}
}

int main()
{
	const uint64_t n = 1UL << 15;	//1 değeri 15 bit sola öteleniyor yani 2^15 değeri
	const uint64_t m = 1UL << 15;
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
	TIMERSTOP(TumProgram)
		
}