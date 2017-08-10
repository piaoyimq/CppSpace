#include <iostream>
#include <chrono>

int main()
{
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
}
