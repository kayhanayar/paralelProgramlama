#include "yardimci.hpp"
#include<iomanip>
#include<mutex>
using namespace std;


int main()
{
	mutex mutexKahvalti;
	condition_variable cv;
	bool kahvaltiZamani = false;

	auto ogrenci = [&]()
	{
		{
			unique_lock<mutex> kilit(mutexKahvalti);
			
			while (!kahvaltiZamani)
				cv.wait(kilit);

			cout << "uyanma zamani"<< endl;
		}
	};

	thread iplik(ogrenci);

	this_thread::sleep_for(2s);

	{
		lock_guard<mutex> kilit(mutexKahvalti);
		kahvaltiZamani = true;
	}
	cv.notify_one();

	iplik.join();
	
}