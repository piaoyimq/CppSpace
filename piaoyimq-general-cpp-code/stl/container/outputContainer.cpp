#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <vector>
#include <list>

#if 0
template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}
#else
template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v)
{
    s.put('[');
    char comma[3] = { '\0', ' ', '\0' };
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        s << comma << *it;
        comma[0] = ',';
    }
    return s << ']';
}
#endif

template<typename T>
std::ostream& operator<<(std::ostream& s, const std::list<T>& v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v) {
        s << comma << e;
        comma[0] = ',';
        // std::cout << s <<std::endl;
    }
    return s << ']';
}


#if 0
template<typename TK, typename TV>
std::ostream& operator<<(std::ostream& s, const std::map<TK, TV>& m) {
    s.put('{');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : m) {
        s << comma << e.first<<':'<<e.second;
        comma[0] = ',';
    }
    return s << '}';
}
#else
template<typename TK, typename TV>
std::ostream& operator<<(std::ostream& s, const std::map<TK, TV>& m)
{
    s.put('{');
    char comma[3] = { '\0', ' ', '\0' };
    for (typename std::map<TK, TV>::const_iterator it=m.begin(); it != m.end(); it++)
    {
        s << comma << it->first << ':' << it->second;
        comma[0] = ',';
    }
    return s << '}';
}
#endif

int main()
{
    std::vector<std::string> f{"1","2","3"};
    std::vector<std::string> f1{"11","22","33"};
    std::vector<std::string> f2{"10","20","30"};

    std::vector<std::string> f3{"a","b","c"};
    std::vector<std::string> f4{"aa","bb","cc"};
    std::vector<std::string> f5{"aaa","bbb","ccc"};
    std::list<std::vector<std::string>> words1= {f1, f2, f};
    std::list<std::vector<std::string>> words2= {f3, f4, f5};

    std::map<std::string, std::list<std::vector<std::string>>> map1= {
      {"m1", words1},
      {"m2", words2},
  };

  std::cout << map1<<std::endl;
}
