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
		void show_time();
		
	private:
		int hour,min,sec;
};



Clock::Clock(int H,int M,int S)
{
	hour=H;
	min=S;
	sec=S;
}


Clock::Clock()
{
//	printf("no paramer\n");	
	cout<<"no paramer\n";
}


Clock::Clock(Clock &time)
{
	hour=time.hour;
	min=time.min;
	sec=time.sec;
	
	cout<<"copy \n";

}

Clock::~Clock()
{
	cout<<"over\n";
}

void Clock::set_time(int Hour,int Min,int Sec)
{
	hour=Hour;
	min=Min;
	sec=Sec;
}





inline void Clock::show_time()
{
	cout<<hour<<":"<<min<<":"<<sec<<endl;
}


void fs_1(Clock a)
{
	a.show_time();
}


//Clock fs_2(int tt)
Clock fs_2()
{
	Clock c_clock(999,0,0);
	c_clock.show_time();
	return c_clock;

}

int main_copy_gouzao()
{
	Clock a_clock(11,20,49);

	Clock d_clock;
	cout<<"1-1\n";
	Clock b_clock(a_clock);
	
	cout<<"1-2\n";
	fs_1(a_clock);

	cout<<"1-3\n";
	a_clock=fs_2();	//æ­¤å¤„ä¸ºä½•æ²¡æœ‰è°ƒç”¨æ‹·è´�æž„é€ å‡½æ•°?????
	a_clock.show_time();

}

