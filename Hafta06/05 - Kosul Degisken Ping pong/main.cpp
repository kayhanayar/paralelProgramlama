#include "yardimci.hpp"
#include<iomanip>
#include<mutex>
using namespace std;


int main()
{
	mutex mutexOyun;
	condition_variable cv;
	bool pingAktifmi = true;

	auto ping = [&]()
	{
		while(true)
		{
			unique_lock<mutex> kilit(mutexOyun);
			
			cv.wait(kilit, [&](){return pingAktifmi;});
			this_thread::sleep_for(1s);
			cout << "ping"<< endl;
			pingAktifmi = !pingAktifmi;
			cv.notify_one();
		}
	};

	auto pong = [&]()
	{
		while (true)
		{
			unique_lock<mutex> kilit(mutexOyun);

			cv.wait(kilit, [&]() {
				return !pingAktifmi;
				}
			);
			this_thread::sleep_for(1s);
			cout << "pong" << endl;

			pingAktifmi = !pingAktifmi;
			cv.notify_one();
		}
	};
	thread pingThread(ping);
	thread pongThread(pong);

	pingThread.join();
	pongThread.join();
}