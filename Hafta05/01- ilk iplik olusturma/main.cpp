#include<iostream>
#include<thread>
using namespace std;

void merhaba_fonksiyonu(uint64_t id)
{
	cout << "Merhaba ben iplik:" << id << endl;
}


int main()
{
	thread iplik0(merhaba_fonksiyonu, 0);
	thread iplik1(merhaba_fonksiyonu, 1);
	thread iplik2(merhaba_fonksiyonu, 2);
	thread iplik3(merhaba_fonksiyonu, 3);
	iplik0.join();
	iplik1.join();
	iplik2.join();
	iplik3.join();
	
}