#include <iostream>

class Base
{
public:
    Base(int a) :
            _b1(a), b2(2)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    ~Base()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void f()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << "_ba=" << _b1 << std::endl;
    }

    virtual void vf()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << "_ba=" << _b1 << std::endl;
    }

    int b2;

protected:
    virtual void __vh()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

private:
    virtual void _vg()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    int _b1;
};


class C: public Base
{
public:
    C(int a = 1, int c = 4) :
            Base(a), _c1(c)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    ~C()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    virtual void vf()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;

        f();
        Base::vf();
        Base::__vh();
        std::cout << Base::b2 << std::endl; // Compile error
//        std::cout << Base::_b1 << std::endl; // Compile error
//        Base::_vg(); // Compile error
    }

private:
    int _c1;
};


int main()
{
    C c(2,4);
    Base& b = c;
    c.vf();
}
