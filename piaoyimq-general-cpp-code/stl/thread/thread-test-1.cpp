#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <fstream>

void f1(int n)
{
	std::cout << "f1, " << "std::this_thread::get_id()=" << std::this_thread::get_id() << std::endl;
	for (int i = 0; i < 5; ++i)
	{
		std::cout << "Thread 1 executing\n";
		++n;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void f2(int& n)
{
	std::cout << "f2, " << "std::this_thread::get_id()=" << std::this_thread::get_id() << std::endl;
	for (int i = 0; i < 5; ++i)
	{
		std::cout << "Thread 2 executing\n";
		++n;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void test_join()
{
	int n = 0;
	std::thread t1; // t1 is not a thread
	std::cout << std::boolalpha;
	std::cout << "t1.joinable=" << t1.joinable() << '\n';

	std::thread t2(f1, n + 1); // pass by value
	std::cout << "t2.joinable=" << t2.joinable() << '\n';

	std::thread t3(f2, std::ref(n)); // pass by reference
	t3.join();
	std::cout << "t3.joinable=" << t3.joinable() << "\n\n";

	std::thread::id t3_id_1 = t3.get_id();

//	std::thread t4(std::move(t3)); // t4 is now running f2(). t3 is no longer a thread
//	std::cout << "t3.joinable=" << t3.joinable() << '\n';
//	std::cout << "t4.joinable=" << t4.joinable() << '\n';
//
//	std::thread::id t1_id = t1.get_id();
//	std::thread::id t2_id = t2.get_id();
//	std::thread::id t3_id = t3.get_id();
//	std::thread::id t4_id = t4.get_id();
//
//	std::cout << "t1_id=" << t1_id << std::endl;
//	std::cout << "t2_id=" << t2_id << std::endl;
//	std::cout << "t3_id_1=" << t3_id_1 << std::endl;
//	std::cout << "t3_id=" << t3_id << std::endl;
//	std::cout << "t4_id=" << t4_id << std::endl;
//
//	std::cout << "std::this_thread::get_id()=" << std::this_thread::get_id() << std::endl;
//	t2.join();
//	t4.join();
//	std::cout << "Final value of n is " << n << '\n';

	std::cout << "std::thread::id()=" << std::thread::id() << std::endl;
}

void independentThread()
{
	std::cout << "Starting concurrent thread.\n";
	std::this_thread::sleep_for(std::chrono::seconds(8));
	std::cout << "Exiting concurrent thread.\n";
}

void threadCaller()
{
	std::cout << "Starting thread caller.\n";
	std::thread t(independentThread);
	t.detach();//when main process exit, the detached thread will be destoried,
	//if the main process not exit, the detached thread will run, until the the detached thread automatically exit or main process exit.
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Exiting thread caller.\n";
}

void test_detach()
{
	threadCaller();
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << "Exit end\n";
}

class ThreadClass{
public:
    int myThread(int arg){
     // do something
    }

//    void createThread(){
//        std::thread t = std::thread(myThread,10);
//    }

} ;


int main()
{
	test_join();
	test_detach();
//	ThreadClass tc;
//	std::thread t(tc.myThread,  10);

}
