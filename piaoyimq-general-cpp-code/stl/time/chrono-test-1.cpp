#include <iostream>
#include <chrono>
#include <ratio>
#include <thread>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <locale>


void test_time_t_1()
{
    std::cout <<"\nFunction=" <<__FUNCTION__<<std::endl;
    std::time_t result = std::time(nullptr);
    std::cout << std::asctime(std::localtime(&result))<< result << " seconds since the Epoch\n";
}


void test_time_t_2()
{
    std::cout <<"\nFunction=" <<__FUNCTION__<<std::endl;
    std::time_t t = std::time(nullptr);
    std::cout << "UTC:   " << std::put_time(std::gmtime(&t), "%c %Z") << '\n';
    std::cout << "local: " << std::put_time(std::localtime(&t), "%c %Z") << '\n';
}

void test_time_t_3()
{
    std::cout <<"\nFunction=" <<__FUNCTION__<<std::endl;
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::cout.imbue(std::locale("ru_RU.utf8"));
    std::cout << "ru_RU: " << std::put_time(&tm, "%c %Z") << '\n';

    std::cout.imbue(std::locale("ja_JP.utf8"));
    std::cout << "ja_JP: " << std::put_time(&tm, "%c %Z") << '\n';
}


void test_time_t_4()
{
    std::cout <<"\nFunction=" <<__FUNCTION__<<std::endl;
    std::time_t result = std::time(nullptr);
    std::cout <<"ctime=" << std::ctime(&result);
    std::cout << "asctime=" <<std::asctime(std::localtime(&result));

    std::locale::global(std::locale("ja_JP.utf8"));
    char mbstr[100];
    if (std::strftime(mbstr, sizeof(mbstr), "%A %c", std::gmtime(&result)))
    {
        std::cout <<"strftime=" << mbstr << '\n';
    }

    std::locale::global(std::locale("ja_JP.utf8"));
    wchar_t wstr[100];
    if(std::wcsftime(wstr, 100, L"%A %c", std::localtime(&result)))
    {
        std::wcout << "wcsftime=" <<wstr << '\n';
    }
}


void test_time_t_5()
{
    std::cout <<"\nFunction=" <<__FUNCTION__<<std::endl;
    std::tm t = {};
    std::istringstream ss("2011-Februar-18 23:12:34");
    ss.imbue(std::locale("de_DE.utf-8"));
    ss >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");
    if (ss.fail())
    {
        std::cout << "Parse failed\n";
    }
    else
    {
        std::cout << std::put_time(&t, "%c") << '\n';
    }
}


void test_time_t_and_chrono_convert()
{
    std::cout <<"\nFunction=" <<__FUNCTION__<<std::endl;
    using namespace std::chrono;
    auto now = system_clock::now();
    time_t t2 = system_clock::to_time_t(now);//to_time_t
    std::tm tm = *std::localtime(&t2);
    auto time = std::put_time(&tm, "%b %d %Y %H:%M:%S");
    std::cout << time << std::endl;


    std::stringstream ss;
    ss << time;
    ss >> std::get_time(&tm, "%b %d %Y %H:%M:%S");
    auto now2 = std::chrono::system_clock::from_time_t(std::mktime(&tm)); //time_t ==>time_point
    duration<double> diff = now - now2;
    std::cout << diff.count() << std::endl;

}
void f()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void test_chrono_1()
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



void test_chrono_2()
{
    using shakes = std::chrono::duration<int, std::ratio<1, 100000000>>;
    using jiffies = std::chrono::duration<int, std::centi>;
    using microfortnights = std::chrono::duration<float, std::ratio<12096,10000>>;
    using nanocenturies = std::chrono::duration<float, std::ratio<3155,1000>>;

    std::chrono::seconds sec(1);
    std::chrono::minutes min(1);
    std::chrono::hours hour(1);



    // integer scale conversion with no precision loss: no cast
    std::cout << "1 second is: \n";
    std::cout << shakes(sec).count() << " shakes\n"<< jiffies(sec).count() << " jiffies\n";
    std::cout << std::chrono::milliseconds(sec).count() << "ms\n";
    std::cout << std::chrono::microseconds(sec).count() << "us\n";
//    std::cout << std::chrono::minutes(sec).count() << "min\n";  //could not up convert

    std::cout << "1min is:  \n";
    std::cout <<std::chrono::milliseconds(min).count() << "ms\n";
    std::cout <<std::chrono::seconds(min).count() << "sec\n";
//    std::cout <<std::chrono::hour(min).count() << "h\n";

    std::cout << "1h is:  \n";
    std::cout <<std::chrono::minutes(hour).count() << "min\n";
    std::cout <<std::chrono::seconds(hour).count() << "sec\n";

    //add
    std::cout <<"hour+min+sec="<< std::chrono::duration_cast<std::chrono::seconds>(hour+min+sec).count()<< " s\n";
    std::cout <<"hour+min+sec="<< std::chrono::duration_cast<std::chrono::hours>(hour+min+sec).count()<< "h\n";

    // integer scale conversion with precision loss: requires a cast
    std::cout << std::chrono::duration_cast<std::chrono::minutes>(sec).count()<< " minutes\n";
    std::cout << std::chrono::duration_cast<std::chrono::hours>(sec).count()<< " hours\n";

    // floating-point scale conversion: no cast
    std::cout << microfortnights(sec).count() << " microfortnights\n"<< nanocenturies(sec).count() << " nanocenturies\n";
}

void test_chrono_3()
{
#if 0//until c++17
    using namespace std::chrono_literals;
    auto day = 24h;
    auto halfhour = 0.5h;
    std::cout << "one day is " << day.count() << " hours\n" << "half an hour is " << halfhour.count() << " hours\n";

    auto lesson = 45min;
    auto halfmin = 0.5min;
    std::cout << "one lesson is " << lesson.count() << " minutes\n" << "half a minute is " << halfmin.count() << " minutes\n";

    auto halfmin = 30s;
    std::cout << "half a minute is " << halfmin.count() << " seconds\n" << "a minute and a half is " << (1min + 30s).count() << " seconds\n";

    auto d1 = 250ms;
    std::chrono::milliseconds d2 = 1s;
    std::cout << "250ms = " << d1.count() << " milliseconds\n" << "1s = " << d2.count() << " milliseconds\n";

    auto d1 = 250us;
    std::chrono::microseconds d2 = 1ms;
    std::cout << "250us = " << d1.count() << " microseconds\n" << "1ms = " << d2.count() << " microseconds\n";

    auto d1 = 250ns;
    std::chrono::nanoseconds d2 = 1us;
    std::cout << "250ns = " << d1.count() << " nanoseconds\n" << "1us = " << d2.count() << " nanoseconds\n";
#endif
}


void test_chrono_time_point_1()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));
    std::cout << "24 hours ago, the time was "<< std::put_time(std::localtime(&now_c), "%F %T") << '\n';

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::cout << "Hello World\n";
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Printing took "<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()<< "us.\n";
}

int main()
{
    //time_t
    test_time_t_1();
    test_time_t_2();
    test_time_t_3();
    test_time_t_4();
    test_time_t_5();

    test_time_t_and_chrono_convert();

    //chrono
    test_chrono_1();
    test_chrono_2();
    test_chrono_3();

    test_chrono_time_point_1();
}
