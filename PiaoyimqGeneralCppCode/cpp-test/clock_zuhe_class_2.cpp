#include <iostream>
#include <unistd.h>
//#include <stdio.h>
using namespace std;

class Clock
{
	public:
		Clock(int H,int M,int S);//æž„é€ å‡½æ•°
		Clock();//æž„é€ å‡½æ•°
		Clock(Clock &time);//æ‹·è´�æž„é€ å‡½æ•°
		~Clock();
		void set_time(int Hour=0,int Min=0,int Sec=0);
		void show_clock();
		
	private:
		int hour,min,sec;
};



Clock::Clock(int H,int M,int S)
{
	hour=H;
	min=S;
	sec=S;
	cout<<"Clock init\n";
}


Clock::Clock()
{
//	printf("no paramer\n");	
	cout<<"Clock default init\n";
}


Clock::Clock(Clock &time)
{
	hour=time.hour;
	min=time.min;
	sec=time.sec;
	
	cout<<"Clock copy \n";

}

Clock::~Clock()
{
	cout<<"Clock over\n";
}

void Clock::set_time(int Hour,int Min,int Sec)
{
	hour=Hour;
	min=Min;
	sec=Sec;
}





inline void Clock::show_clock()
{
	cout<<"Clock:"<<hour<<":"<<min<<":"<<sec<<endl;
}



class Date
{
	public:
		Date(int Y,int M ,int D);
		Date(Date &date);
		~Date();
		void show_date();
	
	private:
		int year,month,day;
};




Date::Date(int Y,int M ,int D)
{
	year=Y;
	month=M;
	day=D;
	cout<<"Date init\n";
}



Date::Date(Date &date)
{
	cout<<"Date copy\n";

	year=date.year;
	month=date.month;
	day=date.day;
}



Date::~Date()
{
	cout<<"Date over\n";
}




void Date::show_date()
{
	cout<<"date: "<<year<<":"<<month<<":"<<day<<endl;
}



class Time
{
	public:
		Time(Clock ck);
		Time(Time &tm);
		int test(Clock &ck){//对象引用，这里既不会调用clock的构造函数，也不会调用其拷贝构造函数
			cout<<"test"<<endl;
		}
		~Time();
		void show_time();

	private:
		//static int a;
		int b;
		Clock my_clock;
};

//int Time::a=100;

#if 0
Time::Time(Clock ck):my_clock(ck)//my_clock(ck)这里会通过拷贝构造函数来初始化内嵌对象my_clock.
{//Time(Clock ck)是通过拷贝构造函数生成的临时对象，也要进行析构
	cout<<"Time init\n";
	b=10;

}
#else
Time::Time(Clock ck)
{//Time(Clock ck)是通过拷贝构造函数生成的临时对象，也要进行析构
	cout<<"Time init\n";
	b=10;

}
#endif

#if 1
//Time::Time(Time &tm):my_clock(tm.my_clock),my_date(tm.my_date)
Time::Time(Time &tm):my_clock(tm.my_clock)
{
	cout<<"Time copy \n";	
	
	b=tm.b;
		
	//my_clock=tm.my_clock;
	//my_date=tm.my_date;	
}

#endif



Time::~Time()
{
	cout<<"Time over\n";
}


void Time::show_time()
{
	my_clock.show_clock();

	cout<<"b="<<b<<endl;
}




int main()
{

// 这三个对象的消亡顺序不能确定
	Clock a_clock(11,20,49);

//	Clock b(a_clock);//此处调用了拷贝构造函数生成一个对象，就不会再调用普通的构造函数。但无论是调用了普通构造函数还是拷贝构造函数生成的对象，都要对其进行析构。

	cout<<"1\n";

	Time a_time(a_clock);
	cout<<"2\n";

	a_time.test(a_clock);
//	Date a_date(2013,3,8);
	cout<<"3\n";



#if 0
	Time b_time(a_time);

	a_time.show_time();
	
	b_time.show_time();
#endif
}
	

