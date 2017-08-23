#include <vector>
#include <cmath>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>

void accumulate(std::vector<int>::iterator first, std::vector<int>::iterator last, std::promise<int> accumulate_promise)
{
    int sum = std::accumulate(first, last, 0);
    accumulate_promise.set_value(sum);  // Notify future
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << __FUNCTION__ << std::endl;
}

void do_work(std::promise<void> barrier)
{
    barrier.set_value();
    std::cout << "do_work\n";
}

void test_promise_1()
{
    // Demonstrate using promise<int> to transmit a result between threads.
    std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
    std::promise<int> accumulate_promise;
    std::future<int> accumulate_future = accumulate_promise.get_future();

    std::thread work_thread(accumulate, numbers.begin(), numbers.end(), std::move(accumulate_promise));
    accumulate_future.wait();  // wait for result
    std::cout << "result=" << accumulate_future.get() << '\n';
    work_thread.join();  // wait for thread completion

    // Demonstrate using promise<void> to signal state between threads.
//    std::promise<void> barrier;
//    std::future<void> barrier_future = barrier.get_future();
//    std::thread new_work_thread(do_work, std::move(barrier));
//    barrier_future.wait();  //???
//    new_work_thread.join();

    std::cout << "Main thread exit\n";
}

void Thread_Fun1(std::promise<int> &p)
{
    //为了突出效果，可以使线程休眠5s
    std::this_thread::sleep_for(std::chrono::seconds(1));

    int iVal = 233;
    std::cout << "set value int)：" << iVal << std::endl;

    //传入数据iVal
    p.set_value(iVal);
}

void Thread_Fun2(std::future<int> &f)
{
    //阻塞函数，直到收到相关联的std::promise对象传入的数据
    auto iVal = f.get();        //iVal = 233

    std::cout << "get value(int)：" << iVal << std::endl;
}

void test_promise_2()
{
    //声明一个std::promise对象pr1，其保存的值类型为int
    std::promise<int> pr1;
    //声明一个std::future对象fu1，并通过std::promise的get_future()函数与pr1绑定
    std::future<int> fu1 = pr1.get_future();

    //创建一个线程t1，将函数Thread_Fun1及对象pr1放在线程里面执行
    std::thread t1(Thread_Fun1, std::ref(pr1));
    //创建一个线程t2，将函数Thread_Fun2及对象fu1放在线程里面执行
    std::thread t2(Thread_Fun2, std::ref(fu1));

    //阻塞至线程结束
    t1.join();
    t2.join();
    std::cout << "Main thread exit\n";
}

void test_promise_set_exception()
{
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t([&p]
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        try
        {
            // code that may throw
            throw std::runtime_error("Example");
        }
        catch(...)
        {
            try
            {
                // store anything thrown in the promise
                p.set_exception(std::current_exception());
            }
            catch(...)
            {} // set_exception() may throw too
        }
    });

    try
    {
        std::cout << f.get();
    } catch (const std::exception& e)
    {
        std::cout << "Exception from the thread: " << e.what() << '\n';
    }

    t.join();
    std::cout << "Main thread exit\n";
}

void test_promise_set_value_at_thread_exit()
{
//    using namespace std::chrono_literals;
//    std::promise<int> p;
//    std::future<int> f = p.get_future();
//    std::thread([&p]
//    {
//        std::this_thread::sleep_for(1s);
//        p.set_value_at_thread_exit(9);
//    }).detach();
//
//    std::cout << "Waiting..." << std::flush;
//    f.wait();
//    std::cout << "Done!\nResult is: " << f.get() << '\n';
}

template<typename RAIter>
int parallel_sum(RAIter beg, RAIter end)
{
    auto len = end - beg;
    std::cout << "len=" << len << std::endl;
    if (len < 1000)
        return std::accumulate(beg, end, 0);

    RAIter mid = beg + len / 2;
    auto handle = std::async(std::launch::async, parallel_sum<RAIter>, mid, end);
    int sum = parallel_sum(beg, mid);
    return sum + handle.get();
}

void test_std_async()
{
    std::vector<int> v(10000, 1);
    std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';
}

// unique function to avoid disambiguating the std::pow overload set
int f(int x, int y)
{
    return std::pow(x, y);
}

void task_lambda()
{
    std::packaged_task<int(int, int)> task([](int a, int b)
    {
        return std::pow(a, b);
    });
    std::future<int> result = task.get_future();

    task(2, 9);

    std::cout << "task_lambda:\t" << result.get() << '\n';
}

void task_bind()
{
    std::packaged_task < int() > task(std::bind(f, 2, 11));
    std::future<int> result = task.get_future();

    task();

    std::cout << "task_bind:\t" << result.get() << '\n';
}

void task_thread()
{
    std::packaged_task<int(int, int)> task(f);
    std::future<int> result = task.get_future();

    std::thread task_td(std::move(task), 2, 10);
    task_td.join();

    std::cout << "task_thread:\t" << result.get() << '\n';
}

void test_packaged_task()
{
    task_lambda();
    task_bind();
    task_thread();
}

void worker(std::future<void>& output)
{
    std::packaged_task<void(bool&)> my_task { [](bool& done)
    {   done=true;} };

    auto result = my_task.get_future();

    bool done = false;

    my_task.make_ready_at_thread_exit(done); // execute task right away

    std::cout << "worker: done = " << std::boolalpha << done << std::endl;

    auto status = result.wait_for(std::chrono::seconds(0));
    if (status == std::future_status::timeout)
        std::cout << "worker: result is not ready yet" << std::endl;

    output = std::move(result);
}

void test_packaged_task_make_ready_at_thread_exit()
{
    std::future<void> result;

    std::thread { worker, std::ref(result) }.join();

    auto status = result.wait_for(std::chrono::seconds(0));
    if (status == std::future_status::ready)
        std::cout << "main: result is ready" << std::endl;
}

std::string time()
{
    static auto start = std::chrono::steady_clock::now();
    std::chrono::duration<double> d = std::chrono::steady_clock::now() - start;
    return "[" + std::to_string(d.count()) + "s]";
}

void test_future_get()
{
    using namespace std::chrono_literals;
    {
        std::cout << time() << " launching thread\n";
        std::future<int> f = std::async(std::launch::async, []
        {
            std::this_thread::sleep_for(1s);
            return 7;
        });

        std::cout << time() << " waiting for the future, f.valid() == " << f.valid() << "\n";
        int n = f.get(); //did get() include wait() operation ?
        std::cout << time() << " future.get() returned with " << n << ". f.valid() = " << f.valid() << '\n';
    }

    {
        std::cout << time() << " launching thread\n";
        std::future<int> f = std::async(std::launch::async, []
        {
            std::this_thread::sleep_for(1s);
            return true ? throw std::runtime_error("7") : 7;
        });

        std::cout << time() << " waiting for the future, f.valid() == " << f.valid() << "\n";

        try
        {
            int n = f.get();
            std::cout << time() << " future.get() returned with " << n << " f.valid() = " << f.valid() << '\n';
        } catch (const std::exception& e)
        {
            std::cout << time() << " caught exception " << e.what() << ", f.valid() == " << f.valid() << "\n";
        }
    }
}

int fib(int n)
{
    if (n < 3)
        return 1;
    else
        return fib(n - 1) + fib(n - 2);
}

void test_future_wiat()
{
#if 1
    std::future<int> f1 = std::async(std::launch::async, []()
    {
        return fib(20);
    });
    std::future<int> f2 = std::async(std::launch::async, []()
    {
        return fib(25);
    });

#else   //equal  shared_future
    std::shared_future<int> f1 = std::async(std::launch::async, []()
    {
        return fib(20);
    });
    std::shared_future<int> f2 = std::async(std::launch::async, []()
    {
        return fib(25);
    });
#endif


    std::cout << "waiting...\n";
    f1.wait();//why use wait(), could delete it ?
    f2.wait();

    std::cout << "f1: " << f1.get() << '\n';
    std::cout << "f2: " << f2.get() << '\n';
}


void test_future_wiat_for()
{
    std::future<int> future = std::async(std::launch::async, []()
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 8;
    });

    std::cout << "waiting...\n";
    std::future_status status;
    do
    {
        status = future.wait_for(std::chrono::seconds(1));
        if (status == std::future_status::deferred)
        {
            std::cout << "deferred\n";
        }
        else
            if (status == std::future_status::timeout)
            {
                std::cout << "timeout\n";
            }
            else
                if (status == std::future_status::ready)
                {
                    std::cout << "ready!\n";
                }
    } while (status != std::future_status::ready);

    std::cout << "result is " << future.get() << '\n';
}


void test_future_wait_until()
{
    std::chrono::system_clock::time_point two_seconds_passed
        = std::chrono::system_clock::now() + std::chrono::seconds(2);

    // Make a future that that takes 1 second to completed
    std::promise<int> p1;
    std::future<int> f_completes = p1.get_future();
    std::thread([](std::promise<int> p1)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    p1.set_value_at_thread_exit(9);
                },
                std::move(p1)
    ).detach();

    // Make a future that that takes 5 seconds to completed
    std::promise<int> p2;
    std::future<int> f_times_out = p2.get_future();
    std::thread([](std::promise<int> p2)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    p2.set_value_at_thread_exit(8);
                },
                std::move(p2)
    ).detach();

    std::cout << "Waiting for 2 seconds..." << std::endl;

    if(std::future_status::ready == f_completes.wait_until(two_seconds_passed))
        { std::cout << "f_completes: " << f_completes.get() << "\n"; }
    else
        { std::cout << "f_completes did not complete!\n"; }

    if(std::future_status::ready == f_times_out.wait_until(two_seconds_passed))
        { std::cout << "f_times_out: " << f_times_out.get() << "\n"; }
    else
        { std::cout << "f_times_out did not complete!\n"; }

    std::cout << "Done!\n";
}

void test_future()
{
    // future from a packaged_task
    std::packaged_task<int()> task([](){ return 7; }); // wrap the function
    std::future<int> f1 = task.get_future();  // get a future
    std::thread(std::move(task)).detach(); // launch on a thread

    // future from an async()
    std::future<int> f2 = std::async(std::launch::async, [](){ return 8; });

    // future from a promise
    std::promise<int> p;
    std::future<int> f3 = p.get_future();
    std::thread( [&p]{ p.set_value_at_thread_exit(9); }).detach();

    std::cout << "Waiting..." << std::flush;
    f1.wait();//could delete it ?
    f2.wait();
    f3.wait();
    std::cout << "Done!\nResults are: "
              << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
}

void test_shared_future()
{
    std::promise<void> ready_promise, t1_ready_promise, t2_ready_promise;
    std::shared_future<void> ready_future(ready_promise.get_future());

    std::chrono::time_point<std::chrono::high_resolution_clock> start;

    auto fun1 = [&, ready_future]() -> std::chrono::duration<double, std::milli>
    {
        t1_ready_promise.set_value();
        ready_future.wait(); // waits for the signal from main()
        return std::chrono::high_resolution_clock::now() - start;
    };


    auto fun2 = [&, ready_future]() -> std::chrono::duration<double, std::milli>
    {
        t2_ready_promise.set_value();
        ready_future.wait(); // waits for the signal from main()
        return std::chrono::high_resolution_clock::now() - start;
    };

    auto result1 = std::async(std::launch::async, fun1);
    auto result2 = std::async(std::launch::async, fun2);

    // wait for the threads to become ready
    t1_ready_promise.get_future().wait();
    t2_ready_promise.get_future().wait();

    // the threads are ready, start the clock
    start = std::chrono::high_resolution_clock::now();

    // signal the threads to go
    ready_promise.set_value();
    std::cout <<  static_cast<std::chrono::duration<double, std::milli>>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;
    std::cout << "Thread 1 received the signal "
              << result1.get().count() << " ms after start\n"
              << "Thread 2 received the signal "
              << result2.get().count() << " ms after start\n";
}

int main()
{
    test_promise_1();
    test_promise_2();
    test_promise_set_exception();
    test_promise_set_value_at_thread_exit();

    test_std_async();

    test_packaged_task();
    test_packaged_task_make_ready_at_thread_exit();

    test_future();
    test_future_get();
    test_future_wiat();
    test_future_wiat_for();
    test_future_wait_until();

    test_shared_future();
}
