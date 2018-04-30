#ifndef CALLBACK
#define CALLBACK
#endif
#include<Windows.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include<gl\GL.h>
#include<gl\GLU.H>
#include<gl\glut.h>
#include"QuadraticTriangleBezier.h"
#include<iostream>
#include"ArcBall.h"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")
#include"BSplineCurve.h"
using namespace std;

//********************************************************       鼠标控制定义
GLUquadricObj *quadratic;
const float PI2 = 2.0*3.1415926f;
Matrix4fT Transform ={
1.0f, 0.0f, 0.0f, 0.0f,				// NEW: Final Transform  初始化单位矩阵
0.0f, 1.0f, 0.0f, 0.0f,
0.0f, 0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 0.0f, 1.0f };

Matrix3fT   LastRot = { 
1.0f, 0.0f, 0.0f,					// NEW: Last Rotation
0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 1.0f };

Matrix3fT   ThisRot = {
1.0f, 0.0f, 0.0f,					// NEW: This Rotation
0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 1.0f };


ArcBallT    ArcBall(10.0f, 10.0f);				                    // NEW: ArcBall Instance
Point2fT    MousePt;												// NEW: Current Mouse Point
bool        isClicked = false;										// NEW: Clicking The Mouse?
bool        isRClicked = false;										// NEW: Clicking The Right Mouse Button?
bool        isDragging = false;
bool        isCenterClicked = false;                                 //鼠标中间的键值
GLdouble posX;
GLdouble posY;
GLdouble posZ;
//*********************************************************
//--------------------------------------------------------
GLfloat ctlpoints[4][4][3];
int showPoints = 0;
GLUnurbsObj *theNurb;
BSplineCurve bSplineCurve;
//--------------------------------------------------------
QuadraticTriangleBezier quadraticTriangleBezier;
vector<vector<vector<Vector3 >>>controlPoints;
//---------------------------------------------------------
DWORD				tickCount;
DWORD milliseconds;
DWORD				lastTickCount = GetTickCount();
float dt = 0;
//---------------------屏幕坐标到OpenGL坐标----------------------------------------
void pixelTranslateGL(int x, int y, GLdouble &posX, GLdouble &posY, GLdouble &posZ)
{
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 10086;

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0, 0, -1.5);
	//       glRotatef(angx,1,0,0);
	//       glRotatef(angy,0,1,0);
	//       glRotatef(angz,0,0,1);

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glPopMatrix();
	winX = (float)x;
	winY = viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(double(winX), double(winY), double(winZ), modelview, projection, viewport, &posX, &posY, &posZ);
}


//------------------------------------------------------------------------------
void init_surface()
{
	int u, v;
	for (u = 0; u < 4; u++)
	{
		for (v = 0; v < 4; v++)
		{
			ctlpoints[u][v][0] = 2 * ((GLfloat)u - 1.5);
			ctlpoints[u][v][1] = 2 * ((GLfloat)v - 1.5);

			if ((u == 1 || u == 2) && (v==1||v==2))
			{
				ctlpoints[u][v][2] = 3.0;
			}
			else
			{
				ctlpoints[u][v][2] = -3.0;
			}
		}
	}//for
	//------------------------------------------------------------
}
//---------------------------------------------------------
void init()
{
	GLfloat mat_diffuse[] = { 0, 0.6, 1, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };

	glClearColor(1, 1, 1, 1);
	// Start Of User Initialization
	isClicked = false;								            // NEW: Clicking The Mouse?
	isDragging = false;
	glClearDepth(1.0f);											// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_FLAT);											// Select Flat Shading (Nice Definition Of Objects)
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// Set Perspective Calculations To Most Accurate
	quadratic = gluNewQuadric();										// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);						// Create Smooth Normals
	gluQuadricTexture(quadratic, GL_TRUE);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	init_surface();
	theNurb = gluNewNurbsRenderer();
	
	GLfloat n = 20;
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE,n);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
	gluNurbsCallback(theNurb, GLU_ERROR,NULL);


	vector<Vector3 >P;
	vector<double >U;
	U.resize(8);
	P.resize(5);
	for (int i = 0; i <5; i++)
	{
		P[i] = *new Vector3(i, i*0.1, 0);
	}
	for (int i = 0; i < 8; i++)
	{
		U[i] = i / 7.0;
	}
    bSplineCurve = *new BSplineCurve(4, 2);   //4个控制顶点和2次曲线来表达。
	
	bSplineCurve.setP(P);
	bSplineCurve.setU(U);

	//---------------绘制二次三角Bezier曲面------------------------------------
	quadraticTriangleBezier = *new QuadraticTriangleBezier();
	controlPoints.resize(3);
	for (int i = 0; i < 3; i++)
	{
		controlPoints[i].resize(3);
		for (int j = 0; j < 3; j++)
		{
			controlPoints[i][j].resize(3);
		}
	}
	controlPoints[0][0][2] = *new Vector3(0, 2, 4);                //V002
	controlPoints[0][1][1] = *new Vector3(2, 4, 2);                  //V011

	controlPoints[0][2][0] = *new Vector3(4, 2, 0);                //V020
	controlPoints[1][1][0] = *new Vector3(2, 4, 0);                  //V110

	controlPoints[2][0][0] = *new Vector3(0, 2, 0);                //V200
	controlPoints[1][0][1] = *new Vector3(0, 4, 2);                  //V101

	
	quadraticTriangleBezier.setControlPoints(controlPoints);
	


}
//------------------------------------------------------------
void Deinitialize()
{
	gluDeleteQuadric(quadratic);
}
//------------------------------------------------------------
void physics(float dt)
{
	if (isRClicked)													// If Right Mouse Clicked, Reset All Rotations
	{
		Matrix3fSetIdentity(&LastRot);								// Reset Rotation
		Matrix3fSetIdentity(&ThisRot);								// Reset Rotation
		Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);		// Reset Rotation
	}

	if (!isDragging)												// Not Dragging
	{
		if (isClicked)												// First Click
		{
			isDragging = true;										// Prepare For Dragging
			LastRot = ThisRot;										// Set Last Static Rotation To Last Dynamic One
			ArcBall.click(&MousePt);								// Update Start Vector And Prepare For Dragging
		}
	}
	else
	{
		if (isClicked)												// Still Clicked, So Still Dragging
		{
			Quat4fT     ThisQuat;

			ArcBall.drag(&MousePt, &ThisQuat);						// Update End Vector And Get Rotation As Quaternion
			Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);		// Convert Quaternion Into Matrix3fT
			Matrix3fMulMatrix3f(&ThisRot, &LastRot);				// Accumulate Last Rotation Into This One
			Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);	// Set Our Final Transform's Rotation From This One
		}
		else														// No Longer Dragging
			isDragging = false;
	}
}
//------------------------------------------------------------
void display()
{
	glColor3f(0, 1, 1);
	GLfloat knots[8] = { 0, 0.1, 0.2, 0.4, 0.6, 0.8, 0.9, 1};
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(30, 30, 30, 0, 0, 0, 0, 1, 0);
	
   /*glPushMatrix();
   glMultMatrixf(Transform.M);
	//glRotatef(330, 1, 0, 0. );         //0. 也能表示浮点数
	glScaled(0.5, 0.5, 0.5);
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb, 8, knots, 8, knots, 4 * 3, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb);
	
	if (showPoints)
	{
		glPointSize(5.0);
		glDisable(GL_LIGHTING);
		glColor3f(1, 0,0);
		glBegin(GL_POINTS);
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				glVertex3f(ctlpoints[i][j][0], ctlpoints[i][j][1], ctlpoints[i][j][2]);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}//if展示控制顶点
	glPopMatrix();
	*/
	//------------------------------------------------
	/*Vector3 point;//=*new Vector3(0,0,0);
	glPointSize(4);
	glColor3f(1, 0, 0);
	for (float u = 0; u <1; u += 0.1)
	{
		//cout << "t"<<endl;
		point = bSplineCurve.paintBSplineCurve(u);
		glBegin(GL_POINTS);    //绘制X轴
		glVertex3f(point.x, point.y, point.z);
		glEnd();
	}
	*/
	glPushMatrix();
	glMultMatrixf(Transform.M);
	quadraticTriangleBezier.paintQuadraticTriangleBezier(controlPoints);

	if (showPoints)                                                                //绘制控制网格点
	{
		glPointSize(8);
		glColor3f(0.8, 0.6, 0.4);
		glBegin(GL_POINTS);

		int k = 0;
		for (i = 0; i <=2; i++)
		{
			for (j = 0; j <=2-i; j++)
			{
				k = 2 - i - j;
				glVertex3f(controlPoints[i][j][k].x, controlPoints[i][j][k].y, controlPoints[i][j][k].z);
			}
		}
		glEnd();
	}//if展示控制顶点
	glPopMatrix();
	//绘制三维坐标轴
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);    //绘制X轴
	glVertex3f(100, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);    //绘制Y轴
	glVertex3f(0, 100, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);    //绘制Z轴
	glVertex3f(0, 0, 100);
	glVertex3f(0, 0, 0);
	glEnd();
	glEnable(GL_DEPTH_TEST);                  //开启深度测试
	glutSwapBuffers();
	
                  //平移多少，则相应的减多少，让它返回到原点，因为是以原点为中心进行选择的


	glFlush();
}
//************************************************************************
void redisplay(int value)
{
	tickCount = GetTickCount();
	milliseconds = tickCount - lastTickCount;
	lastTickCount = tickCount;
	//dt = milliseconds / 1000.0f;                                                    //仿真时间dt 和fps可以设定
	//if (0.01 <= dt &&dt <= 0.1)                                                        //由于计算机时钟的不稳定，这里通过条件约束仿真时间间隔在30到40毫秒的范围内
	//{
	
	physics(0.033);                                                             //对面进行运动仿真，其实就是封装调用里面的运动函数  
	//}

	glutPostRedisplay();
	//cout <<"系统仿真时间： "<< GetTickCount() - tickCount << "毫秒"<<endl;                                                                             //这里进行回调反复绘制，同时，这里可以设置时间间隔dt,用于动画和碰撞处理
	glutTimerFunc(33, redisplay, 1);                                               //最后再次调用glutTimerFunc，因为glut的定时器是调用一次才产生一次定时，所以如果要持续产生定时的话， 在定时函数末尾再次调用glutTimerFunc
}
//------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case' ':showPoints = !showPoints;
		glutPostRedisplay();
		break;
	case 27:                            //27是ASCll中ESC的值
	{
			Deinitialize();
			   exit(0);
			   break;
	}
	default:
		break;
	}
}

//------------------------------------------------------------
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(10, (GLdouble)w / (GLdouble)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ArcBall.setBounds((GLfloat)w, (GLfloat)h);
	glTranslated(0, 0, 5);
}
//------------------------------鼠标事件---------------------
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON: {
							   if (state == GLUT_UP){ isClicked = false; }
							   else if (state == GLUT_DOWN){ isClicked = true;
							  // showPoints = !showPoints;
							   //glutPostRedisplay();
							   }
	} break;
	case GLUT_RIGHT_BUTTON:{
							   if (state == GLUT_UP){ isRClicked = false; }
							   else if (state == GLUT_DOWN){ isRClicked = true; 
							  
							   }
	}break;
	default:{
				if (state == GLUT_UP){ isCenterClicked = false; }
				else if (state == GLUT_DOWN){ isCenterClicked = true; }
	}
	}

}
//----------------------------------------------------------
void mouseMoveFunc(int x, int y)
{
	MousePt.s.X = x;
	MousePt.s.Y = y;
	if (isCenterClicked)
	{
		pixelTranslateGL(x, y,posX, posY, posZ);
         showPoints = !showPoints;              
	}
}

//***********************************************************
int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMoveFunc);
	glutTimerFunc(33, redisplay, 1);
	glutMainLoop();
	return 0;
}