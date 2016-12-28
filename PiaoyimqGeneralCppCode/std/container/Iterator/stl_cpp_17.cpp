// stl_cpp_17.cpp
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
using namespace std;
int main(void)
{
    vector<string> v1;
    ifstream file("Text1.txt");
    if(file.fail())
    {
      cout<<"open file Text1.txt failed"<<endl;
      return 1;
    }    
  copy(istream_iterator<string>(file),istream_iterator<string>(),inserter(v1,\
v1.begin()));
    copy(v1.begin(),v1.end(),ostream_iterator<string>(cout," "));
    cout<<endl;
    cin.get();
    return 0;
}
