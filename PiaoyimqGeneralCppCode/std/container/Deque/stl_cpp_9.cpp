//stl_cpp_9.cpp
#include <iostream>
#include <deque>
using namespace std;
typedef deque<int> INTDEQUE;//��Щ�˺��������ֶ��巨���Ǻ�
//��ǰ�����ʾdeque���е�ȫ��Ԫ��
void put_deque(INTDEQUE deque, char *name)
{
    INTDEQUE::iterator pdeque;//��Ȼʹ�õ��������
    cout << "The contents of " << name << " : ";
    for(pdeque = deque.begin(); pdeque != deque.end(); pdeque++)
        cout << *pdeque << " ";//ע���� "*"��Ŷ��û��"*"�ŵĻ��ᱨ��
    cout<<endl;
}
//����deqtor�����Ĺ���
int main(void)
{
	//deq1�����ʼΪ��
    INTDEQUE deq1;   
    //deq2���������10��ֵΪ6��Ԫ��  
    INTDEQUE deq2(10,6);  
    //deq3���������3��ֵΪ6��Ԫ��  
    //����һ����Ϊi��˫�����������
    INTDEQUE::iterator i;
    //��ǰ�����ʾdeq1�е�����
    put_deque(deq1,"deq1");
    //��ǰ�����ʾdeq2�е�����
    put_deque(deq2,"deq2");
	//��deq1���к�����������Ԫ��
	deq1.push_back(2);
	deq1.push_back(4);
	cout<<"deq1.push_back(2) and deq1.push_back(4):"<<endl;
    put_deque(deq1,"deq1");
	//��deq1����ǰ����������Ԫ��
	deq1.push_front(5);
	deq1.push_front(7);
	cout<<"deq1.push_front(5) and deq1.push_front(7):"<<endl;
    put_deque(deq1,"deq1");
	//��deq1�����м��������
	deq1.insert(deq1.begin()+1,3,9);
	cout<<"deq1.insert(deq1.begin()+1,3,9):"<<endl;
    put_deque(deq1,"deq1");
	//���������ຯ��
	cout<<"deq1.at(4)="<<deq1.at(4)<<endl;
	cout<<"deq1[4]="<<deq1[4]<<endl;
	deq1.at(1)=10;
	deq1[2]=12;
	cout<<"deq1.at(1)=10 and deq1[2]=12 :"<<endl;
    put_deque(deq1,"deq1");
	//��deq1���е�ǰ�����ȥһ��Ԫ��
	deq1.pop_front();
	deq1.pop_back();
	cout<<"deq1.pop_front() and deq1.pop_back():"<<endl;
    put_deque(deq1,"deq1");
	//���deq1�еĵ�2��Ԫ��
	deq1.erase(deq1.begin()+1);
	cout<<"deq1.erase(deq1.begin()+1):"<<endl;
    put_deque(deq1,"deq1");
	//��deq2��ֵ����ʾ
	deq2.assign(8,1);
	cout<<"deq2.assign(8,1):"<<endl;
    put_deque(deq2,"deq2");
}