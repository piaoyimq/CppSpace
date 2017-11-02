#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <unistd.h>
/*
 good 等价于!eof()&&!fail()&&!bad(): y
operator bool等价于!fail()&&!bad()
判断文件是否打开成功用is_open()，
if(!fs)(即operator!) 无法检测到 ： 以不合适的文件模式打开文件失败,但is_open()能检查到。

由此可知：
多用
is_open():打开文件时检测是否成功打开
good()：读写文件时检测是否可读或可写，即判断是否到文件末尾。
 */


void test_clear_read_write()
{
	std::cout << "Function=" << __FUNCTION__ << std::endl;
	std::string filename = "test.bin";
//	std::fstream s(filename, s.binary | s.trunc | s.in | s.out);
	std::fstream s(filename, s.binary);
	if (!s.is_open()) //similar to  (!s), less use !s, more use is_open() here.
	{
		std::cout << "failed to open " << filename << '\n';

		std::cout << std::boolalpha;  //boolalpha，函数名称，功能是把bool值显示为true或false。
		std::cout << "!fs = " << !s << '\n';
		std::cout << "fs.is_open() = " << s.is_open() << '\n';  //<==> !fs<==>!fail()&&!bad()
		std::cout << "fs.good() = " << s.good() << '\n';//<==> !bad()&&!fail()&&!eof()
		std::cout << "fs.bad() = " << s.bad() << '\n';  // I/O error while reading
		std::cout << "fs.fail() = " << s.fail() << '\n';  //Non-integer data encountered
		std::cout << "fs.eof() = " << s.eof() << '\n'; //End of file reached successfully

		s.clear();

		std::cout << "After fs.clear()" << std::endl;
		std::cout << std::boolalpha;
		std::cout << "!fs = " << !s << '\n';
		std::cout << "fs.is_open() = " << s.is_open() << '\n';
		std::cout << "fs.good() = " << s.good() << '\n';
		std::cout << "fs.bad() = " << s.bad() << '\n';
		std::cout << "fs.fail() = " << s.fail() << '\n';
		std::cout << "fs.eof() = " << s.eof() << '\n';
	}
	else
	{
		// write
		double d = 3.14;
		s.write(reinterpret_cast<char*>(&d), sizeof d); // binary output
		s << 123 << "abc"; // text output

		// for fstream, this moves the file position pointer (both put and get)
		s.seekp(0);

		// read
		s.read(reinterpret_cast<char*>(&d), sizeof d); // binary input
		int n;
		std::string str;
		if (s >> n >> str) // text input
			std::cout << "read back from file: " << d << ' ' << n << ' ' << str << '\n';
	}
}

int test_2()
{
	std::cout << "Function=" << __FUNCTION__ << std::endl;
	std::ifstream file("test.txt");
	if (!file.is_open()) //similar to  (!s), less use !s, more use is_open() here.
	{
		std::cout << "File opening failed\n";
		return EXIT_FAILURE;
	}

	// typical C++ I/O loop uses the return value of the I/O function
	// as the loop controlling condition, operator bool() is used here
	for (int n; file >> n;)
	{
		std::cout << n << ' ';
	}
	std::cout << '\n';

	if (file.bad())
		std::cout << "I/O error while reading\n";
	else
		if (file.eof())
			std::cout << "End of file reached successfully\n";
		else
			if (file.fail())
				std::cout << "Non-integer data encountered\n";
}

void test_3()
{
	std::cout << "Function=" << __FUNCTION__ << std::endl;
	std::string filename = "example.123";

	std::fstream fs;

	fs.open(filename);

	if (!fs.is_open())
	{
		fs.clear();
		fs.open(filename, std::ios::out); //Create file.
		fs.close();
		fs.open(filename);
	}
	if(!fs.good())
	{
		std::cout <<"could not read or write";
		return;
	}
	std::cout << std::boolalpha;
	std::cout << "fs.is_open() = " << fs.is_open() << '\n';
	std::cout << "fs.good() = " << fs.good() << '\n';
	std::cout << "fs.bad() = " << fs.bad() << '\n';
	std::cout << "fs.fail() = " << fs.fail() << '\n';
	std::cout << "fs.eof() = " << fs.eof() << '\n';
	fs.close();
}

bool writeFile()
{
    std::fstream fs;
    fs.open("./disk-test", std::ios::ate | std::ios::in|std::ios::out);
    if(!fs.is_open())
    {
        std::cout << "____open failed";
        return false;
    }

    if(fs.good())
    {
        fs << "1";
        fs.close();
        std::cout <<"____write sucessfully.";
        return true;
    }
    else
    {
        std::cout <<"____write failed.";
        fs.close();
        return false;
    }
}

void diskMoniter()
{
    std::cout <<"____start diskMoniterThread.";
    bool isRaised = false;

    std::fstream fs("./disk-test", std::ios::trunc|std::ios::in|std::ios::out);
    if(!fs.is_open())
    {
        std::cout <<"____open failed.";
    }
    fs.close();

    int i=0;
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << i++<< std::endl;
        writeFile();
    }

    std::cout << "____diskMoniterThread exit.";
}

void test_4()
{
    std::thread diskMoniterThread(diskMoniter);
    diskMoniterThread.detach();
}


int main()
{
	test_clear_read_write();
	std::cout << std::endl;
	test_2();
	std::cout << std::endl;
	test_3();

    test_4();
    sleep(1000);
}
