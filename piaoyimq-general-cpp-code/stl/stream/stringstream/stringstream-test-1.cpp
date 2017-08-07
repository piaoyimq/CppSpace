#include <iostream>
#include <sstream>
#include <typeinfo> //typeid()
using namespace std;


int test_1()
{
	ostringstream ostr("first");
	std::cout <<ostr.str()<<std::endl;

	ostr.str("abccccccccccccccccccccccc");//will change the orginl data.
	std::cout <<ostr.str()<<std::endl;
	ostr<<"hello";
	ostr.put('d');//如果构造的时候设置了字符串参数,那么增长操作的时候不会从结尾开始增加,而是修改原有数据,超出的部分增长
	ostr.put('e');
	ostr << "fg";

	string gstr = ostr.str();
	cout << gstr<<std::endl;
	ostr.str("");
	ostr<<"good";

	cout << ostr.str()<<std::endl;


}


int test_2()
{
	// default constructor (input/output stream)
	std::stringstream buf1;
	buf1 << 7;
	int n = 0;
	buf1 >> n;
	std::cout << "buf1 = " << buf1.str() << " n = " << n << '\n';

	// input stream
	std::istringstream inbuf("-10");
	inbuf >> n;
	std::cout << "n = " << n << '\n';

	// output stream in append mode (C++11)
	std::ostringstream buf2("test", std::ios_base::ate);
	buf2 << '1';
	std::cout << buf2.str() << '\n';
}


template<class out_type, class in_value>
out_type convert(const in_value & t) {
    std::stringstream stream;
    stream << t;         //向流中传值
    out_type result;         //这里存储转换结果
    stream >> result;         //向result中写入值
    return result;
}


void convert_test()
{
	  double d, d2;
	    std::string salary, salary2;
	    std::string s = "12.56";
	    d = convert<double>(s);         //d等于12.56
	    d2 = d;
	    printf("d2=%f\n", d2);
	    std::cout << "d2: " << d2 << std::endl;

	    salary = convert<std::string>(9000.0);         //salary等于”9000”
	    salary2 = salary;
	    printf("salary2: %s\n", salary2.c_str());
	    std::cout << "typeid(d): " << typeid(d2).name() << std::endl;
	    std::cout << "typeid(salary2): " << typeid(salary2).name() << std::endl;
	    std::cout << "stringstream: " << typeid(std::stringstream).name() << std::endl;
}


void printAlarm(const int &id, const std::string &alarmType, std::stringstream& os) {
    os << "    Fault Id: " << id << std::endl;
    os << "    Alarm Major Type: " << alarmType << std::endl;
    os << "    Alarm Minor Type:  Unknown" << std::endl;
}


void test_3() {
    std::stringstream stringBuffer;
    printAlarm(10, "ActiveAlarm", stringBuffer);
    std::cout << "stringBuffer size:" << stringBuffer.str().size() << std::endl;
    std::cout << stringBuffer.str() << std::endl;         //stringBuffer.str().c_str(), is c language charaters.

    //stringstream is  used to data type convertsion.
    std::stringstream stream;
    std::string first, second;
    int i = 1000;
	std::cout << std::boolalpha;
	std::cout <<"line=" << __LINE__ <<std::endl;
	std::cout << "fs.good() = " << stream.good() << '\n';
	std::cout << "fs.bad() = " << stream.bad() << '\n';
	std::cout << "fs.fail() = " << stream.fail() << '\n';
	std::cout << "fs.eof() = " << stream.eof() << "\n\n";

    stream << i;
    stream >> first;         //从stream中抽取前面插入的int值
    std::cout << "first: " << first << std::endl;         // print the string "1000"

/*在C++中可以使用stringstream来很方便的进行类型转换，字符串串接，
 * 不过注意重复使用同一个stringstream对象时要 先继续清空，而清空很容易想到是clear方法，
 * 而在stringstream中这个方法实际上是清空stringstream的状态（比如出错等），真 正清空内容需要使用.str("")方法。
 *
 * 重复利用stringstream对象
如果你打算在多次转换中使用同一个stringstream对象，记住再每次转换前要使用clear()方法；
在多次转换中重复使用同一个stringstream（而不是每次都创建一个新的对象）对象最大的好处在于效率。stringstream对象的构造和析构函数通常是非常耗费CPU时间的。
 * */
	std::cout << std::boolalpha;
	std::cout <<"line=" << __LINE__ <<std::endl;
	std::cout << "fs.good() = " << stream.good() << '\n';
	std::cout << "fs.bad() = " << stream.bad() << '\n';
	std::cout << "fs.fail() = " << stream.fail() << '\n';
	std::cout << "fs.eof() = " << stream.eof() << "\n\n";

    stream.clear();//here, use clear(), to reset the flags
    stream.str("");//在进行多次转换前，必须清除stream, use clear(), then, initial stream use stream.str（“”）.

	std::cout << std::boolalpha;
	std::cout <<"line=" << __LINE__ <<std::endl;
	std::cout << "fs.good() = " << stream.good() << '\n';
	std::cout << "fs.bad() = " << stream.bad() << '\n';
	std::cout << "fs.fail() = " << stream.fail() << '\n';
	std::cout << "fs.eof() = " << stream.eof() << "\n\n";


    std::cout << "first: " << first << std::endl;         // print the string "1000"
    stream << true;         //插入bool值
    stream >> second;         //提取出int
    std::cout << "second: " << second << std::endl;

	std::cout << std::boolalpha;
	std::cout <<"line=" << __LINE__ <<std::endl;
	std::cout << "fs.good() = " << stream.good() << '\n';
	std::cout << "fs.bad() = " << stream.bad() << '\n';
	std::cout << "fs.fail() = " << stream.fail() << '\n';
	std::cout << "fs.eof() = " << stream.eof() << "\n\n";
}

int main()
{
	test_1();
//	test_2();
//	convert_test();
//	test_3();
}
