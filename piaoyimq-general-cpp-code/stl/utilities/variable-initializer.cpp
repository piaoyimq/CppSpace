#include <iostream>
#include <vector>
#include <tuple>
#include <initializer_list>

using namespace std;

void test_initialize()
{
    int a =3;
    int b(4);
    int c{5};
    c={6};

    int d={7};

    int value[] {1 , 2 , 3};
    std::vector<int> vi {2 , 3 , 4 , 56, 7};
    std::vector<std::string> cities {"Berlin" , "New York " , "london " , "cairo"};
    std::complex<double> c{4.0 , 3.0};  //相当于c（4.0 ， 3.0）；

    //old
//    vector<string> v1("hello", "world"); // 错误
    vector<string> v3(2, "hello");       // 正确，直接初始化要指定一个重复次数，

    //new
    vector<string> v2{"hello", "world"}; // 正确，可以列表初始化, or  v2={"hello", "world"}
    vector<string> v4{10};      // 直接初始化，v4有10默认初始化的元素
    vector<string> v5{10, "hi"};// 直接初始化，v5有10个值为“hi”的元素


    std::tuple<std::string, std::string, std::string, int, int> record{"aa", "bb", "cc", 1000, 2000};


    vector<string> vs={ "first", "second", "third"};
    map<string,string> singers ={ {"Lady Gaga", "+1 (212) 555-7890"},{"Beyonce Knowles", "+1 (212) 555-0987"}};

    //old
    std::vector<std::map<std::string, std::string> > a;
    std::map<std::string, std::string> tmp;
    tmp.insert(std::make_pair("1", "2"));
    a.push_back(tmp);

    //new
    std::vector<std::map<std::string, std::string>> a{ { { "1", "2" } }, { { "3", "4" } } };
}


int main()
{
    test_initialize();
}
