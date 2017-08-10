#include <iostream>
#include <chrono>
#include <ratio>
#include <thread>

void f()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    f();
    auto t2 = std::chrono::high_resolution_clock::now();

    // integral duration: requires duration_cast
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    // fractional duration: no duration_cast needed
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

    std::cout << "f() took " << fp_ms.count() << " ms, "
              << "or " << int_ms.count() << " whole milliseconds\n";
}
