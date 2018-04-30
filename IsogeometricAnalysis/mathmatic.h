#ifndef mathmatic
#define mathmatic
#include <math.h>
#define EPSILON 1.0e-8
#define ZERO EPSILON
#define M_PI 3.1415926535
#define VC 1
#if VC
#define bool int
#define false 0
#define true !false
#endif

template <class T> inline T limit(const T &x, const T &lower, const T &upper)
{
	if (x < lower)
		return lower;
	if (x > upper)
		return upper;
	return x;
}
template <class T> inline T sqr(const T &x)
{
	return x*x;
}//定义一个阶乘模板函数
template<class T> inline T fac(const T  x)//注意这里不能取地址，因为以下返回的事f
{
	register int i, f = 1;
	for (i = 1; i <= x; i++)
	{
		f *= i;
	}
	return f;
}

template <class T> inline T RadToDeg(const T &rad)
{
	return (rad * 180.0) / M_PI;
}

template <class T> inline T DegToRad(const T &deg)
{
	return (deg * M_PI) / 180.0;
}

#endif
