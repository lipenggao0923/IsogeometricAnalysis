

#include"QuadraticTriangleBezier.h"

QuadraticTriangleBezier::QuadraticTriangleBezier()
{
	controlPoints.resize(3);                                 //二次三角Bezier曲面的6个控制顶点
	for (int i = 0; i < 3; i++)
	{
		controlPoints[i].resize(3);
		for (int j = 0; j < 3; j++)
		{
			controlPoints[i][j].resize(3);
		}
	}
}
QuadraticTriangleBezier::~QuadraticTriangleBezier()
{
	                                  
}

vector<vector<vector<Vector3 >>> QuadraticTriangleBezier::getControlPoints()
{
	return controlPoints;
}
void QuadraticTriangleBezier::setControlPoints(vector<vector<vector<Vector3 >>> controlPoints)
{
	this->controlPoints = controlPoints;
}

void QuadraticTriangleBezier::paintQuadraticTriangleBezier(vector<vector<vector<Vector3 >>>controlPoints)
{
	this->controlPoints = controlPoints;                             //二次三角Bezier按逆时针方向存储
	double u, v, w;                                                  //特注：float 数1.0在计算机中的表示是无限接近于1，但是精度却小于1，而double类型的1.0却是可以与1相等
	vector<Vector3 >pointsDown, pointsUp;
	Vector3 A, B, C;
	double t = 0.01;
	int i=0;
	glColor3f(0, 0, 1);
	glPointSize(3);
	int count = 0;
	vector<Vector3 >::iterator it;
	for (u = 0; u <= 1; u += t)
	{
		for (v = 0; v <= 1 - u; v += t)
		{
			w = 1 - u - v;
			if (u == 0)
			{
			  pointsDown.push_back(computerInterpolation(u, v, w));          //我们可以插值计算出来的顶点存储起来,在返回一个坐标数组到主函数中，在进行集中绘制。
			}
			else
			{
				pointsUp.push_back(computerInterpolation(u, v, w));
			}
		}
		if (pointsUp.size() != 0)
		{
			glColor3f(0.2, 0.4, 0.8);
			for (int i = 0; i < pointsUp.size(); i++)                       //渲染正三角形
			{
				A = pointsUp[i];
				B = pointsDown[i];
				C = pointsDown[i + 1];
				glBegin(GL_TRIANGLES);
				glVertex3f(A.x, A.y, A.z);
				glVertex3f(B.x, B.y, B.z);
				glVertex3f(C.x, C.y, C.z);
				glEnd();
			}
			for (int i = 1; i < pointsDown.size() - 1; i++)                //渲染倒三角形
			{
				A = pointsDown[i];
				B = pointsUp[i - 1];
				C = pointsUp[i];
				glBegin(GL_TRIANGLES);
				glVertex3f(A.x, A.y, A.z);
				glVertex3f(B.x, B.y, B.z);
				glVertex3f(C.x, C.y, C.z);
				glEnd();
			}

			pointsDown = pointsUp;
		}
		pointsUp.clear();
	}
}

Vector3 QuadraticTriangleBezier::computerInterpolation(double u, double v, double w)
{
	Vector3 p;
	p = controlPoints[0][0][2]*w*w+controlPoints[0][1][1]*2*v*w+controlPoints[1][0][1]*2*u*w+controlPoints[0][2][0]*v*v+controlPoints[1][1][0]*2*u*v+controlPoints[2][0][0]*u*u;
	return p;
}