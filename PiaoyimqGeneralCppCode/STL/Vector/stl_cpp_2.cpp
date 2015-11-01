//stl_cpp_2.cpp
#include <vector>
#include <iostream>

int main(void)
{
    std::vector<double> a;
    std::vector<double>::const_iterator i;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_back(5);
    for(i=a.begin(); i!=a.end(); ++i){
        std::cout<<(*i)<<std::endl;
    }
    return 0;
}
