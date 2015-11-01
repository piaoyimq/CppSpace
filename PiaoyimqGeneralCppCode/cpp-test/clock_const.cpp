#include <iostream>
//#include <stdio.h>
using namespace std;

class Clock
{
	public:
		Clock(int H,int M,int S);
		Clock();
		void set_time(int Hour=0,int Min=0,int Sec=0);
		void show_time();
		void show_time_2() const;
		int pub;
	private:
		int hour,min,sec;
};



Clock::Clock(int H,int M,int S)
{
	hour=H;
	min=S;
	sec=S;
	pub=99;
}


Clock::Clock()
{
//	printf("no paramer\n");	
	cout<<"no paramer\n";
}



void Clock::set_time(int Hour,int Min,int Sec)
{
	hour=Hour;
	min=Min;
	sec=Sec;
}




 void Clock::show_time()
{
	cout<<hour<<":"<<min<<":"<<sec<<endl;
}



void Clock::show_time_2() const
{
	cout<<"const show_time"<<endl;
	cout<<hour<<":"<<min<<":"<<sec<<endl;
}



int main_clock_const()
{
	Clock clock(11,20,49);
	
	const Clock b_clock(12,13,14);

	Clock a_clock;

	cout<<"The default time:"<<endl;
	clock.show_time();

	clock.pub=78;
//	b_clock.pub=100;
	cout<<"b_clock.pub="<<b_clock.pub<<endl;
	b_clock.show_time_2();
//	b_clock.show_time();//error

	clock.show_time_2();
	cout<<"clock.pub="<<clock.pub<<endl;

	
}

