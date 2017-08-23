#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>


std::pair<int, std::string> checkValue(int value)
{
    if (value < 1000)
    {
        return std::make_pair(0, "checked pass");
    }
    else
    {
        return std::make_pair(-1, "checked failed");
    }
}


void test_pair()
{
    auto test1 = std::make_pair(1, "test1");
    std::pair<int , std::string> test2 {2, "test2"};


    std::cout << "test1.first=" <<test1.first << ",  test1.second=" <<std::get<1>(test1)<< std::endl;
    std::cout << "test2.first=" <<test2.second << ",  test2.second=" <<std::get<1>(test2)<< std::endl;

    test1.first=3;
    std::get<1>(test1)="string type";
    std::cout << "test1.first=" <<test1.first << ",  test1.second=" <<std::get<1>(test1)<< std::endl;

    auto result = checkValue(100);
    if (result.first == 0)
    {
        std::cout << "Checked pass! The message is: " << result.second << "\n";
    }

    auto result2 = checkValue(1000);
    if (std::get < 0 > (result2) != 0)
    {
        std::cout << "Checking failed. The message is:" << (std::get < 1 > (result2)) << "\n";
    }
}


std::tuple<double, char, std::string> get_student(int id)
{
    if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
    if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
    if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
    throw std::invalid_argument("id");
}

void test_tuple_1()
{
    auto student0 = get_student(0);
    std::cout << "ID: 0, "
              << "GPA: " << std::get<0>(student0) << ", "
              << "grade: " << std::get<1>(student0) << ", "
              << "name: " << std::get<2>(student0) << '\n';

    double gpa1;
    char grade1;
    std::string name1;
    std::tie(gpa1, grade1, name1) = get_student(1);
    std::cout << "ID: 1, "
              << "GPA: " << gpa1 << ", "
              << "grade: " << grade1 << ", "
              << "name: " << name1 << '\n';
}

void test_tuple_2()
{
    std::tuple<std::string, std::string, std::string, int, int> record = std::make_tuple("a", "b", "c", 100, 200);
    std::cout << std::get<0>(record) << std::endl;
    std::cout << std::get<4>(record) << std::endl;
}

void test_tuple_3()
{
    std::tuple<std::string, std::string, std::string, int, int> record{"aa", "bb", "cc", 1000, 2000};
    auto record2=std::make_tuple("a2", "b2", "c2", 1000, 2000); //    std::tuple<std::string, std::string, std::string, int, int> record2={"a2", "b2", "c2", 1000, 2000};

    record={"anew", "bnew", "cnew", 1000, 2000};
    std::cout << std::get<0>(record) << std::endl;
    std::cout << std::get<4>(record) << std::endl;

    std::cout << std::get<0>(record2) << std::endl;
    std::cout << std::get<4>(record2) << std::endl;

    std::get<0>(record2)="old";
    std::cout << std::get<0>(record2) << std::endl;

//    auto re={"string", 1, 3}; //error
}

std::tuple<int, std::string, double> get_tuple()
{
    return {3, "string", 3.14};  //return a tuple, NOT a use of std::initializer_list, // Error until C++17
}

void test_tuple_4()
{
    auto result=get_tuple();
    int a;
    std::string s;
    double d;
    std::tie(a, s , d)=result;
    std::cout << std::get<0>(result) << std::endl;
}


int main()
{
    test_pair();
    test_tuple_1();
    test_tuple_2();
    test_tuple_3();
    test_tuple_4();
}
