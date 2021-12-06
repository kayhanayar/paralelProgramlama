#include "yardimci.hpp"
#include<iomanip>
#include<mutex>
using namespace std;


int main()
{
	promise<void> promise;

	auto future = promise.get_future();

	auto ogrenci = [&]()
	{
		future.get();
		cout << "Kahve Zamani Geldi" << endl;
	};
	
	thread iplik(ogrenci);
	this_thread::sleep_for(2s);

	promise.set_value();
	iplik.join();
	
}