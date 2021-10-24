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
void fibo(uint64_t n, promise<uint64_t>&& sonuc)
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
	sonuc.set_value(a0);
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
		promise<uint64_t> promise;

		//promise nesnesine bağlı olan future nesnesi sonuclar listesine aktarılıyor
		sonuclar.emplace_back(promise.get_future());
		
		//iplikler listeye atanıyor.
		iplikler.emplace_back(
			fibo, 
			id, 
			std::move(promise)//promise nesnesi direk verilse yeni bir kopyası oluşturulacaktır.
								//move sayesinde içerik tümüyle kopyalanmaktadır.
								//move işleminin ardından promise nesnesi tümüyle fonksiyona trafnster oldu
								//döngü içerisinde tekrar erişlmemelidir.
								
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