//stl_cpp_2.cpp
#include <vector>
#include <iostream>
#include <iterator>

//std::ostream& operator<<(std::ostream& os,const employee& e)
//  {
//    os<<e.id<<" "<<e.name<<" "<<e.age<<std::endl;
//    return os;
//  }

int main(void)
{
    std::vector<double> a;
    std::vector<double>::const_iterator i;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_back(5);
#if 0
    for(i=a.begin(); i!=a.end(); ++i){
        std::cout<<(*i)<<std::endl;
    }
#else
    std::copy(a.begin(),a.end(),std::ostream_iterator<double>(std::cout, "\n"));
//    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
#endif
    return 0;
}
