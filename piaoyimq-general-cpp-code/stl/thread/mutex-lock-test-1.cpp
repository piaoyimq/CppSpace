#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <functional>


int g_num = 0;  // protected by g_num_mutex
std::mutex g_num_mutex;

void slow_increment(int id)
{
	for (int i = 0; i < 3; ++i)
	{
		g_num_mutex.lock();
		++g_num;
		std::cout << id << " => " << g_num << '\n';
		g_num_mutex.unlock();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void test_lock_and_unlock()
{
	std::thread t1(slow_increment, 0);
	std::thread t2(slow_increment, 1);
	t1.join();
	t2.join();
}

std::map<std::string, std::string> g_pages;
std::mutex g_pages_mutex;

void save_page(const std::string &url)
{
	// simulate a long page fetch
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::string result = "fake content";

	std::lock_guard < std::mutex > guard(g_pages_mutex);
	g_pages[url] = result;
}

void test_lock_guard()
{
	std::thread t1(save_page, "http://foo");
	std::thread t2(save_page, "http://bar");
	t1.join();
	t2.join();

	// safe to access g_pages without lock now, as the threads are joined
	for (const auto &pair : g_pages)
	{
		std::cout << pair.first << " => " << pair.second << '\n';
	}
}

#include <mutex>
#include <thread>

struct bank_account
{
	explicit bank_account(int balance) :
			balance(balance)
	{
	}
	int balance;
	std::mutex m;
};

void transfer(bank_account &from, bank_account &to, int amount)
{
#if 1
	// lock both mutexes without deadlock
	std::lock(from.m, to.m);
	// make sure both already-locked mutexes are unlocked at the end of scope

	std::lock_guard < std::mutex > lock1(from.m, std::adopt_lock);  //adopt_lock: assume the calling thread already has ownership of the mutex
	std::lock_guard < std::mutex > lock2(to.m, std::adopt_lock);
#else
// equivalent approach:
	std::unique_lock<std::mutex> lock1(from.m, std::defer_lock);
	std::unique_lock<std::mutex> lock2(to.m, std::defer_lock);//defer_lock: do not acquire ownership of the mutex
	std::lock(lock1, lock2);
#endif

	from.balance -= amount;
	to.balance += amount;
}

void test_std_lock_by_lock_guard_or_unique_lock()
{
	bank_account my_account(100);
	bank_account your_account(50);

	std::thread t1(transfer, std::ref(my_account), std::ref(your_account), 10);
	std::thread t2(transfer, std::ref(your_account), std::ref(my_account), 5);

	t1.join();
	t2.join();
}

std::mutex mtx;           // mutex for critical section

void print_block(int n, char c)
{
	// critical section (exclusive access to std::cout signaled by lifetime of lck):
	std::unique_lock < std::mutex > lck(mtx);
	for (int i = 0; i < n; ++i)
	{
		std::cout << c;
	}
	std::cout << '\n';
}

void test_unique_lock()
{
	std::thread th1(print_block, 50, '*');
	std::thread th2(print_block, 50, '$');

	th1.join();
	th2.join();

	return 0;
}

std::chrono::milliseconds interval(100);

std::mutex mutex;
int job_shared = 0; // both threads can modify 'job_shared',
// mutex will protect this variable

int job_exclusive = 0; // only one thread can modify 'job_exclusive'
// no protection needed

// this thread can modify both 'job_shared' and 'job_exclusive'
void job_1()
{
	std::this_thread::sleep_for(interval); // let 'job_2' take a lock

	while (true)
	{
		// try to lock mutex to modify 'job_shared'
		if (mutex.try_lock())
		{
			std::cout << "job shared (" << job_shared << ")\n";
			mutex.unlock();
			return;
		}
		else
		{
			// can't get lock to modify 'job_shared'
			// but there is some other work to do
			++job_exclusive;
			std::cout << "job exclusive (" << job_exclusive << ")\n";
			std::this_thread::sleep_for(interval);
		}
	}
}

// this thread can modify only 'job_shared'
void job_2()
{
	mutex.lock();
	std::this_thread::sleep_for(5 * interval);
	++job_shared;
	mutex.unlock();
}

void test_try_lock()
{
	std::thread thread_1(job_1);
	std::thread thread_2(job_2);

	thread_1.join();
	thread_2.join();
}

std::mutex cout_mutex; // control access to std::cout
std::timed_mutex mutex_time;

void job(int id)
{
	using Ms = std::chrono::milliseconds;
	std::ostringstream stream;

	for (int i = 0; i < 3; ++i)
	{
		if (mutex_time.try_lock_for(Ms(100))) //wait to get the lock until timeout.
		{
			stream << "success ";
			std::this_thread::sleep_for(Ms(100));
			mutex_time.unlock();
		}
		else
		{
			stream << "failed ";
		}
		std::this_thread::sleep_for(Ms(100));
	}

	std::lock_guard < std::mutex > lock(cout_mutex);
	std::cout << "[" << id << "] " << stream.str() << "\n";
}

void test_timed_mutex()
{
	std::vector<std::thread> threads;
	for (int i = 0; i < 1; ++i)
	{
		threads.emplace_back(job, i);
	}

	for (auto& i : threads)
	{
		i.join();
	}
}

void f()
{
	auto now = std::chrono::steady_clock::now();
	test_mutex.try_lock_until(now + std::chrono::seconds(10));
	std::cout << "hello world\n";
}

void test_try_lock_until()
{
	std::lock_guard < std::timed_mutex > l(test_mutex);
	std::thread t(f);
	t.join();
}

struct Employee
{
	Employee(std::string id) :
			id(id)
	{
	}
	std::string id;
	std::vector<std::string> lunch_partners;
	std::mutex m;
	std::string output() const
	{
		std::string ret = "Employee " + id + " has lunch partners: ";
		for (const auto& partner : lunch_partners)
			ret += partner + " ";
		return ret;
	}
};

void send_mail(Employee &, Employee &)
{
	// simulate a time-consuming messaging operation
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

void assign_lunch_partner(Employee &e1, Employee &e2)
{
	static std::mutex io_mutex;
	{
		std::lock_guard < std::mutex > lk(io_mutex);
		std::cout << e1.id << " and " << e2.id << " are waiting for locks" << std::endl;
	}

	// use std::lock to acquire two locks without worrying about
	// other calls to assign_lunch_partner deadlocking us
	{
		std::lock(e1.m, e2.m);	//std::lock must combine the lock_guard or unique_lock to use, out of the scope, it will unlock.
		std::lock_guard < std::mutex > lk1(e1.m, std::adopt_lock);
		std::lock_guard < std::mutex > lk2(e2.m, std::adopt_lock);
// Equivalent code (if unique_locks are needed, e.g. for condition variables)
//        std::unique_lock<std::mutex> lk1(e1.m, std::defer_lock);
//        std::unique_lock<std::mutex> lk2(e2.m, std::defer_lock);
//        std::lock(lk1, lk2);
		{
			std::lock_guard < std::mutex > lk(io_mutex);
			std::cout << e1.id << " and " << e2.id << " got locks" << std::endl;
		}
		e1.lunch_partners.push_back(e2.id);
		e2.lunch_partners.push_back(e1.id);
	}
	send_mail(e1, e2);
	send_mail(e2, e1);
}

void test_std_lock()
{
	Employee alice("alice"), bob("bob"), christina("christina"), dave("dave");

	// assign in parallel threads because mailing users about lunch assignments
	// takes a long time
	std::vector<std::thread> threads;
	threads.emplace_back(assign_lunch_partner, std::ref(alice), std::ref(bob));
	threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(bob));
	threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(alice));
	threads.emplace_back(assign_lunch_partner, std::ref(dave), std::ref(bob));

	for (auto &thread : threads)
		thread.join();
	std::cout << alice.output() << '\n' << bob.output() << '\n' << christina.output() << '\n' << dave.output() << '\n';
}

std::shared_timed_mutex m;
int i = 10;

void read()
{
	// both the threads get access to the integer i
	std::shared_lock < std::shared_timed_mutex > slk(m);
	std::cout << "read i as " << i << "...\n"; // this is not synchronized
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::cout << "woke up...\n";
}

void test_shared_lock()
{
	std::thread r1(read);
	std::thread r2(read);

	r1.join();
	r2.join();
	return 0;
}

class ThreadSafeCounter
{
public:
	ThreadSafeCounter() = default;

	// Multiple threads/readers can read the counter's value at the same time.
	unsigned int get() const
	{
		std::shared_lock < std::shared_mutex > lock(mutex_);
		return value_;
	}

	// Only one thread/writer can increment/write the counter's value.
	void increment()
	{
		std::unique_lock < std::shared_mutex > lock(mutex_);
		value_++;
	}

	// Only one thread/writer can reset/write the counter's value.
	void reset()
	{
		std::unique_lock < std::shared_mutex > lock(mutex_);
		value_ = 0;
	}

private:
	mutable std::shared_mutex mutex_;
	unsigned int value_ = 0;
};


void test_shared_mutex()
{

	// Explanation: The output below was generated on a single-core machine. When
	// thread1 starts, it enters the loop for the first time and calls increment()
	// followed by get(). However, before it can print the returned value to
	// std::cout, the scheduler puts thread1 to sleep and wakes up thread2, which
	// obviously has time enough to run all three loop iterations at once. Back to
	// thread1, still in the first loop iteration, it finally prints its local copy
	// of the counter's value, which is 1, to std::cout and then runs the remaining
	// two loop iterations. On a multi-core maschine, none of the threads is put to
	// sleep and the output is more likely to be in ascending order.

	ThreadSafeCounter counter;

	auto increment_and_print = [&counter]()
	{
		for (int i = 0; i < 3; i++)
		{
			counter.increment();
			std::cout << std::this_thread::get_id() << ' ' << counter.get() << '\n';

			// Note: Writing to std::cout actually needs to be synchronized as well
			// by another std::mutex. This has been omitted to keep the example small.
		}
	};

	std::thread thread1(increment_and_print);
	std::thread thread2(increment_and_print);

	thread1.join();
	thread2.join();
}

int g_i = 0;
std::mutex g_i_mutex;  // protects g_i

void safe_increment()
{
    std::scoped_lock lock{g_i_mutex};
    ++g_i;

    std::cout << std::this_thread::get_id() << ": " << g_i << '\n';

    // g_i_mutex is automatically released when lock
    // goes out of scope
}

void test_scope_lock()
{
    std::cout << __func__ << ": " << g_i << '\n';

    std::thread t1(safe_increment);
    std::thread t2(safe_increment);

    t1.join();
    t2.join();

    std::cout << __func__ << ": " << g_i << '\n';
}

std::once_flag flag1, flag2;

void simple_do_once()
{
    std::call_once(flag1, [](){ std::cout << "Simple example: called once\n"; });
}

void may_throw_function(bool do_throw)
{
  if (do_throw) {
    std::cout << "throw: call_once will retry\n"; // this may appear more than once
    throw std::exception();//throw exception, means called failed.
  }
  std::cout << "Didn't throw, call_once will not attempt again\n"; // guaranteed once
}

void do_once(bool do_throw)
{
  try {
    std::call_once(flag2, may_throw_function, do_throw);
  }
  catch (...) {
  }
}

void test_call_once()
{
    std::thread st1(simple_do_once);
    std::thread st2(simple_do_once);
    std::thread st3(simple_do_once);
    std::thread st4(simple_do_once);
    st1.join();
    st2.join();
    st3.join();
    st4.join();//only call once, in all thread.

    std::thread t1(do_once, true);
    std::thread t2(do_once, true);
    std::thread t3(do_once, false);//only call once, it called failed, it will retry, until this funcion called successfully.
    std::thread t4(do_once, true);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}


void test_std_try_lock()
{
    int foo_count = 0;
    std::mutex foo_count_mutex;
    int bar_count = 0;
    std::mutex bar_count_mutex;
    int overall_count = 0;
    bool done = false;
    std::mutex done_mutex;

    auto increment = [](int &counter, std::mutex &m,  const char *desc) {
        for (int i = 0; i < 10; ++i) {
            std::unique_lock<std::mutex> lock(m);
            ++counter;
            std::cout << desc << ": " << counter << '\n';
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    std::thread increment_foo(increment, std::ref(foo_count),
        std::ref(foo_count_mutex), "foo");
    std::thread increment_bar(increment, std::ref(bar_count),
        std::ref(bar_count_mutex), "bar");

    std::thread update_overall([&]() {
        done_mutex.lock();
        while (!done) {
            done_mutex.unlock();
            int result = std::try_lock(foo_count_mutex, bar_count_mutex);
            if (result == -1) {
                overall_count += foo_count + bar_count;
                foo_count = 0;
                bar_count = 0;
                std::cout << "overall: " << overall_count << '\n';
                foo_count_mutex.unlock();
                bar_count_mutex.unlock();
            }
            std::this_thread::sleep_for(std::chrono::seconds(2));
            done_mutex.lock();
        }
        done_mutex.unlock();
    });

    increment_foo.join();
    increment_bar.join();
    done_mutex.lock();
    done = true;
    done_mutex.unlock();
    update_overall.join();

    std::cout << "Done processing\n"
              << "foo: " << foo_count << '\n'
              << "bar: " << bar_count << '\n'
              << "overall: " << overall_count << '\n';
}

int main()
{
//	test_lock_and_unlock();
//	test_lock_guard();
	test_unique_lock();
	test_std_lock_by_lock_guard_or_unique_lock();
//	test_try_lock();
	test_timed_mutex();
	test_try_lock_until();
	test_std_lock();
	test_std_try_lock();
	test_shared_lock();
	test_shared_mutex();
	test_scope_lock();

}
