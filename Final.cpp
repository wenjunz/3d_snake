#include <stdlib.h>
#include <GL/freeglut.h>
#include <GL/glui.h>
#include "MyCamera.h"
#include "Block.h"
#include "Geometry.h"
#include "Snake.h"


static GLint windowWidth = 800;
static GLint windowHeight = 800;
GLfloat perspective; // ratio


MyCamera mycamera;
Snake snake;

/* MyInit */
void MyInit( void ) // also initialize the glui
{
    /* set background color to black */
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void CameraInit(void)
{
	mycamera.Init(GLUT_LEFT_BUTTON, SIZE, 60);

	/* lighting */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	
	//glClearColor(0.0, 0.0, 0.0, 0.0);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	/* menu */
/*  int o_menu = glutCreateMenu(menu);
	glutAddMenuEntry("Rotate",	        ROTATE);
	glutAddMenuEntry("Zoom",              ZOOM);
	glutAddMenuEntry("Twist",             TWIST);
	glutAddMenuEntry("Pan",               PAN);
	glutAddMenuEntry("Capture View",      CAPTURE);
	glutAddMenuEntry("Replay View",       REPLAY);
	//glutAddMenuEntry("View all",          VIEWALL);

	// Trigger this menu by right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
*/
}

/* MyDisplay */
void MyDisplay( void )
{
    /* handle draw events */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(0,0,windowWidth,windowHeight);
	glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
	glLoadIdentity();
	mycamera.Matrix();
	glColor3f(1.0, 1.0, 1.0);
    glPointSize(4.0);
	snake.DrawAll();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glViewport(0,0,windowWidth,200);
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(snake.eye_x,snake.eye_y,snake.eye_z,snake.look_x,snake.look_y,snake.look_z,snake.up_x,snake.up_y,snake.up_z);
	snake.DrawAll();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
		
	glutSwapBuffers();
}


void MyReshape( GLint width, GLint height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0,0,windowWidth,windowHeight);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60,windowWidth/windowHeight,1,100);
	mycamera.Reshape(windowWidth,windowHeight);
	glutPostRedisplay();
}

void MyMouseButton( GLint button, GLint state, GLint x, GLint y )
{
	mycamera.Mouse(button, state, x, y);
	glutPostRedisplay();
}

void MyMouseMotion( GLint x, GLint y )
{
    mycamera.Motion(x,y);
	glutPostRedisplay();
}

void MyKeyboard( unsigned char key, int x, int y )
{
	snake.Control(key);
	glutPostRedisplay();
}

void MyTimer( int dummy ) // 10ms every time
{
	//snake.HitTest();
	snake.Timer();
	glutTimerFunc(10,MyTimer,0);
	glutPostRedisplay();
}

/* main */
int main( int argc, char** argv )
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 150);
	int main_window = glutCreateWindow("Snake");

	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMouseFunc(MyMouseButton);
    glutMotionFunc(MyMouseMotion);
	glutKeyboardFunc(MyKeyboard);
	glutTimerFunc(100,MyTimer,0);
	MyInit();
	CameraInit();

	glutMainLoop();
}
