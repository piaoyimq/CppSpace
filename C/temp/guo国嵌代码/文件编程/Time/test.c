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

	t=mktime(&p);//����ͨʱ��ת������������asctime����
	 
	//s=1378832357;
	
	ptr=gmtime(&s);//������ת������ͨʱ�䣬��ctime���ã���mktime�෴
	
	printf(asctime(ptr));//asctime����ͨʱ��ת������ͨʱ���ַ���
	
  //printf(ctime(&t));//ctime������ת����
	
	//printf(ctime(&s));//ctime������ת������ͨʱ���ַ����������ꡢ�¡��ա�ʱ���֡�������
	

	printf("t=%d\n",t);
	printf(asctime(&p));//asctime����ͨʱ��ת������ͨʱ���ַ���
	
	#endif
		
}