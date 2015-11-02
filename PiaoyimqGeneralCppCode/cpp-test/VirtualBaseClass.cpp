#include<iostream>        
using namespace std;  
class A  
{  
    protected:  
        int a;  
    public:  
        A(int a)  
        {  
            this->a=a;  
            cout << "A coustructor is invoked." << endl;
        }  
};  


class B1: public A  
{  
    public:  
        B1(int a):A(a)  
    {  
        this->a=a;  
        cout << "B1 coustructor is invoked." << endl;
    }  
};  


class B2: public A  //not virtual derived
{  
    public:  
        B2(int a):A(a)  
    {  
        this->a=a;  
        cout << "B2 coustructor is invoked." << endl;
    }  
};  


class C:public B1,public B2  
{  
    public:  
        C(int a):B1(a),B2(a) //A() was called two times
    {  
        cout << "C coustructor is invoked." << endl;
    }  
        void display()  
        {  
            //cout<<"a="<<a<<endl;//error, it's ambiguous
            cout<<"B1::a="<<B1::a<<endl;//correct
            cout<<"B2::a="<<B2::a<<endl;//correct
        }  
};  



class VB1: virtual public A  
{  
    public:  
        VB1(int a):A(a)  
    {  
        this->a=a;  
        cout << "VB1 coustructor is invoked." << endl;
    }  
};  


class VB2:virtual public A  //not virtual derived
{  
    public:  
        VB2(int a):A(a)  
    {  
        this->a=a;  
        cout << "VB2 coustructor is invoked." << endl;
    }  
};  


class VC:public VB1,public VB2  
{  
    public:  

        VC(int a):A(a),VB1(a),VB2(a)// can not lack of virtual A(a) here,  A(a) was invoked one times
    {  
        cout << "VC coustructor is invoked." << endl;
    }  
        void display()  
        {  
            cout<<"a="<<a<<endl;//correct
        }  
};



int main()  
{  
#if 0
    C c(1);  
    c.display();  
#else
    VC vc(1);  
    vc.display();  
#endif
}  
