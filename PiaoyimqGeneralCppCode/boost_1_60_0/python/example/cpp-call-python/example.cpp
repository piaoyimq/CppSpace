#include <iostream>
#include <string>
#include <boost/python.hpp>
#include <boost/ref.hpp>

using namespace boost::python;

int main(int argc, char* argv[])
{
    Py_Initialize();

    object sys = import("sys");
    object path = sys.attr("path");
    path.attr("append")(".");

    object example = import("example");
    object goods = example.attr("goods")(1.23);
    object rc = goods.attr("sale")(1.1);
//    object rc2=goods.attr.("say")();

    std::cout << extract<float>(rc) << std::endl;

    return 0;
}
