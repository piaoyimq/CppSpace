/*
 * my-cpp-test.cc
 *
 *  Created on: 08-Aug-2015
 *      Author: piaoyimq
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h> //abs()

#include <memory> //unique_ptr
#include <assert.h>  //assert
#include <stdarg.h>
#include <stdint.h>
#include <errno.h>
#include <dirent.h>
#include<utmp.h>  //getuid()
#include <pwd.h>
#include <unistd.h>

#define C_PLUS_PLUS_ELEVEN 0

static const double Pi = 3.14;
static const char Str[] = "abc, 123";

enum Color {
    COLOR_RED, COLOR_GREEN, COLOR_YELLOW
};

Color myColor = COLOR_RED;

#define OK 0

#define __PRINT_MACRO(x) #x
#define PRINT_MACRO(x) #x"="__PRINT_MACRO(x)

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define PRINT_COLOR(color, format, ...)  printf(color format RESET, ##__VA_ARGS__)
#define App_Log(logLevel, moduleId, ...)    writeLog(logLevel, moduleId, __VA_ARGS__)

void writeLog(int logLevel, int moduleId, const char* format, ...){
    char m_buf[100];
    va_list valst;
    va_start(valst, format);
    int m = vsnprintf(m_buf, sizeof(m_buf), format, valst);
    printf("logLevel=%d, moduleId=%d, m_buf:%s\n", logLevel, moduleId, m_buf);
    va_end(valst);
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

    Test(int number, std::string s): st(s), num(number){//st should initialized after number, else report waring.
    }


    void setValue() {
        num = 10;
        a = 9;
        st = "temporary";
    }

    int getValue() {
        std::cout << __FUNCTION__ << std::endl;
        return 1;
    }

    void display() {
        static int a = getValue();  // a static value in function, it would be not invoked before main function, same to run_const_test
        static_cast<void>(a);

        std::cout << "Test:a=" << Test::a << std::endl;
    }

    ~Test() {
        std::cout << "destructor: this=" << this << ", num = " << num << std::endl;
    }
private:
    int num;
    std::string st;
    static int a;   // a static value in a class, it would be invoked before main function , same to a global value, like runBeforeMain
};

int testFunction() {
    std::cout << __FUNCTION__ << std::endl;
    return 1;
}



#if 1
int Test::a = 1;
#else
int Test::a = testFunction();
#endif

void pointer() {
    int** ppUcProfileNames;
    ppUcProfileNames = new int*[5]; //???
    memset(ppUcProfileNames, 0, sizeof(void*) * 5);
}

void int_and_char_pointer() {
    char a[36] = "You _are _a_ girl_hello_how_are_you";
    int *ptr = (int *) a;
    char *p2 = a;
    char (*p3)[36] = &a; //注意，此处不可以用 char *p3=&a， 编译会报错
    ptr += 5; //相当于 ptr +5*sizeof(int)
    p2 += 5; //相当于p2+5*sizeof(char)
    p3 += 5; //相当于 ptr +5*sizeof(a),

    printf("(char*)ptr=%s, p2=%s, (char*)p3=%s\n", (char *) ptr, p2, (char*) p3);
}

void int_pointer() {
    int a = 5;
    int b[10] = { 1, 2, 3, 4, 5, 6, 7 };

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
    std::cout << "sizeof(int [10])=" << sizeof(int[10]) << std::endl; //40
    std::cout << "sizeof(ptr)=" << sizeof(ptr) << std::endl; //4
    std::cout << "sizeof(array)=" << sizeof(array) << std::endl; //4

    char *p[10];
    char a[5] = { 1, 2, 3, 4, 5 };
    p[0] = a;
    std::cout << "sizeof(p): " << sizeof(p) << std::endl; //40
    std::cout << "sizeof(*p): " << sizeof(*p) << std::endl; //*p == p[0], 4

    apn_DB_t *apndb = new apn_DB_t();
    std::cout << "sizeof(*apndb->apnByNumber): " << sizeof(*apndb->apnByNumber) << std::endl; //4
    std::cout << "sizeof(**apndb->apnByNumber): " << sizeof(**apndb->apnByNumber) << std::endl; //12
}

void pointer_array() {
    int a[10] = { 1, 2, 3, 4, 5, 6 };
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
    unsigned char test_u[] = { 0xf };
    printf("test_u[0]=%02x\n", test_u[0]);

    char test[] = { 0xf };
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
//    std::string s("hello");
//    const Test &test3 = s;
//
////    Test test3("he");
////    test3.setValue();
//    test3.print();
//    (void) test3;

    Test t4(3, "two");

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
    float c = f2(); //相当于把一个变量的引用赋值给另一个变量
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
    if (sec > 120) {
        addSeconds(900 - sec);
    } else {
        addSeconds(-sec);
    }

    struct tm reall_time_2 = *localtime(&(timeval_.tv_sec));
    std::cout << "real_time_2= " << asctime(&reall_time_2) << std::endl;
#endif
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
    //char *pNon = NULL;
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
    //static const char Str[] = "hello statistics";
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

int const_test(const std::string& ss) {
    std::cout << __FUNCTION__ << ": " << ss << std::endl;
    return 1;
}

void run_const_test() {
    std::string ss("hello");
    static int a = const_test(ss); //same to display
    static_cast<void>(a);
}

#if 0// waiting for finish
class Cls {
public:
//    Cls(int a){this->a = a ;}
//    Cls() {}
    int a;
};

struct Str {
//    Str():cls(3) {}
    int a;
    Cls cls;
};

void PodTest() {
//    Cls* cls = new Cls(3) ;
    Cls* cls = new Cls(3);
    struct Str str;
    std::cout << "cls.a=" << cls->a << std::endl;
    std::cout << "str.a=" << str.a << std::endl;
}
#endif

#if 0
void printInCompiling() {
#define TEST_MACRO 512
#pragma message("Print in Compileing:")
#pragma message(PRINT_MACRO(TEST_MACRO))
}

void runAfterMain() {
    std::cout << __FUNCTION__ <<"would be run after main function\n";
}

static int runBeforeMain() {
    std::cout << __FUNCTION__ <<"would be run before main function\n";
    return 1;
}

int a = runBeforeMain();    // a global value, it would be invoked before main function , same to a attribute in a class, like Test class.

__attribute((constructor)) void before_main()
{
    printf("%s\n",__FUNCTION__);
}

__attribute((destructor)) void after_main()
{
    printf("%s\n",__FUNCTION__);
}
#endif

const int appStartupSig(unsigned int instanceId){
    std::cout << __FUNCTION__ << std::endl;
    return instanceId;
}


void testAppStartupSig(){
    int a;
    a = appStartupSig(3);
}

void debugPrintf(int db_level, int module, const char *format, ...) {

	va_list args;

	va_start(args, format);
	char *buf = new char[256];
	if (NULL == buf)
		return; // not good!

	vsprintf(buf, format, args);
	va_end(args);

	printf("level=%d, module=%d, buf=%s", db_level, module, buf);
}

void functionParameter(int a, int b, int c){
    printf("a = %d，b = %d, c = %d", a, b, c);
}

void testFunctionParameter(){
//    functionParameter(1, 2);
}

void logFileCompression(const char* fileName, uint32_t alreadyCompressFileAmount){
    if(alreadyCompressFileAmount <= 0){
        return;
    }
    char shellContent[300] = {0};
    snprintf(shellContent, sizeof(shellContent),
            "echo '\
#!/bin/bash\n\
var=%s\n\
mv $var $var.1\n\
echo \"mv $var $var.1\"\n\
for((i=%u;i>0;i--));\n\
do\n\
  mv \"$var.$i.gz\" \"$var.$((i+1)).gz\"\n\
  echo \"mv $var.$i.gz\" \"$var.$((i+1)).gz\"\n\
done\n\
gzip $var.1\n\
echo \"gzip -f $var.1\"\n\
                    ' > test.sh; bash test.sh; rm -rf test.sh", fileName , alreadyCompressFileAmount);
//    printf("length shell:%d", strlen(shellContent));
    system(shellContent);
}

enum AppModuleID
{
    LOG_ID = 0,
    CONF_ID,
    LAST_ID                   /* -- LAST_ID MUST BE LAST ONE -- */
};


#define MODULE_NAME_MAX_SIZE 10
struct AppModule{
    AppModuleID moduleId;
    char moduleName[MODULE_NAME_MAX_SIZE];
};

static struct AppModule logModuleList[LAST_ID]={
        {LOG_ID, "LOG"},
        {CONF_ID, "CONF"},
};



namespace my{
template <class T>
size_t getArrayLen(T& array){
    return (sizeof(array) / sizeof(array[0]));
}
}

void testFunctionParameterConst(AppModuleID id){// not add const, it will run correctly, but add const it compile error. so should add const.
    printf("id1=%d", id);
    id =CONF_ID;
    printf("id2=%d", id);
}

void arrayTest(){
    std::cout << "log = " << logModuleList[LOG_ID].moduleName << std::endl;
    std::cout << "log = " << logModuleList[1].moduleName << std::endl; //so this method will lead to Segmentation fault.
    const char* const logLevelString[] = { "***EMERG","9", "lllllllALERT", "***CRIT", "***ERROR", "!!!WARN", "+++NOTICE", "INFO", "DEBUG"};
    printf("sizeof(logLevelString)=%d, sizeof([0])=%d, sizeof()=%d, len=%d\n", sizeof(logLevelString), sizeof(logLevelString[3]), sizeof(char*), sizeof(logLevelString)/sizeof(logLevelString[0]));
    printf("getArrayLen=%lu\n", my::getArrayLen(logLevelString));
    printf("sizeof(long)=%lu, sizeof(int)=%lu\n", sizeof(long), sizeof(int));
}

void renameTest(const char* oldName, const char* newName, int alreadyCompressFileAmount){
    char oldNameTemp[150]={0};
    char newNameTemp[150]={0};
    
    for(int i=0; i<= alreadyCompressFileAmount; i++){
        if(0 == i){
            if(rename(oldName, newName) < 0 ){
                printf("error: %s\n", strerror(errno));        
            }
            else{
                printf("ok!\n");        
            }
        }
        else{
            snprintf(oldNameTemp, sizeof(oldNameTemp), "%s.%d.gz", oldName, i);
            snprintf(newNameTemp, sizeof(newNameTemp), "%s.%d.gz", newName, i);
            if(rename(oldNameTemp, newNameTemp) < 0 ){
                printf("error: %s\n", strerror(errno));        
            }
            else{
                printf("ok!\n");        
            }
        }
    }
}

bool isDirExist(const char* dir){
	if(NULL == dir){
		return false;
	}
	
	if(NULL == opendir(dir)){
		return false;	
	}
	
	return true;
}

void stringTest()
{
	std::string s(41, '-');
	std::cout << s  << std::string(20, '*')<< std::endl;
}

static int a = 10;
int& returnReference()
{
	a++;
	return a;
}



void returnReferenceTest()
{
//	int b=0;
	int& b = returnReference();
	std::cout << "a" << a << std::endl;
	std::cout << "b" << b << std::endl;
	b++;
	std::cout << "a" << a << std::endl;
	std::cout << "b" << b << std::endl;


	int c =0;

}

struct sTest
{
	std::shared_ptr<int> value ;
};

sTest b;
sTest& returnReferenceSharePointer()
{
	b.value = std::make_shared<int>(9);
	return b;
}

void returnReferenceSharePointerTest()
{
	sTest c;
	c = returnReferenceSharePointer();
	std::cout << "b1= " << *b.value << std::endl;
	std::cout << "c1= " << *c.value << std::endl;

//	++(*c.value);
	*c.value = 99;
	std::cout << "b2= " << *b.value << std::endl;
	std::cout << "c2= " << *c.value << std::endl;

}

class St
{
public:
	St(){}  // if declare the copy constructor, must declare the default constructor

#if 0
private:
	St(const St& s);
	St& operator= (const St& s);
#else
public:
	St(const St& s) = delete;
	St& operator= (const St& s) = delete;
#endif
};

void testCallcopyConstructorWhenTransferParameter_1(St s) // it will called the  St(const St& s);
{
	return ;
}

void testCallcopyConstructorWhenTransferParameter_2(St& s) // it will not called the  St(const St& s);
{
	return ;
}


//St testCallcopyConstructorWhenReturn_1()
//{
//	St s;
//	return s;
//}

St aa;
St& testCallcopyConstructorWhenReturn_2()
{
	return aa;
}

//St testCallcopyConstructorWhenReturn_3(St& s)
//{
//	return s;
//}

void testCopyConstructor()
{
	St a;
	St b;

//	St c(b);//compile error, St(const St& s) is private
//	a = b;//compile error, St& operator= (const St& s) is private

//	testCallcopyConstructorWhenTransferParameter_1(a);
	testCallcopyConstructorWhenTransferParameter_2(a);
//	testCallcopyConstructorWhenReturn_1();
	testCallcopyConstructorWhenReturn_2();
//	testCallcopyConstructorWhenReturn_3(b);//compiler error
}


void testGetuserName()
{
	   struct utmp ut, *u;
	    ut.ut_type = RUN_LVL;
		while ((u = getutid(&ut)))
		{
	        printf("%d %s %s %s\n", u->ut_type, u->ut_user, u->ut_line, u->ut_host);
		}
		struct passwd *pwd;
		pwd = getpwuid(getuid());
		printf("当前登陆的用户名为：%s\n", pwd->pw_name);
}


int testUserExist(std::string userName)
{
	int result = -1;
	if (std::system(NULL))
	{
		std::string command = "id "  + userName;
		result = std::system(command.c_str()); // execute the command
	}
	else
	{
		// some system environment error, not expected to end up here
		std::cout << "no command handler available" << std::endl;
	}
	  return WEXITSTATUS(result);
}


int main() {
    PRINT_COLOR(RED, "===> Enter main\n\n");
//    int_pointer();

//    int_and_char_pointer();

//    sizeof_test();

//    pointer_array();

//    char_and_unsigned_char();

//    run_get_user_name();

    // run_Test();

//    run_f();

//    run_return_reference();

//    run_string_replace();

//    time_adjusted();

//    cpp11_smart_pointer_test();

//    coredump_test();

//    const_and_non_test();

//    enum_memset_test();

//    constructor_call_test();

//    delete_null();

//    null_reference_test();

//    private_constructor();

//    atexit(runAfterMain);

//    PodTest();

//    printInCompiling();

//    run_const_test();

//    testAppStartupSig();

    // testFunctionParameter();

//    debugPrintf(1, 2, "good");

//    logFileCompression("message", 0);

//    arrayTest();
    // testFunctionParameterConst(LOG_ID);
   
    // App_Log(3, 4, "Test=%d", 3);
    
//    stringTest();
//    returnReferenceTest();
//    returnReferenceSharePointerTest();
//    testCopyConstructor();

//

	int ret = testUserExist("ezhweibb");
	std::cout << "ret=" << ret << std::endl;
    PRINT_COLOR(RED, "\n\nExit main ===>||\n");
}

