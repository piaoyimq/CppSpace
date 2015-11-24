/*
 * pointer.cc
 *
 *  Created on: 08-Aug-2015
 *      Author: ezhweib
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h> //abs()
#include <sstream> //stringstream
#include <typeinfo> //typeid()
#include <memory> //unique_ptr
#include <assert.h>  //assert
static const double Pi = 3.14;
static const char sgwStatsStr[] = "show services epg sgw statistics";
enum Color {
    COLOR_RED, COLOR_GREEN, COLOR_YELLOW
};
Color myColor = COLOR_RED;
#define OK 0

#if 0//c++11
#if 1// Todo:  why
void pointer() {
    int** ppUcProfileNames;
    ppUcProfileNames = new int*[5]; //???
    memset(ppUcProfileNames, 0, sizeof(void*) * 5);
}

#endif

void int_and_char_pointer() {
    char a[36] = "You _are _a_ girl_hello_how_are_you";
    int *ptr = (int *) a;
    char *p2 = a;
    char (*p3)[36] = &a; //注意，此处不可以用 char *p3=&a， 编译会报错
    ptr += 5;//相当于 ptr +5*sizeof(int)
    p2 += 5;//相当于p2+5*sizeof(char)
    p3 += 5;//相当于 ptr +5*sizeof(a),

    printf("(char*)ptr=%s, p2=%s, (char*)p3=%s\n", (char *) ptr, p2, (char*) p3);
}

void int_pointer() {
    int a = 5;
    int b[10] = {1, 2, 3, 4, 5, 6, 7};

    int *p1 = &a;
    int *p2 = b;

    printf("p1[0]=%d,  *(p1+0)=%d\n", p1[0], *(p1 + 0));
    printf("p2[1]=%d, *(p2+1)=%d\n", p2[1], *(p2 + 1));
    printf("&p2[1]=%p, p2+1=%p\n", &p2[1], p2 + 1);
    printf("&p2[0]=%p, p2+0=%p,\n", &p2[0], p2 + 0);

    printf("&b=%p, &b[0]=%p, b=%p\n", &b, &b[0], b);
    printf("&b+1=%p, &b[0]+1=%p, b+1=%p", &b + 1, &b[0] + 1, b + 1);
}

struct Apn_Entry_t {
    char a[5];
    int b;  //按 4字节对齐
};

struct apn_DB_t {
    char *p;
    Apn_Entry_t *apnByNumber[5];
    int m;
};

void sizeof_test() {
    int array[10];
    int (*ptr)[10];
    ptr = &array;

    std::cout << "sizeof(int (*)[10])=" << sizeof(int (*)[10]) << std::endl; //4
    std::cout << "sizeof(int [10])=" << sizeof(int[10]) << std::endl;//40
    std::cout << "sizeof(ptr)=" << sizeof(ptr) << std::endl;//4
    std::cout << "sizeof(array)=" << sizeof(array) << std::endl;//4

    char *p[10];
    char a[5] = {1, 2, 3, 4, 5};
    p[0] = a;
    std::cout << "sizeof(p): " << sizeof(p) << std::endl; //40
    std::cout << "sizeof(*p): " << sizeof(*p) << std::endl;//*p == p[0], 4

    apn_DB_t *apndb = new apn_DB_t();
    std::cout << "sizeof(*apndb->apnByNumber): " << sizeof(*apndb->apnByNumber) << std::endl;//4
    std::cout << "sizeof(**apndb->apnByNumber): " << sizeof(**apndb->apnByNumber) << std::endl;//12
}

void pointer_array() {
    int a[10] = {1, 2, 3, 4, 5, 6};
    int b = 9;
    int *p[5];

    for (int i = 0; i < 5; i++) {
        p[i] = NULL;
    }

    p[3] = a;
    p[4] = &b;

    printf("p[3][3]=%d, **(p+3)=%d\n", p[3][3], *(*(p + 3) + 3));
    printf("*(p[4])=%d, *(p+4)=%d\n", *(p[4]), **(p + 4));
}

void char_and_unsigned_char() {
    unsigned char test_u[] = {0xf};
    printf("test_u[0]=%02x\n", test_u[0]);

    char test[] = {0xf};
    printf("test=[0]%02x\n", test[0]);
}

const char *get_user_name() {
    char const *username = "abc";   //养成好习惯，此处const不要少，虽然少了不会编译出错
    return username;
}

void run_get_user_name() {
    const char *p = get_user_name(); //此处声明指针p,必须带const，否则编译错误，因为函数返回值是const *指针
    printf("p =%s\n", p);
    //    p[0] ='m';
    printf("p[0] =%c\n", p[0]);
}

class Test {
public:
    Test() :
    num(0) {
    }

    Test(int number) :
    num(number) {
    }
    Test(std::string s) :
    st(s) {
    }
    void print() const {
        std::cout << "num = " << num << std::endl;
        std::cout << "st = " << st << std::endl;
    }

    void setValue() {
        num = 10;
        a = 9;
        st = "temporary";
    }

    void display() {
        std::cout << "Test:a=" << Test::a << std::endl;
    }

    ~Test() {
        std::cout << "destructor: this=" << this << ", num = " << num << std::endl;
    }
private:
    int num;
    std::string st;
    static int a;
};

int Test::a = 8;

void call_static_variable(Test &test) {
    test.setValue();
//    std::cout << "Test:a=" << Test::a <<std::endl;    //静态成员变量只可以在初始化时在类外直接通过类名赋值，其它任何时候都不可以通过类名在类外操作
    test.display();
}

void fun1_reference(Test &test) {
//    test.setValue();
    test.print();
}

void fun1_pointer(const Test *test) {
//    test.setValue();
    test->print();
}

void fun2_const(const Test &test) {
//    test.setValue();
//    test.num=10;
}

void fun2_non_const(Test &test) {
    test.setValue();
//    test.num=20;
}

void fun3(int a) {
    (void) a; //cancel the warning of complier
}

void run_Test() {
//    Test t1(1);
//    fun1_reference(t1); //can't generate a temporary variable
//    char m = 2;
//    fun2_const(m);/*引用必须有一个实在的、可引用的对象，否则引用就是错误的。因此，在没有实在的、可引用的对象的时候，只有依赖于临时对象。 此处必须为const*/
//    fun2_non_const(t1);
//
//    const Test &test2 = m;  //const 引用可以引用一个与其类型完全不相同的类型，前提是可以进行类型转换
//    (void) test2;
//
    std::string s("hello");
    const Test &test3 = s;
//    Test test3("he");
//    test3.setValue();
    test3.print();
    (void) test3;

//    call_static_variable(t1);
}

float f;
float f1(float r = 5) {
    f = r * r;
    return f;
}

float &f2(float r = 5) {
    f = r * r;
    return f;
}

void run_f() {
    float a = f1();
//    float &b = f1();  //error,见C++笔记
    float c = f2();//相当于把一个变量的引用赋值给另一个变量
    float &d = f2();
    c = c + 1;
    std::cout << "a=" << a << std::endl;
//    std::cout << "b=" << b << std::endl;
    std::cout << "c=" << c << std::endl;
    d = d + 2;
    std::cout << "d=" << d << std::endl;
    std::cout << "f=" << f << std::endl;
}

int &return_reference() {
    int *p1 = new int(10);
    printf("*p1=%d\n", *p1);
    printf("p1=%p\n", p1);
    printf("&p1=%p\n", &p1);
    return *p1; //返回new分配内存的引用， 一般不提倡这么做：不要返回函数内new分配内存的引用，容易造成内存泄露，因为要释放需要： delete &引用名
}

void run_return_reference() {
    int &p2 = return_reference();
    printf("p2=%d\n", p2);
    printf("&p2=%p\n", &p2);
    delete &p2;         //导致这样奇怪的delete，是因为返回了new分配的引用
    printf("after delete&p2,    p2=%d\n", p2);
}

char * delete_charater_by_c(char *str, char c) {
    char *head = NULL;
    char *p = NULL;
    if (str == NULL) {
        return NULL;
    }

    head = p = str;
    while (*p++) {
        if (*p != c) {
            *str++ = *p;
        }
    }
    *str = '\0';
    return head;
}

void string_replace_1(std::string&source_str, const std::string&old_str, const std::string&new_str) {
    std::string::size_type pos = 0;
    std::string::size_type a = old_str.size();
    std::string::size_type b = new_str.size();

    while ((pos = source_str.find(old_str, pos)) != std::string::npos) {
        source_str.replace(pos, a, new_str);
        pos += b;
    }
}

void string_replace_2(std::string&s1, const std::string&s2, const std::string&s3) {
    //if s3 is null ,then this function equal delete_charater_by_c
    std::string::size_type pos = 0;
    std::string::size_type a = s2.size();
    std::string::size_type b = s3.size();
    while ((pos = s1.find(s2, pos)) != std::string::npos) {
        s1.erase(pos, a);
        s1.insert(pos, s3);
        pos += b;
    }
}

void run_string_replace() {
    std::string s("<<helloxxxmorinig");
//    std::string s_t("<version<>.<application>@<domain>");
    std::string s_t("<version.application@<>domain");
    char s_c[] = "<<everyone are is here";

    delete_charater_by_c(s_c, '<');
    std::cout << s_c << std::endl;
//
//    string_replace_1(s_t, "<", "");
//    string_replace_1(s_t, ">", "");
//    std::cout << s_t << std::endl;

    string_replace_2(s, "hello", "hi");
    std::cout << s << std::endl;

    std::cout << "s_t" << s_t << std::endl;
    if ((s_t.find("<")) != std::string::npos || (s_t.find(">")) != std::string::npos) {

        std::string str_temp(s_t);
        std::cout << "str_temp_1=" << str_temp << std::endl;
        string_replace_1(str_temp, "<", "");
        string_replace_1(str_temp, ">", "");
        std::cout << "str_temp_2=" << str_temp << std::endl;
    }

    std::cout << "s_t=" << s_t << std::endl;
}

struct timeval timeval_;

void addSeconds(int seconds) {
    //#[ operation addSeconds(int)
    timeval_.tv_sec += seconds;
    //#]
}

void time_adjusted() {
#if 0
    gettimeofday(&timeval_, 0);
    time_t timeval_tv_sec = timeval_.tv_sec;
    struct tm reall_time_1 = *localtime(&timeval_tv_sec);
    std::cout << "reall_time_1= " << asctime(&reall_time_1) << std::endl;

    struct tm local = *localtime(&timeval_tv_sec);
    int sec = 900 - (local.tm_min * 60 + local.tm_sec) % 900;
    addSeconds(sec);

    time_t timeval_tv_sec_2 = timeval_.tv_sec;
    if (abs(timeval_tv_sec_2 - timeval_tv_sec) >= 450) {
        std::cout << "_______enter abs()___" << std::endl;
        addSeconds(-900);
    }

    struct tm reall_time_2 = *localtime(&(timeval_.tv_sec));
    std::cout << "real_time_2= " << asctime(&reall_time_2) << std::endl;
#else
    gettimeofday(&timeval_, 0);
    time_t timeval_tv_sec = timeval_.tv_sec;
    struct tm local = *localtime(&timeval_tv_sec);

    std::cout << "reall_time_1= " << asctime(&local) << std::endl;
    int sec = (local.tm_min * 60 + local.tm_sec) % 900;
    if(sec > 120) {
        addSeconds(900-sec);
    }
    else {
        addSeconds(-sec);
    }

    struct tm reall_time_2 = *localtime(&(timeval_.tv_sec));
    std::cout << "real_time_2= " << asctime(&reall_time_2) << std::endl;
#endif
}

void printAlarm(const int &id, const std::string &alarmType, std::stringstream& os) {
    os << "    Fault Id: " << id << std::endl;
    os << "    Alarm Major Type: " << alarmType << std::endl;
    os << "    Alarm Minor Type:  Unknown" << std::endl;
}

template<class out_type, class in_value>
out_type convert(const in_value & t) {
    std::stringstream stream;
    stream << t;         //向流中传值
    out_type result;//这里存储转换结果
    stream >> result;//向result中写入值
    return result;
}

void stringstream_test() {
    std::stringstream stringBuffer;
    printAlarm(10, "ActiveAlarm", stringBuffer);
    std::cout << "stringBuffer size:" << stringBuffer.str().size() << std::endl;
    std::cout << stringBuffer.str() << std::endl;         //stringBuffer.str().c_str(), is c language charaters.

    //stringstream is  used to data type convertsion.
    std::stringstream stream;
    std::string first, second;
    int i = 1000;
    stream << i;
    stream >> first;//从stream中抽取前面插入的int值
    std::cout << "first: " << first << std::endl;// print the string "1000"

    stream.clear();//在进行多次转换前，必须清除stream  , stream.str（“”） ???
    std::cout << "first: " << first << std::endl;// print the string "1000"
    stream << true;//插入bool值
    stream >> second;//提取出int
    std::cout << "second: " << second << std::endl;

    double d, d2;
    std::string salary, salary2;
    std::string s = "12.56";
    d = convert<double>(s);//d等于12.56
    d2 = d;
    printf("d2=%f\n", d2);
    std::cout << "d2: " << d2 << std::endl;

    salary = convert<std::string>(9000.0);//salary等于”9000”
    salary2 = salary;
    printf("salary2: %s\n", salary2.c_str());
    std::cout << "typeid(d): " << typeid(d2).name() << std::endl;
    std::cout << "typeid(salary2): " << typeid(salary2).name() << std::endl;
    std::cout << "stringstream: " << typeid(std::stringstream).name() << std::endl;
}

struct Foo {
    Foo() {
        std::cout << "Foo::Foo\n";
    }
    ~Foo() {
        std::cout << "Foo::~Foo\n";
    }
    void bar() {
        std::cout << "Foo::bar\n";
    }
};

void function_1(const Foo& foo) {
    std::cout << __FUNCTION__ << "(const Foo&)" << std::endl;
}

void unique_ptr_test_1() {
    std::unique_ptr<Foo> p1(new Foo);  // p1 owns Foo
    if (p1)
    p1->bar();

    std::unique_ptr<Foo> p2(std::move(p1));// now p2 owns Foo
    function_1(*p2);//Notice: is *p2, not p2

    p1 = std::move(p2);// ownership returns to p1
    std::cout << "destroying p2...\n";

    if (p1)
    p1->bar();

    // Foo instance is destroyed when p1 goes out of scope
}

std::unique_ptr<int> unique_f() {
    std::unique_ptr<int> p(new int[10]);
    *p=5;
    // 做一些事情，可能会抛出异常,抛出异常,unique_ptr会自动释放，即为动态申请的内存提供异常安全
    return p;
}

void unique_ptr_test_2() {
    std::unique_ptr < int > q = unique_f();
    /* 使用移动构造函数（move constructor）,unique_ptr具有移动语义，所以我们可以使用函数
     * unique_f()返回的右值对q进行初始化，这样就简单地将所有权传递给了q。*/
//    q->DoSomething();                   // 使用q
    int x = *q;//复制指针q所指向的对象
    std::cout << "x="<<x<<std::endl;
}    // 在函数退出的时候，q以及它所指向的对象都被删除释放

void shared_ptr_test_1() {
    std::shared_ptr<int> pInt1;
    assert(pInt1.use_count() == 0);         // 还没有引用指针
    {
        std::shared_ptr<int> pInt2(new int(5));
        assert(pInt2.use_count() == 1);        // new int(5)这个指针被引用1次

        pInt1 = pInt2;
        assert(pInt2.use_count() == 2);// new int(5)这个指针被引用2次
        assert(pInt1.use_count() == 2);
    }       //pInt2离开作用域, 所以new int(5)被引用次数-1

    std::cout << "pInt1.use_count= " << pInt1.use_count() << std::endl;
    assert(pInt1.use_count() == 1);
}         // pInt1离开作用域，引用次数-1,现在new int(5)被引用0次，所以销毁它

/*如果资源的创建销毁不是以new和delete的方式进行的，该怎么办呢？
 * 通过前面的接口可以看到，shared_ptr的构造函数中可以指定删除器*/
void shared_ptr_test_2() {
    class FileCloser {
    public:
        void operator()(FILE *pf) {
            if (pf != NULL) {
                std::cout << "close FILE" << std::endl;
                fclose(pf);
                pf = NULL;
            }
        }
    };
    std::string pszConfigFile("C:\\zhuweibo\\Code Workspace\\CppSpace\\README.md");
    std::shared_ptr<FILE> fp(fopen(pszConfigFile.c_str(), "r"), FileCloser());
//    char buf[100];
//    fread(buf, 100, 100, fp);
//    std::cout << buf << std::endl;
}

void shared_ptr_test_3() {
#if 0//err operation
    /*在使用shared_ptr时，需要避免同一个对象指针被两次当成shard_ptr构造函数里的参数的情况*/
    int *pInt = new int(5);
    std::shared_ptr<int> temp1(pInt);
    assert(temp1.use_count() == 1);
    std::shared_ptr<int> temp2(pInt);
    assert(temp2.use_count() == 1);
    std::cout <<"temp2.use_count: "<< temp2.use_count() <<std::endl;
    // temp1和temp2都离开作用域，它们都销毁pInt，会导致两次释放同一块内存
#elif 0
    /*正确的做法是将原始指针赋给智能指针后，以后的操作都要针对智能指针了*/
    std::shared_ptr<int> temp1(new int(5));
    assert(temp1.use_count() == 1);
    std::shared_ptr<int> temp2(temp1);
    assert(temp2.use_count() == 2);
    std::cout << "temp2.use_count: " << temp2.use_count() << std::endl; //2
    // temp1和temp2都离开作用域，引用次数变为0，指针被销毁
#elif 1
    /*使用shared_ptr来包装this时，也会产生与上面类似的问题*/

    class A {
    public:
        std::shared_ptr<A> Get() {
            return std::shared_ptr < A > (this);
        }
        A() {
            p = new int[10];
        }
        ~A() {
            std::cout << "delete" << std::endl;
            delete[] p;
        }
        int *p;
    };

    std::shared_ptr<A> pA(new A());
    //pA, pB 分别被同一个对象初始化了，即pA, pB都指向同一地址 ，但是他们的引用计数才为1 ，应该为2，这样会导致多次释放，出现coredump
    std::cout << "pA.use_count: " << pA.use_count() << std::endl;
    std::shared_ptr<A> pB = pA->Get();
    std::cout << "pB.use_count: " << pB.use_count() << std::endl;
#else
    /*当pA和pB离开作用域时，会将堆上的对象释放两次。如何解决上述问题呢？C++ 11提供了如下机制：
     * 将类从enable_shared_from_this类派生，获取shared_ptr时使用shared_from_this接口。*/
    class A: public std::enable_shared_from_this<A> {
    public:
        std::shared_ptr<A> Get() {
            return shared_from_this();
        }
        A() {
            p = new int[10];
        }
        ~A() {
            std::cout << "delete" << std::endl;
            delete[] p;
        }
        int *p;
    };

    std::shared_ptr<A> pA(new A());
    std::cout << "pA.use_count: " << pA.use_count() << std::endl;
    std::shared_ptr<A> pB = pA->Get();
    std::cout << "pB.use_count: " << pB.use_count() << std::endl;
#endif
}

std::weak_ptr<int> gw;

void weak_ptr_f() {
    if (auto spt = gw.lock()) { // Has to be copied into a shared_ptr before usage
        std::cout << *spt << "\n";
    } else {
        std::cout << "gw is expired\n";
    }
}

void weak_ptr_test_1() {
    {
        auto sp = std::make_shared<int>(42);
        gw = sp;

        weak_ptr_f();
    }

    weak_ptr_f();
}

void weak_ptr_test_2() {
#if 0/*a和b相互进行引用。在a和b离开作用域时，a和b的引用计数都是1，内存没有正常释放*/
    class B;
    class A;
    typedef std::shared_ptr<A> A_Share;
    typedef std::shared_ptr<B> B_Share;
    class A {
    public:
        ~A() {
            std::cout << "A delete" <<std::endl;
        }
        B_Share m_b;
    };

    class B {
    public:
        ~B() {
            std::cout << "B delete" <<std::endl;
        }
        A_Share m_a;
    };

    A_Share a(new A());
    std::cout << "a.use_count: " << a.use_count() << std::endl;
    B_Share b(new B());
    a->m_b = b;
    std::cout << "a.use_count: " << a.use_count() << std::endl;
    b->m_a = a;
    std::cout << "a.use_count: " << a.use_count() << std::endl;
    std::cout << "b.use_count: " << b.use_count() << std::endl;
#else
    /*解决方法是将A和B中的任意一个类声明的变量改为week_ptr类型的。比如，修改类B后的代码如下：
     * 修改后，b.m_a = a不会增加A对象的引用计数，因此a离开作用域时，引用计数为0。
     B对象的引用计数为2，在a和b离开作用域时，引用计数各减1后也为0。*/
    class B;
    class A;
    typedef std::shared_ptr<A> A_Share;
    typedef std::shared_ptr<B> B_Share;
    class A {
    public:
        ~A() {
            std::cout << "A delete" << std::endl;
        }
        B_Share m_b;
    };

    class B {
    public:
        ~B() {
            std::cout << "B delete" << std::endl;
        }
        std::weak_ptr<A> m_a;
    };

    A_Share a(new A());
    B_Share b(new B());
    a->m_b = b;
    b->m_a = a;
    std::cout << "a.use_count: " << a.use_count() << std::endl;
    std::cout << "b.use_count: " << b.use_count() << std::endl;

#endif
}

void cpp11_smart_pointer_test() {   //c++11
//    unique_ptr_test_1();
    unique_ptr_test_2();
//    shared_ptr_test_1();
//    shared_ptr_test_2();
//    shared_ptr_test_3();
//    weak_ptr_test_1();
//    weak_ptr_test_2();
}
#endif//c++11 

void coredump_1() {
    struct Apn_Entry_t {
        unsigned long int ApnFingerprint;
    };
    struct Apn_Entry_t *apn = NULL;

    printf("an->ApnFingerprint=%lu", apn->ApnFingerprint);

}

void coredump_test() {
    coredump_1();
}
void const_and_non_test() {
    int a;
    const int b = 10;
    double c = 12; // should initilize, otherwise produce a waring

    int& ra = a;
    const int& cra = a; //Does it produce a temporary object? ==> no

    //int& rb = b; //error: nonconst reference to a const object reference.
    const int& crb = b;

    // int& rc = c;//error, not the same type ,should use a const reference.
    const int& crc = c; //produce temporary object

    // int& rNon = 99;//error, should use const reference
    const int& crNon = 99; //produce a temporary object: int temp(99);

    const char *pc = "hello";
    char *pNon = NULL;
    const std::string& rpc = pc;
    const std::string& srpc = std::string(pc);
    // const std::string& rpNon = pNon;//produce core dump
    // const std::string& srpNon = std::string(pNon);//produce core dump

    std::cout << "&a   = " << &a << std::endl;
    std::cout << "&ra  = " << &ra << std::endl;
    std::cout << "&cra = " << &cra << std::endl;

    std::cout << "&b   = " << &b << std::endl;
    // std::cout << "&rb = " << &rb << std::endl;
    std::cout << "&crb = " << &crb << std::endl;

    std::cout << "&c   = " << &c << std::endl;
    // std::cout << "&rc  = " << &rc << std::endl;
    std::cout << "&crc = " << &crc << std::endl;

    // std::cout << "&rNon = " << &rNon << std::endl;
    std::cout << "&crNon = " << &crNon << std::endl;

    std::cout << "&rpc    = " << &rpc << std::endl;
    std::cout << "&srpc   = " << &srpc << std::endl;
    // std::cout << "&rpNon  = " << &rpNon << std::endl;
    // std::cout << "&srpNon = " << &srpNon << std::endl;
}

void enum_memset_test() {
    enum Radius_retry_method_t {
        RADIUS_SINGLE_SERVER, RADIUS_MULTIPLE_SERVER
    };

    bool bQuarantineAll(false);

    Radius_retry_method_t retryMethod(RADIUS_MULTIPLE_SERVER);

    std::cout << retryMethod << std::endl;
    std::cout << bQuarantineAll << std::endl;
}

void constructor_call_test() {

    struct A {
        A() {
            std::cout << "A was initilized." << std::endl;
        }
    };

    struct B {
        struct A a;
        int b;
        bool c;
        char *d;

        B() :
                b(0), c(false), d(NULL) {
            std::cout << "B was initilized>" << std::endl;
        }
    };

    struct B b;

    if (!b.d) {
        std::cout << "d is null" << std::endl;
    }
}

void delete_null() {
    int *p = new int;
    p = NULL;
    delete p;
    p = NULL;
}

void null_reference_test() {    //???
    //int *p = new int[10];
    int *p = NULL;
    //p[1]= 10;
    int& rp = *p;
    p = new int;
    *p = 10;
    rp = 9;
    //std::cout << rp << std::endl;
}

void private_constructor() {
    static const char Str[] = "hello statistics";
    class Person {
    public:

        ~Person() {
            std::cout << "Person destructor in public." << std::endl;
        }

        static Person* createInstance() {
            Person* per = new Person();
            return per;
        }

    private:

        Person() {
            std::cout << "Person constructor." << std::endl;
        }
        // ~Person(){
        //    std::cout << "Person destructor in private." << std::endl;
        //}
    };

    Person *pPer = Person::createInstance();
    delete pPer;
}

int main() {
//    int_pointer();

//    int_and_char_pointer();

//    sizeof_test();

//    pointer_array();

//    char_and_unsigned_char();

//    run_get_user_name();

//    run_Test();

//    run_f();

//    run_return_reference();

//    run_string_replace();

    // time_adjusted();

    // stringstream_test();

//    cpp11_smart_pointer_test();
     coredump_test();
//    const_and_non_test();
//    enum_memset_test();
//    constructor_call_test();
//    delete_null();
//      null_reference_test();
<<<<<<< HEAD
    private_constructor();

    printf("LINE= %d\n", __LINE__);

    int a= 0;
    a-= (3-1);
    int b= 0;
    b-= 3-1;
    std::cout << "a=" << a << "b=" << b << std::endl;
=======
   // private_constructor();
>>>>>>> Modify my-cpp-test.cpp
}


