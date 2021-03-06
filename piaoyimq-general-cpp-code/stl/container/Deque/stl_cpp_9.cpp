//stl_cpp_9.cpp
#include <iostream>
#include <deque>
using namespace std;
typedef deque<int> INTDEQUE;//有些人很讨厌这种定义法，呵呵
//从前向后显示deque队列的全部元素
void put_deque(INTDEQUE deque, char *name)
{
    INTDEQUE::iterator pdeque;//仍然使用迭代器输出
    cout << "The contents of " << name << " : ";
    for(pdeque = deque.begin(); pdeque != deque.end(); pdeque++)
        cout << *pdeque << " ";//注意有 "*"号哦，没有"*"号的话会报错
    cout<<endl;
}
//测试deqtor容器的功能
int main(void)
{
	//deq1对象初始为空
    INTDEQUE deq1;   
    //deq2对象最初有10个值为6的元素  
    INTDEQUE deq2(10,6);  
    //deq3对象最初有3个值为6的元素  
    //声明一个名为i的双向迭代器变量
    INTDEQUE::iterator i;
    //从前向后显示deq1中的数据
    put_deque(deq1,"deq1");
    //从前向后显示deq2中的数据
    put_deque(deq2,"deq2");
	//从deq1序列后面添加两个元素
	deq1.push_back(2);
	deq1.push_back(4);
	cout<<"deq1.push_back(2) and deq1.push_back(4):"<<endl;
    put_deque(deq1,"deq1");
	//从deq1序列前面添加两个元素
	deq1.push_front(5);
	deq1.push_front(7);
	cout<<"deq1.push_front(5) and deq1.push_front(7):"<<endl;
    put_deque(deq1,"deq1");
	//在deq1序列中间插入数据
	deq1.insert(deq1.begin()+1,3,9);
	cout<<"deq1.insert(deq1.begin()+1,3,9):"<<endl;
    put_deque(deq1,"deq1");
	//测试引用类函数
	cout<<"deq1.at(4)="<<deq1.at(4)<<endl;
	cout<<"deq1[4]="<<deq1[4]<<endl;
	deq1.at(1)=10;
	deq1[2]=12;
	cout<<"deq1.at(1)=10 and deq1[2]=12 :"<<endl;
    put_deque(deq1,"deq1");
	//从deq1序列的前后各移去一个元素
	deq1.pop_front();
	deq1.pop_back();
	cout<<"deq1.pop_front() and deq1.pop_back():"<<endl;
    put_deque(deq1,"deq1");
	//清除deq1中的第2个元素
	deq1.erase(deq1.begin()+1);
	cout<<"deq1.erase(deq1.begin()+1):"<<endl;
    put_deque(deq1,"deq1");
	//对deq2赋值并显示
	deq2.assign(8,1);
	cout<<"deq2.assign(8,1):"<<endl;
    put_deque(deq2,"deq2");
}
