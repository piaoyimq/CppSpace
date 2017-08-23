#include <atomic>
#include <chrono>
#include <thread>
#include <list>
#include <utility>
#include <iostream>       // std::cout
#include <vector>         // std::vector

std::atomic<long> g_CountStd(0);
void std_atomic_fun()
{
    for (int i = 0; i < 10000000; i++)
    {
        ++g_CountStd;
    }
}

void test_atomic_1()
{
    std::list < std::thread > threadlist;

    std::cout << "testing std::atomic...\n";
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 16; ++i)
    {
        threadlist.emplace_back(std_atomic_fun);
    }

    for (auto& th : threadlist)
    {
        th.join();
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "result=" << g_CountStd << std::endl;
    std::cout << "cost=" << std::chrono::duration_cast < std::chrono::milliseconds > (finish - start).count() << "ms" << std::endl;

}

std::atomic<bool> ready(false); //can be checked without being set
std::atomic_flag winner = ATOMIC_FLAG_INIT;  //make sure winner is clear, winner=false,  always set when checked

void count1m(int id)
{
    while (!ready)
    {
        std::this_thread::yield();
    }      // wait for the ready signal

    for (volatile int i = 0; i < 1000000; ++i)
    {
    }          // go!, count to 1 million

    // 如果某个线程率先执行完上面的计数过程，则输出自己的 ID.
    // 此后其他线程执行 test_and_set 是 if 语句判断为 false，
    // 因此不会输出自身 ID.
    if (!winner.test_and_set())
    {
        //test_and_set() 函数检查 std::atomic_flag 标志，如果 std::atomic_flag 之前没有被设置过，
        //则设置 std::atomic_flag 的标志，并返回先前该 std::atomic_flag 对象是否被设置过，如果之前 std::atomic_flag 对象已被设置，则返回 true，否则返回 false。

        std::cout << "thread #" << id << " won!\n";
    }

}

void test_atomic_flag()
{
    std::vector < std::thread > threads;
    std::cout << "spawning 10 threads that count to 1 million...\n";
    for (int i = 1; i <= 10; ++i)
    {
        threads.emplace_back(count1m, i);
    }

    ready = true;
    for (auto& th : threads)
    {
        th.join();
    }

}

struct A
{
    int a[100];
};

struct B
{
    int x, y;
};

void test_atomic_is_lock_free()
{
//    std::cout << std::boolalpha << "std::atomic<A> is lock free? " << std::atomic<A> { }.is_lock_free() << '\n' << "std::atomic<B> is lock free? "
//            << std::atomic<B> { }.is_lock_free() << '\n';
}

std::atomic<int> foo(0);

void set_foo(int x)
{
    foo.store(x, std::memory_order_relaxed);     // set value atomically, <==> foo=x;
}

void print_foo()
{
    int x = 0;
    do
    {
        std::cout << "____foo: " << x << '\n';
        x = foo.load(std::memory_order_relaxed);  // get value atomically
    } while (x == 0);
    std::cout << "foo: " << x << '\n';
}

void test_atomic_store_load()
{
    std::thread first(print_foo);
    std::thread second(set_foo, 10);
    first.join();
    second.join();
}

std::atomic<int> fooo(0);
std::atomic<int> barr(0);

void set_fooo(int x)
{
    fooo = x;
    std::cout <<__FUNCTION__<<std::endl;
}

void copy_foo_to_bar()
{
    while (fooo == 0)
    {
        std::this_thread::yield();
        std::cout <<__FUNCTION__<<std::endl;
    }
    barr = static_cast<int>(fooo); //error, barr = fooo;
}

void print_barr()
{
            while (barr == 0)
        {
            std::this_thread::yield();
            std::cout <<__FUNCTION__<<std::endl;
        }

    std::cout << "bar: " << barr << '\n';  // <==> barr.load();
}

void test_atomic_T()
{
    std::thread first2(print_barr);
    std::thread second2(set_fooo, 10);
    std::thread third(copy_foo_to_bar);
    first2.join();
    second2.join();
    third.join();
}

std::atomic<bool> ready2 (false);
std::atomic<bool> winner2 (false);

void count1m2 (int id) {
  while (!ready2) {}                  // wait for the ready signal
  for (int i=0; i<1000000; ++i) {}   // go!, count to 1 million
  if (!winner2.exchange(true)) { std::cout << "thread #" << id << " won!\n"; }
};

void test_atomic_exchange()
{
  std::vector<std::thread> threads;
  std::cout << "spawning 10 threads that count to 1 million...\n";
  for (int i=1; i<=10; ++i) threads.push_back(std::thread(count1m2,i));
  ready2 = true;
  for (auto& th : threads) th.join();

}

std::atomic<long long> data;
void do_add()
{
    data.fetch_add(1, std::memory_order_relaxed);
}

void do_sub()
{
    data.fetch_sub(1, std::memory_order_relaxed);
}

void do_increase()
{
    ++data;

    data-=4;
}

#include <chrono>
void test_fetch_add()
{
    std::thread th1(do_add);
    std::thread th2(do_add);
    std::thread th3(do_add);
    std::thread th4(do_add);


    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Result:" << data << '\n';
    std::thread th5(do_sub);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Result:" << data << '\n';

    std::thread th6(do_increase);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Result:" << data << '\n';

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
}

void test_init()
{
    std::atomic<int> a= ATOMIC_VAR_INIT(1);
    a=3;
    // std::atomic<int> a(1);   // C++-only alternative
    std::cout << "Initialized std::atomic<int> as: " << a << '\n';
}

int main()
{
//    test_atomic_1();
//    test_atomic_flag();
//    test_atomic_is_lock_free();
//    test_atomic_store_load();   //store()  <==> operator=(),   load()<==>operator T()
//    test_atomic_T();
//    test_atomic_exchange();
//    test_fetch_add();
    test_init();
}
