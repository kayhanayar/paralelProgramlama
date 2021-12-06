#include "yardimci.hpp"
#include<iomanip>
using namespace std;

void dinamik_blok_paralel_uzaklik_Hesaplamasi(
	vector<float>& mnist,
	vector<float>& uzaklikMatrisi,
	uint64_t satir,
	uint64_t sutun,
	uint64_t iplikSayisi = 8,
	uint64_t parcaBoyutu=8/(sizeof(float))
)
{
	std::mutex mutex;
	uint64_t global_sinir=0;
	auto devirli_blok = [&](const uint64_t& id) {

		uint64_t dusukSinir = 0;

		while (dusukSinir < satir)
		{

			{
				lock_guard<mutex> kritikBolge(mutex);
				dusukSinir = global_sinir;
				global_sinir += parcaBoyutu;
			}
			const uint64_t ust = std::min(dusukSinir + parcaBoyutu, satir);
			for (uint64_t i = dusukSinir; i < ust; i++)
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
	dinamik_blok_paralel_uzaklik_Hesaplamasi(mnist, mesafeMatrisi, satir, sutun);
	TIMERSTOP(MesafeleriHesaplama)
}

