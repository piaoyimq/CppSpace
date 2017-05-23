#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

std::weak_ptr<int> gw;

void weak_ptr_f()
{
	if (auto spt = gw.lock())
	{ // Has to be copied into a shared_ptr before usage
		std::cout << *spt << "\n";
	}
	else
	{
		std::cout << "gw is expired\n";
	}
}

void weak_ptr_test_1()
{
	{
		auto sp = std::make_shared<int>(42);
		gw = sp;

		weak_ptr_f();
	}

	weak_ptr_f();
}

void weak_ptr_test_2()
{
#if 0/*a和b相互进行引用。在a和b离开作用域时，a和b的引用计数都是1，内存没有正常释放*/
	class B;
	class A;
	typedef std::shared_ptr<A> A_Share;
	typedef std::shared_ptr<B> B_Share;
	class A
	{
	public:
		~A()
		{
			std::cout << "A delete" <<std::endl;
		}
		B_Share m_b;
	};

	class B
	{
	public:
		~B()
		{
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
	class A
	{
	public:
		~A()
		{
			std::cout << "A delete" << std::endl;
		}
		B_Share m_b;
	};

	class B
	{
	public:
		~B()
		{
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

int test_weak_ptr3()
{
    // OLD, problem with dangling pointer
    // PROBLEM: ref will point to undefined data!
    int* ptr = new int(10);
    int* ref = ptr;
    delete ptr;

    // NEW
    // SOLUTION: check expired() or lock() to determine if pointer is valid
    // empty definition
    std::shared_ptr<int> sptr;

    // takes ownership of pointer
    sptr.reset(new int);
    *sptr = 10;

    // get pointer to data without taking ownership
    std::weak_ptr<int> weak1 = sptr;

    // deletes managed object, acquires new pointer
    sptr.reset(new int);
    *sptr = 5;

    // get pointer to new data without taking ownership
    std::weak_ptr<int> weak2 = sptr;

    // weak1 is expired!

    if (auto tmp = weak1.lock())
        std::cout << *tmp << '\n';
    else
        std::cout << "weak1 is expired\n";

    // weak2 points to new data (5)

    if (auto tmp = weak2.lock())
        std::cout << *tmp << '\n';
    else
        std::cout << "weak2 is expired\n";

    return 0;
}


class Controller
{
public:
    int Num;
    std::string Status;
    std::vector<std::weak_ptr<Controller>> others;
    explicit Controller(int i) : Num(i), Status("On")
    {
        std::cout << "Creating Controller" << Num << std::endl;
    }

    ~Controller()
    {
        std::cout << "Destroying Controller" << Num << std::endl;
    }

    // Demonstrates how to test whether the pointed-to memory still exists or not.
    void CheckStatuses() const
    {
        for_each(others.begin(), others.end(), [](std::weak_ptr<Controller> wp) {
            try {
                auto p = wp.lock();
                std::cout << "Status of " << p->Num << " = " << p->Status << std::endl;
            }
            catch (std::bad_weak_ptr b) {
                std::cout << "Null object" << std::endl;
            }
        });
    }
};

void RunTest()
{
    std::vector<std::shared_ptr<Controller>> v;

    v.push_back(std::shared_ptr<Controller>(new Controller(0)));
    v.push_back(std::shared_ptr<Controller>(new Controller(1)));
    v.push_back(std::shared_ptr<Controller>(new Controller(2)));
    v.push_back(std::shared_ptr<Controller>(new Controller(3)));
    v.push_back(std::shared_ptr<Controller>(new Controller(4)));

    // Each Controller depends on all others not being deleted.
    // Give each Controller a pointer to all the others.
    for (int i = 0; i < v.size(); ++i) {
        for_each(v.begin(), v.end(), [v, i](std::shared_ptr<Controller> p) {
            if (p->Num != i) {
                v[i]->others.push_back(std::weak_ptr<Controller>(p));
                std::cout << "push_back to v[" << i << "]: " << p->Num << std::endl;
            }
        });
    }

    for_each(v.begin(), v.end(), [](std::shared_ptr<Controller>& p) {
        std::cout << "use_count = " << p.use_count() << std::endl;
        p->CheckStatuses();
    });
}

int test_weak_ptr4()
{
    RunTest();
    std::cout << "Press any key" << std::endl;
    char ch;
    std::cin.getline(&ch, 1);

    return 0;
}

////////////////////////////////////////////////
// reference: https://oopscenities.net/2014/08/03/c-smart-pointers-part-5-weak_ptr/
struct Child;
struct Parent
{
    std::shared_ptr<Child> child;

    ~Parent() { std::cout << "Bye Parent" << std::endl; }

    void hi() const { std::cout << "Hello" << std::endl; }
};

struct Child
{
    std::weak_ptr<Parent> parent;
    //std::shared_ptr<parent> parent; // memory leak

    ~Child() { std::cout << "Bye Child" << std::endl; }
};

int test_weak_ptr5()
{
    auto parent = std::make_shared<Parent>();
    auto child = std::make_shared<Child>();

    parent->child = child;
    child->parent = parent;
    child->parent.lock()->hi();
    // child->parent->hi();

    return 0;
}

/////////////////////////////////////////////////////
// reference: http://thispointer.com/shared_ptr-binary-trees-and-the-problem-of-cyclic-references/
class Node
{
    int value;
public:
    std::shared_ptr<Node> leftPtr;
    std::shared_ptr<Node> rightPtr;
    // Just Changed the shared_ptr to weak_ptr
    std::weak_ptr<Node> parentPtr;
    Node(int val) : value(val)     {
        std::cout << "Contructor" << std::endl;
    }
    ~Node()     {
        std::cout << "Destructor" << std::endl;
    }
};

int test_weak_ptr6()
{
    std::shared_ptr<Node> ptr = std::make_shared<Node>(4);
    ptr->leftPtr = std::make_shared<Node>(2);
    ptr->leftPtr->parentPtr = ptr;
    ptr->rightPtr = std::make_shared<Node>(5);
    ptr->rightPtr->parentPtr = ptr;
    std::cout << "ptr reference count = " << ptr.use_count() << std::endl;
    std::cout << "ptr->leftPtr reference count = " << ptr->leftPtr.use_count() << std::endl;
    std::cout << "ptr->rightPtr reference count = " << ptr->rightPtr.use_count() << std::endl;
    std::cout << "ptr->rightPtr->parentPtr reference count = " << ptr->rightPtr->parentPtr.lock().use_count() << std::endl;
    std::cout << "ptr->leftPtr->parentPtr reference count = " << ptr->leftPtr->parentPtr.lock().use_count() << std::endl;

    return 0;
}
int main()
{
//	weak_ptr_test_1();
//	weak_ptr_test_2();
//	test_weak_ptr3();
	test_weak_ptr4();
	test_weak_ptr5();
	test_weak_ptr6();
}
