#pragma once
#include<iostream>
#include"Vector3.h"

#include<vector>
using namespace std;
class BSplineCurve
{
public:
	BSplineCurve(int n,int p);
	BSplineCurve();
	~BSplineCurve();
	vector<double >getU();
	void setU(vector<double >U);
	vector<Vector3 >getP();
	void setP(vector<Vector3 >P);
	//------------------------曲线的绘制-------------------------------
	Vector3 paintBSplineCurve(double u);                                  //给定一个u值，返回曲线上的一个向量坐标
	double computerN(int i,int p,double u);
	int get_m();
	void set_m(int m);

	int get_n();
	void set_n(int n);

	int get_p();
	void set_p(int p);
private:                                                                                                                                                         
	vector<double >U;                                            //注意，要用vector函数时，一定先包含该函数所在的类和命名空间，不然显示不出来
	vector<Vector3 >P;                                           //定义一个控制顶点序列数组P
	int m;          //节点数为m+1,所以定义节点向量数组大小为m+1.U[m]则为最后一个节点。
	int n;          //控制顶点数为n+1,则控制顶点数组大小为P[n+1],P[n]则为最后一个控制顶点。
	int p;          //参数曲面的次数,p>=0
};

