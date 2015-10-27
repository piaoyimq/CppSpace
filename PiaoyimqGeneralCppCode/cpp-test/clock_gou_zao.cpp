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




int main_gou_zao()
{
	Clock clock(11,20,49);

	Clock a_clock;
	int h,m,s;

	cout<<"The default time:"<<endl;
	clock.show_time();

	cout<<"Please input hour,min,sec:\n";
	cin>>h>>m>>s;

	clock.set_time(h,m,s);
	cout<<"Show time:"<<endl;
	clock.show_time();
	
}

