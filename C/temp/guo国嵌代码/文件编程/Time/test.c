#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

int main()
{
	#if  0
	time_t t,s;
	struct tm p,*ptr;
	
	p.tm_year=2011-1900;
	p.tm_mon=9-1;
	p.tm_mday=10;
	p.tm_hour=12;
	p.tm_min=59;
	p.tm_sec=17;

	t=mktime(&p);//把普通时间转换成秒数，与asctime连用
	 
	//s=1378832357;
	
	ptr=gmtime(&s);//把秒数转换成普通时间，与ctime连用，与mktime相反
	
	printf(asctime(ptr));//asctime把普通时间转换成普通时间字符串
	
  //printf(ctime(&t));//ctime把秒数转换成
	
	//printf(ctime(&s));//ctime把秒数转换成普通时间字符串，即：年、月、日、时、分、秒那种
	

	printf("t=%d\n",t);
	printf(asctime(&p));//asctime把普通时间转换成普通时间字符串
	
	#endif
		
}