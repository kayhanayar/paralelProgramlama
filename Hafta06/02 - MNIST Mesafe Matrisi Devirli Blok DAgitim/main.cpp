#include "yardimci.hpp"
#include<iomanip>
using namespace std;

void devirli_blok_paralel_uzaklik_Hesaplamasi(
	vector<float>& mnist,
	vector<float>& uzaklikMatrisi,
	uint64_t satir,
	uint64_t sutun,
	uint64_t iplikSayisi = 8,
	uint64_t parcaBoyutu=8/(sizeof(float))
)
{
	auto devirli_blok = [&](const uint64_t& id) {
		const uint64_t bas = id * parcaBoyutu;
		const uint64_t uzun = iplikSayisi * parcaBoyutu;
		for (uint64_t alt = bas; alt < satir; alt += uzun)
		{
			const uint64_t ust = std::min(alt + parcaBoyutu, satir);
			for (uint64_t i = alt; i < ust; i++)
			{
				for (uint64_t I = 0; I <= i; I++)
				{
					float toplam = 0;
					for (uint64_t j = 0; j < sutun; j++)
					{
						float fark = mnist[i * sutun + j] - mnist[I * sutun + j];
						toplam += fark * fark;
					}
					uzaklikMatrisi[i * satir + I] = toplam;
					uzaklikMatrisi[I * satir + i] = toplam;
				}
			}
		}
	};

	std::vector<thread> iplikler;

	for (uint64_t id = 0; id < iplikSayisi; id++)
		iplikler.emplace_back(devirli_blok, id);

	for (auto& iplik : iplikler)
		iplik.join();
}

int main()
{
	const uint64_t satir = 65000;
	const uint64_t sutun= 28 * 28;
	TIMERSTART(VeriyiDisktenOkuma)
	std::vector<float> mnist(satir * sutun);
	load_binary(mnist.data(), satir * sutun, "mnist_65000_28_28_32.bin");
	TIMERSTOP(VeriyiDisktenOkuma)

	TIMERSTART(MesafeleriHesaplama)
	std::vector<float> mesafeMatrisi(satir * satir);
	devirli_blok_paralel_uzaklik_Hesaplamasi(mnist, mesafeMatrisi, satir, sutun);
	TIMERSTOP(MesafeleriHesaplama)
}