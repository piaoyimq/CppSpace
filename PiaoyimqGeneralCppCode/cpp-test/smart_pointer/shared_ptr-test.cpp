
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
