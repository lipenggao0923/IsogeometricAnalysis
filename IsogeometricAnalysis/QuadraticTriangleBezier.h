/**
  绘制2次三角Bezier曲面
*/
#include<Windows.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include<gl\GL.h>
#include<gl\GLU.H>
#include<vector>
#include<iostream>
#include"Vector3.h"
using namespace std;
class QuadraticTriangleBezier
{
public:
	QuadraticTriangleBezier();
	~QuadraticTriangleBezier();

	vector<vector<vector<Vector3 >>>getControlPoints();
	void setControlPoints(vector<vector<vector<Vector3 >>> controlPoints);

	//----------Quadratic Triangle Bezier 的表达---------------------------------
	void paintQuadraticTriangleBezier(vector<vector<vector<Vector3 >>>controlPoints);
	Vector3 computerInterpolation(double u, double v, double w);
private:
	vector<vector<vector<Vector3 >>>controlPoints;
	
};