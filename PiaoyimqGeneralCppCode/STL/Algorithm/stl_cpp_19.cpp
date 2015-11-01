// stl_cpp_19.cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct adder : public unary_function<double, void>
    {
      adder() : sum(0) {}
      double sum;
      void operator()(double x) { sum += x; }
    };
int main(void)
{ 
    double a[5]={0.5644,1.1,6.6,8.8,9.9};
    vector<double> V(a,a+5);
    adder result = for_each(V.begin(), V.end(), adder());
    cout << "The sum is " << result.sum << endl;
    cin.get();
    return 0;
}
