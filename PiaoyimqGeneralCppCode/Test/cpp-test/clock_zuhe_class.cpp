/*open this file with UTF8 coding format.
 *组合类的构造函数，拷贝构造函数，析构函数执行顺序*/

#include <iostream>
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
	cout<<"Clock init \n";
}


Clock::Clock()
{
//	printf("no paramer\n");	
	cout<<"Clock init no paramer\n";
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









class Clock_2
{
	public:
		Clock_2(int H,int M,int S);//æž„é€ å‡½æ•°
		Clock_2();//æž„é€ å‡½æ•°
		Clock_2(Clock_2 &time);//æ‹·è´�æž„é€ å‡½æ•°
		~Clock_2();

	private:
		int hour,min,sec;
};



Clock_2::Clock_2(int H,int M,int S)
{
	hour=H;
	min=S;
	sec=S;
	cout<<"Clock_2 init \n";
}


Clock_2::Clock_2()
{
//	printf("no paramer\n");
	cout<<"Clock_2 init no paramer\n";
}


Clock_2::Clock_2(Clock_2 &time)
{
	hour=time.hour;
	min=time.min;
	sec=time.sec;

	cout<<"Clock_2 copy \n";

}


Clock_2::~Clock_2()
{
	cout<<"Clock_2 over\n";
}



class Date
{
	public:
		Date(int Y,int M ,int D);
		Date(Date &date);
		Date(){}
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
	cout<<"Date init"<<endl;
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



class Date_2
{
        public:
                Date_2(int Y,int M ,int D); 
                Date_2(Date_2 &date);
                Date_2(){}
                ~Date_2();
                void show_date();
    
        private:
                int year,month,day;
};




Date_2::Date_2(int Y,int M ,int D)
{
        year=Y;
        month=M;
        day=D;
        cout<<"Date_2 init"<<endl;
}



Date_2::Date_2(Date_2 &date)
{
        cout<<"Date_2 copy\n";

        year=date.year;
        month=date.month;
        day=date.day;
}



Date_2::~Date_2()
{
      cout<<" Date_2 over\n";
}





class Time
{
	public:
//		Time(Clock ck,Date de,Date_2 d2);
		Time(Clock_2 ck2,Date_2 d2,Date de,Clock ck);
		Time(Time &tm);
		Time(){cout<<"Time default"<<endl;}
		~Time();
		void show_time();
		static int show_static_data();
		int c;

	private:
		static int a;//ç±»ä¸­å�„å¯¹è±¡çš„å…±äº«æˆ�å‘˜
		int b;
		Date_2 my_date_2;
		Clock my_clock;
		Date my_date;
};

//int Time::a=100;
#if 0
Time::Time(Clock ck,Date de,Date_2 d2):my_clock(ck),my_date(de),my_date_2(d2)
{
	cout<<"Time init\n";
	b=10;
}
#else
Time::Time(Clock_2 ck2,Date_2 d2,Date de,Clock ck):my_clock(ck),my_date(de),my_date_2(d2)
{/*Time(Clock_2 ck2,Date_2 d2,Date de,Clock ck)里是通过拷贝构造函数生成的临时对象，所以生存期结束时，也要对其进行析构*/
	cout<<"Time init\n";
	b=10;
}
#endif

#if 1
//Time::Time(Time &tm):my_clock(tm.my_clock),my_date(tm.my_date)
Time::Time(Time &tm):my_date(tm.my_date),my_date_2(tm.my_date_2),my_clock(tm.my_clock)
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
	my_date.show_date();
	my_clock.show_clock();

	cout<<"b="<<b<<endl;
	cout<<"a="<<a<<endl;
}

int Time::show_static_data()
{
	return a;
}

int Time::a=100;//int ä¸�å�¯ä»¥ç¼ºå°‘


int main()
{
	Clock a_clock(11,20,49);
	Clock_2 a_clock_2(11,20,49);
	Date_2 a_date_2(2011,1,1);
	Date a_date(2013,3,8);
	//上述4个对象的析构顺序刚好跟声明顺序相反


	Time a_time(a_clock_2,a_date_2,a_date,a_clock);

	cout<<"--end--"<<endl;
	/*此处调用Time的构造函数,但Time的构造函数调用之前先按
	"Time(Clock_2 ck2,Date_2 d2,Date de,Clock ck):my_clock(ck),my_date(de),my_date_2(d2)"进行拷贝函数调用，
	然后再调用Time构造函数的函数体里的内容"Time init"。
	此处拷贝构造函数调用顺序为：先进行函数参数形实结合，然后初始化内嵌对象的拷贝构造。
	其中形实结合的调用顺序是按照参数声明顺序“Time(Clock_2 ck2,Date_2 d2,Date de,Clock ck);”从右往左：Clock,Date,Date_2，Clock_2
	内嵌对象调用顺序是按照其声明顺序：
	“	Date_2 my_date_2;
		Clock my_clock;
		Date my_date;
	”从上到下	.

	由于构造函数初始化时，使用了通过拷贝构造函数生成了临时对象Time(Clock_2 ck2,Date_2 d2,Date de,Clock ck)，
	所以当该构造函数结束时，会立即调用析构函数先析构了这4个临时对象，顺序刚好相反:Clock_2,Date_2,Date,Clock.
	*/



#if 0
	Time b_time(a_time);
	b_time.c=99;
	a_time.show_time();
	
	b_time.show_time();


	cout<<"Time::a1="<<b_time.show_static_data()<<endl;
	cout<<"Time::a2="<<Time::show_static_data()<<endl;
//	cout<<"Time::a3"<<b_time.a<<endl;//error
	cout<<"Time::c="<<b_time.c<<endl;
#endif
}
	

/*
 * 该程序中声明了4个对象：a_clock，a_clock_2，a_date_2,a_data,a_time.
 * 析构时，刚好与此相反。
 *
Clock init
Clock_2 init
Date_2 init
Date init

调用Time的构造函数
Clock copy
Date copy
Date_2 copy
Clock_2 copy--->形参与实参结合，调用拷贝构造函数
Date_2 copy
Clock copy
Date copy-->调用拷贝构造函数，初始化内嵌对象
Time init-->调用函数体内的内容

临时对象析构
Clock_2 over
 Date_2 over
Date over
Clock over
--end--

析构声明的对象：

析构Time:
Time over
Date over
Clock over
 Date_2 over

Date over
Date_2 over
Clock_2 over
Clock over
 *
 *
 * */
