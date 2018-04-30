#include<math.h>
#include"mathmatic.h"
#pragma once
using namespace std;
class Vector3
{
public:
	float x;									// the x value of this Vector3D
	float y;									// the y value of this Vector3D
	float z;									// the z value of this Vector3D

	Vector3()									// Constructor to set x = y = z = 0
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3(float x, float y, float z)			// Constructor that initializes this Vector3D to the intended values of x, y and z
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3& operator= (Vector3 v)			// operator= sets values of v to this Vector3D. example: v1 = v2 means that values of v2 are set onto v1
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3 operator+ (Vector3 v)				// operator+ is used to add two Vector3D's. operator+ returns a new Vector3D
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}


	Vector3 operator- (Vector3 v)				// operator- is used to take difference of two Vector3D's. operator- returns a new Vector3D
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator* (float value)			// operator* is used to scale a Vector3D by a value. This value multiplies the Vector3D's x, y and z.
	{
		return Vector3(x * value, y * value, z * value);
	}

	Vector3 operator/ (float value)			// operator/ is used to scale a Vector3D by a value. This value divides the Vector3D's x, y and z.
	{
		return Vector3(x / value, y / value, z / value);
	}

	Vector3& operator+= (Vector3 v)			// operator+= is used to add another Vector3D to this Vector3D.
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3& operator-= (Vector3 v)			// operator-= is used to subtract another Vector3D from this Vector3D.
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3& operator*= (float value)			// operator*= is used to scale this Vector3D by a value.
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vector3& operator/= (float value)			// operator/= is used to scale this Vector3D by a value.
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}


	Vector3 operator- ()						// operator- is used to set this Vector3D's x, y, and z to the negative of them.
	{
		return Vector3(-x, -y, -z);
	}

	float length()								// length() returns the length of this Vector3D
	{
		return sqrtf(x*x + y*y + z*z);
	};

	void unitize()								// unitize() normalizes this Vector3D that its direction remains the same but its length is 1.   //向量的单位化
	{
		float length = this->length();

		if (length == 0)
			return;

		x /= length;
		y /= length;
		z /= length;
	}

	Vector3 unit()								// unit() returns a new Vector3D. The returned value is a unitized version of this Vector3D.
	{
		float length = this->length();

		if (length == 0)
			return *this;

		return Vector3(x / length, y / length, z / length);
	}
	//****************************************
	double mag()
	{

		return 	 sqrt(sqr(x) + sqr(y) + sqr(z));
	}

	//定义点乘
	double dot(Vector3 &v)
	{
		return x*v.x + y*v.y + z*v.z;
	}

	//dist运算
	double dist(Vector3 &v)
	{
		return (*this - v).mag();
	}

	void invert()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	Vector3 cross(Vector3 b)
	{
		Vector3 result = *new Vector3((y*b.z - b.y*z), (x*b.z - b.x*z), (x*b.y - b.x*y));
		return result;                                 //返回叉乘向量的法向量
	}

	double maxAxisValue()                      //返回坐标轴最大的值
	{
		if (abs(y) > abs(z))
		{
			if (abs(y) > abs(x))
			{
				return y;
			}
			else
			{
				return x;
			}
		}
		else
		{
			if (abs(z) > abs(x))
			{
				return z;
			}
			else
			{
				return x;
			}
		}
	}
};