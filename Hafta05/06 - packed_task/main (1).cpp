#include<iostream>
#include<thread>
#include<vector>
#include<future>
using namespace std;
//fonksiyon dönüş değerini promise(söz) olarak almaktadır.
//söz parametresi rvalue referans olarak verilmektedir.
//bu sayede yerel değişken içerisindeki değerler fonksiyona aktarılacaktır.
//fonksiyon kendi kopyasını oluşturmayacaktır.
//promise set edilene kadar(yani değeri atanana kadar) 
//bağlı olduğu future nesnesinin değeri getiremez.
//Burada amacımı promise nesnesinin kontrolünü tümüyle ilgili ipliğe devretmek
//ana iplikteki promise nesnesi devirden sonra boşaltılmış olacaktır.
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
	//fonksiyon sonunda promise değeri atanıyor
	//fonksiyonun yeri değiştirilirse future nesnesinin getirdiği değerde değişecektir.
	return a0;
}


int main()
{
	const int iplikSayisi = 32;
	//iplik nesnelerini tutacak vektor oluşturuluyor
	vector<thread> iplikler;
	//sonuçların aktarılacak future nesneleri için bir vektör oluşturuluyor
	vector<future<uint64_t>> sonuclar;

	for (int id = 0; id < iplikSayisi; id++)
	{
		//sıradaki iplik için bir promise nesnesi oluşturuluyor
		//bu nesne yerel olarak oluşturulmuştur.
		std::packaged_task<uint64_t(uint64_t)> gorev(fibo);

		//promise nesnesine bağlı olan future nesnesi sonuclar listesine aktarılıyor
		sonuclar.emplace_back(gorev.get_future());
		
		//iplikler listeye atanıyor.
		iplikler.emplace_back(
			std::move(gorev), 
			id						
		);
	}
	//ipliklerin sonucları isteniyor
	//ilgili ipliğin promise nesnesine değer atanmadığı taktirde
	//promise nesnesinin bağlı olduğu future nesnesinin get metodu 
	//main fonksiyonunu(ana iplik) bloke edecektir.

	for (auto& sonuc : sonuclar)
		cout << sonuc.get() << endl;

	for (auto& iplik : iplikler)
		iplik.join();
}