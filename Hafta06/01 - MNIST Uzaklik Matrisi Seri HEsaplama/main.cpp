#include "yardimci.hpp"
#include<iomanip>
using namespace std;

void seri_uzaklik_Hesaplamasi(
	vector<float>& mnist,
	vector<float>& uzaklikMatrisi,
	uint64_t satir,
	uint64_t sutun
)
{
	for (uint64_t i = 0; i < satir; i++){
		for (uint64_t I = 0; I <= i; I++){
			float toplam = 0;
			for (uint64_t j = 0; j < sutun; j++){
				float fark = mnist[i * sutun + j] - mnist[I * sutun + j];
				toplam += fark * fark;
			}
			uzaklikMatrisi[i * satir + I] =  toplam;
			uzaklikMatrisi[I * satir + i] = toplam;
		}
	}
}

int main()
{
	const int satir = 65000;
	const int sutun = 28 * 28;
	TIMERSTART(VeriyiDisktenOkuma)
	std::vector<float> mnist(satir * sutun);
	load_binary(mnist.data(), satir * sutun, "mnist_65000_28_28_32.bin");
	TIMERSTOP(VeriyiDisktenOkuma)

	TIMERSTART(MesafeleriHesaplama)
	std::vector<float> mesafeMatrisi(satir * satir);
	seri_uzaklik_Hesaplamasi(mnist, mesafeMatrisi, satir, sutun);
	TIMERSTOP(MesafeleriHesaplama)
}