#include <iostream>
using namespace std;

class Clock
{
	public:
		void set_time(int Hour=0,int Min=0,int Sec=0);
		void show_time();
	private:
		int hour,min,sec;
};






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




int main_clock()
{
	Clock clock;

	int h,m,s;

	cout<<"The default time:"<<endl;
	clock.set_time();
	clock.show_time();

	cout<<"Please input hour,min,sec:\n";
	cin>>h>>m>>s;

	clock.set_time(h,m,s);
	cout<<"Show time:"<<endl;
	clock.show_time();
	
}

