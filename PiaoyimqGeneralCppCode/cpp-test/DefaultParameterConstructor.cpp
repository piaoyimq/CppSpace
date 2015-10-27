#include <iostream>
using namespace std;

class Box
{
   public :
#if 1
   Box(int h=10,int w=10,int len=10); //在声明构造函数时指定默认参数，也可以省略形参名：Box(int=10,int=10,int=10);
#else
   Box(int 10,int 10,int 10); //在声明构造函数时指定默认参数
#endif

   //Box(){cout<<"test"<<endl;}//若定义了带默认形参的构造函数，就不可以再定义其它任何构造函数
   int volume( );
   private :
   int height;
   int width;
   int length;
};

Box::Box(int h,int w,int len) //在定义函数时可以不指定默认参数
{
   height=h;
   width=w;
   length=len;
   cout<<"height="<<height<<endl;
   cout<<"width="<<width<<endl;
   cout<<"length="<<length<<endl;
}

int Box::volume( )
{
   return (height*width*length);
}

int main( )
{
   Box box1; //没有给实参
   cout<<"The volume of box1 is "<<box1.volume( )<<endl;
   Box box2(15); //只给定一个实参
   cout<<"The volume of box2 is "<<box2.volume( )<<endl;
   Box box3(15,30); //只给定2个实参
   cout<<"The volume of box3 is "<<box3.volume( )<<endl;
   Box box4(15,30,20); //给定3个实参
   cout<<"The volume of box4 is "<<box4.volume( )<<endl;
   return 0;
}
