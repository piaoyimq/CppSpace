#include <stdio.h>

int func2(int b)
{
	int ret;
	ret = b * b;
	return ret; 	
}

int func1(int c)
{
	int ret;
	ret = 10 + func2(c);
	return ret;	
}

void main()
{
	int i;
	long result = 0;
	for(i=1; i<=100; i++)
	{
		result += i;
	}
	printf("result = %d\n", result );
	result = func1(result);
} 