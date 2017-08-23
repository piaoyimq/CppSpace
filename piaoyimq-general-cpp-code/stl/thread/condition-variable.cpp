#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void worker_thread()
{
    // Wait until main() sends data
    std::unique_lock < std::mutex > lk(m);
    cv.wait(lk, []
    {   return ready;});

    // after the wait, we own the lock.
    std::cout << "Worker thread is processing data\n";
    data += " after processing";

    // Send data back to main()
    processed = true;
    std::cout << "Worker thread signals data processing completed\n";

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();
}

void test_notify_one()
{
    std::thread worker(worker_thread);

    data = "Example data";
    // send data to the worker thread
    {
        std::lock_guard < std::mutex > lk(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();

    // wait for the worker
    {
        std::unique_lock < std::mutex > lk(m);
        cv.wait(lk, []
        {   return processed;});
    }
    std::cout << "Back in main(), data = " << data << '\n';

    worker.join();
}

std::condition_variable cv;
std::mutex cv_m; // This mutex is used for three purposes:
// 1) to synchronize accesses to i
// 2) to synchronize accesses to std::cerr
// 3) for the condition variable cv
int i = 0;

void waits()
{
    std::unique_lock < std::mutex > lk(cv_m);
    std::cerr << "Waiting... \n";
    cv.wait(lk, []
    {   return i == 1;});
    std::cerr << "...finished waiting. i == 1\n";
}

void signals()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard < std::mutex > lk(cv_m);
        std::cerr << "Notifying...\n";
    }
    cv.notify_all();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::lock_guard < std::mutex > lk(cv_m);
        i = 1;
        std::cerr << "Notifying again...\n";
    }
    cv.notify_all();
}

void test_notify_all()
{
    std::thread t1(waits), t2(waits), t3(waits), t4(signals);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

std::condition_variable cv;
std::mutex cv_m; // This mutex is used for three purposes:
// 1) to synchronize accesses to i
// 2) to synchronize accesses to std::cerr
// 3) for the condition variable cv
int i = 0;

void waits()
{
    std::unique_lock < std::mutex > lk(cv_m);
    std::cerr << "Waiting... \n";
    cv.wait(lk, []{   return i == 1;});//if lambada function is false, continue waiting.
    std::cerr << "...finished waiting. i == 1\n";
}

void signals()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard < std::mutex > lk(cv_m);
        std::cerr << "Notifying...\n";
    }
    cv.notify_all();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::lock_guard < std::mutex > lk(cv_m);
        i = 1;
        std::cerr << "Notifying again...\n";
    }
    cv.notify_all();
}

void test_wait()
{
    std::thread t1(waits), t2(waits), t3(waits), t4(signals);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

using namespace std::chrono_literals;

std::condition_variable cv;
std::mutex cv_m;
int i;

void waits(int idx)
{
    std::unique_lock < std::mutex > lk(cv_m);
    if (cv.wait_for(lk, idx * 100ms, []{   return i == 1;}))
        std::cerr << "Thread " << idx << " finished waiting. i == " << i << '\n';
    else
        std::cerr << "Thread " << idx << " timed out. i == " << i << '\n';
}

void signals()
{
    std::this_thread::sleep_for(120ms);
    std::cerr << "Notifying...\n";
    cv.notify_all();
    std::this_thread::sleep_for(100ms);
    {
        std::lock_guard < std::mutex > lk(cv_m);
        i = 1;
    }
    std::cerr << "Notifying again...\n";
    cv.notify_all();
}

void test_wait_for()
{
    std::thread t1(waits, 1), t2(waits, 2), t3(waits, 3), t4(signals);
    t1.join();
    t2.join(), t3.join(), t4.join();
}

using namespace std::chrono_literals;

std::condition_variable cv;
std::mutex cv_m;
std::atomic<int> i { 0 };

void waits(int idx)
{
    std::unique_lock < std::mutex > lk(cv_m);
    auto now = std::chrono::system_clock::now();
    if (cv.wait_until(lk, now + idx * 100ms, []()
    {   return i == 1;}))
        std::cerr << "Thread " << idx << " finished waiting. i == " << i << '\n';
    else
        std::cerr << "Thread " << idx << " timed out. i == " << i << '\n';
}

void signals()
{
    std::this_thread::sleep_for(120ms);
    std::cerr << "Notifying...\n";
    cv.notify_all();
    std::this_thread::sleep_for(100ms);
    i = 1;
    std::cerr << "Notifying again...\n";
    cv.notify_all();
}

void test_wait_until()
{
    std::thread t1(waits, 1), t2(waits, 2), t3(waits, 3), t4(signals);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

/*
 * The condition_variable_any class is a generalization of std::condition_variable.
 * Whereas std::condition_variable works only on std::unique_lock<std::mutex>, condition_variable_any can operate on any lock
 *
 * */


std::mutex m;
std::condition_variable cv;

bool ready = false;
ComplexType result;  // some arbitrary type

void thread_func()
{
    std::unique_lock<std::mutex> lk(m);
    // assign a value to result using thread_local data
    result = function_that_uses_thread_locals();
    ready = true;
    std::notify_all_at_thread_exit(cv, std::move(lk));
} // 1. destroy thread_locals, 2. unlock mutex, 3. notify cv


void test_notify_all_at_thread_exit()
{
    std::thread t(thread_func);
    t.detach();

    // do other work
    // ...

    // wait for the detached thread
    std::unique_lock<std::mutex> lk(m);
    while(!ready) {
        cv.wait(lk);
    }
    process(result); // result is ready and thread_local destructors have finished
}


int main()
{
    test_notify_one();
    test_notify_all();
    test_wait();
    test_wait_for();
    test_wait_until();
    test_notify_all_at_thread_exit();
}
