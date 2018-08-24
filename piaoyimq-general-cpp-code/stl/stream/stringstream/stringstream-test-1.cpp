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
	ostr.put('d');//���������������������������������������������,������������������������������������������������������,������������������������,���������������������
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
    stream << t;         //���������������
    out_type result;         //������������������������
    stream >> result;         //���result������������
    return result;
}


void convert_test()
{
	  double d, d2;
	    std::string salary, salary2;
	    std::string s = "12.56";
	    d = convert<double>(s);         //d������12.56
	    d2 = d;
	    printf("d2=%f\n", d2);
	    std::cout << "d2: " << d2 << std::endl;

	    salary = convert<std::string>(9000.0);         //salary���������9000���
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
    stream >> first;         //���stream������������������������int���
    std::cout << "first: " << first << std::endl;         // print the string "1000"

/*���C++���������������stringstream������������������������������������������������������
 * ���������������������������������stringstream������������ ���������������������������������������������clear���������
 * ������stringstream���������������������������������stringstream������������������������������������ ���������������������������.str("")���������
 *
 * ������������stringstream������
������������������������������������������������stringstream������������������������������������������clear()���������
���������������������������������������stringstream������������������������������������������������������������������������������������stringstream���������������������������������������������������CPU������������
 * */
	std::cout << std::boolalpha;
	std::cout <<"line=" << __LINE__ <<std::endl;
	std::cout << "fs.good() = " << stream.good() << '\n';
	std::cout << "fs.bad() = " << stream.bad() << '\n';
	std::cout << "fs.fail() = " << stream.fail() << '\n';
	std::cout << "fs.eof() = " << stream.eof() << "\n\n";

    stream.clear();//here, use clear(), to reset the flags
    stream.str("");//���������������������������������������stream, use clear(), then, initial stream use stream.str������������.

	std::cout << std::boolalpha;
	std::cout <<"line=" << __LINE__ <<std::endl;
	std::cout << "fs.good() = " << stream.good() << '\n';
	std::cout << "fs.bad() = " << stream.bad() << '\n';
	std::cout << "fs.fail() = " << stream.fail() << '\n';
	std::cout << "fs.eof() = " << stream.eof() << "\n\n";


    std::cout << "first: " << first << std::endl;         // print the string "1000"
    stream << true;         //������bool���
    stream >> second;         //���������int
    std::cout << "second: " << second << std::endl;

	std::cout << std::boolalpha;
	std::cout <<"line=" << __LINE__ <<std::endl;
	std::cout << "fs.good() = " << stream.good() << '\n';
	std::cout << "fs.bad() = " << stream.bad() << '\n';
	std::cout << "fs.fail() = " << stream.fail() << '\n';
	std::cout << "fs.eof() = " << stream.eof() << "\n\n";
}


void test_4()
{
    std::stringstream ss;
    std::string message("hello\ngood\nhi\n");

    ss << message;
    log.debug() << "____ss=" << ss.str() << std::endl;

    std::string line;

    while (getline(ss, line))
    {
        log.debug() << "____line=" << line << std::endl;
    }
}


int main()
{
	test_1();
//	test_2();
//	convert_test();
//	test_3();
}
