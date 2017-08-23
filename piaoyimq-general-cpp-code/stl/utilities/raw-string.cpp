#include <iostream>

char array1[] = "Foo" "bar";
// same as
char array2[] = { 'F', 'o', 'o', 'b', 'a', 'r', '\0' };

//raw string, 可以定义一个无需转义的字符串，如：std::string b = R"x(c:\abc)" yy l)x";
//其中x(是一个标识符，x可以任意，最长16个字符。
const char* s1 = R"foo(
Hello
World
)foo";
//same as
const char* s2 = "\nHello\nWorld\n";

int main()
{
    std::cout << array1 << '\n';
    std::cout << array2 << '\n';

    std::cout << s1;
    std::cout << s2;
    std::string b = R"x(c:\abc)" yy l)x";//raw string
    std::string c="c:\abc) yy l";

    std::cout << b << std::endl;
    std::cout << c<< std::endl;;
}
