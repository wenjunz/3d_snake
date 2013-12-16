#include "MyCamera.h"
#include <cmath>
#include <assert.h>

using namespace std;

MyCamera::MyCamera(void)
{
    tb_mode = ROTATE; // default is the rotation mode
    tb_currPos[0] = 0.0f;
    tb_currPos[1] = 0.0f;
    tb_prevPos[0] = 0.0f;
    tb_prevPos[1] = 0.0f;
    tb_moving = GL_FALSE;
    tb_width = 1;
    tb_height = 1;
}

MyCamera::~MyCamera(void)
{
}

void MyCamera::Init(GLuint button, GLfloat radius, GLfloat viewAngle)
{
    tb_button = button;

    /* put the identity in the trackball transform */
    glPushMatrix();
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_accuTransform);
    glPopMatrix();

	tb_viewAngle = viewAngle;
/*	GLfloat radius = sqrt(min[0]*min[0]+min[1]*min[1]+min[2]*min[2])
		> sqrt(max[0]*max[0]+max[1]*max[1]+max[2]*max[2])
		? sqrt(min[0]*min[0]+min[1]*min[1]+min[2]*min[2])
		: sqrt(max[0]*max[0]+max[1]*max[1]+max[2]*max[2]);
*/	tb_accuTransform[14] = -radius / sin( tb_viewAngle*0.017453292519943/2 );
}

void MyCamera::SetMode(int mode)
{
    switch (mode)
	{
		case VIEWALL: break;
		case CAPTURE: break;
		case REPLAY: break;
		default: tb_mode = mode;
	}
}

void MyCamera::Matrix()
{
	assert(tb_button != -1);

	glMatrixMode(GL_MODELVIEW);
	// multiple the current one with the previous transformation
	glPushMatrix();
	glLoadIdentity();

	// Get the current translation
	float accRotation[16];
	for(int i=0; i<16;i++) accRotation[i] = tb_accuTransform[i];

	// make accRotation as a rotation only matrix
	accRotation[12] = 0.0;
	accRotation[13] = 0.0;
	accRotation[14] = 0.0;

	glTranslatef(tb_accuTransform[12], tb_accuTransform[13], tb_accuTransform[14]); // Translation later
	glMultMatrixf((GLfloat *)tb_transform);                     // Addition transformation second
	glMultMatrixf((GLfloat *)accRotation);                      // Pure rotation first
  
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_accuTransform);
	glPopMatrix();

	// Set to the current matrix
	glMultMatrixf((GLfloat *)tb_accuTransform);
}

void MyCamera::Reshape(int width, int height)
{
	assert(tb_button != -1);

	tb_width  = width;
	tb_height = height;
}

void MyCamera::Mouse(int button, int state, int x, int y)
{
	assert(tb_button != -1);

	// GLUT_DOWN == 0; GLUT_UP == 1
	if (state == 0 && button == tb_button)
	{
		tb_currPos[0] = (float)x;
		tb_currPos[1] = (float)y;
		tb_prevPos[0] = (float)x;
		tb_prevPos[1] = (float)y;
		tb_moving = GL_TRUE;
	}
	else if (state == 1 && button == tb_button)
	{
		tb_moving = GL_FALSE; 
		for(int i=0;i<16;i++) // reset the tb_transform matrix to be identity matrix
		{
			tb_transform[i] = 0;
		}
		tb_transform[0] = 1;
		tb_transform[5] = 1;
		tb_transform[10] = 1;
		tb_transform[15] = 1;
	}
}

void MyCamera::Motion(int x, int y)
{
    if (!tb_moving ) return;

    tb_currPos[0] = (float)x;
    tb_currPos[1] = (float)y;

    float dx = ( tb_currPos[0] - tb_prevPos[0] ) / (float)tb_width;
	float dy = ( tb_currPos[1] - tb_prevPos[1] ) / (float)tb_height;
	float mag = sqrtf(dx * dx  + dy * dy);

    if ( mag < 1.0e-6f ) return;

    switch (tb_mode)
    {
		case ROTATE: _rotate(dx, dy); break;
		case ZOOM: _zoom(dx, dy); break;
		case TWIST: _twist(dx, dy); break;
		case PAN: _pan(dx, dy); break;
		default: break;
    }

    tb_prevPos[0] = tb_currPos[0];
    tb_prevPos[1] = tb_currPos[1];
}

void MyCamera::ViewAll(float radius)
{
/*	GLfloat radius = sqrt(min[0]*min[0]+min[1]*min[1]+min[2]*min[2])
		> sqrt(max[0]*max[0]+max[1]*max[1]+max[2]*max[2])
		? sqrt(min[0]*min[0]+min[1]*min[1]+min[2]*min[2])
		: sqrt(max[0]*max[0]+max[1]*max[1]+max[2]*max[2]);
*/
	tb_accuTransform[14] = -radius / sin( tb_viewAngle*0.017453292519943/2 );
}

void MyCamera::Capture(void)
{
	GLfloat* m = new GLfloat [16];
	for(int i=0;i<16;i++)
	{
		m[i] = tb_accuTransform[i];
	//	printf("%f\n",m[i]);
	}
	tb_record.push_back(m);
	//int n = tb_record.size();
}

void MyCamera::Replay(void)
{
	if(tb_record.size() == 0) return;
	for(int i=0;i<16;i++)
	{
		tb_accuTransform[i] = tb_record.at(0)[i];
	}
	delete tb_record.at(0);
	tb_record.erase(tb_record.begin());

}

void MyCamera::_getWorldXYZ(float* axis)
{
    float sx[3], sy[3], sz[3];
	float mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)mat);
    sx[0] = mat[0];    sx[1] = mat[4];    sx[2] = mat[8];
    sy[0] = mat[1];    sy[1] = mat[5];    sy[2] = mat[9];
    sz[0] = mat[2];    sz[1] = mat[6];    sz[2] = mat[10];
	
	tb_axis[0] = sx[0] * axis[0] + sy[0] * axis[1] + sz[0] * axis[2];
    tb_axis[1] = sx[1] * axis[0] + sy[1] * axis[1] + sz[1] * axis[2];
    tb_axis[2] = sx[2] * axis[0] + sy[2] * axis[1] + sz[2] * axis[2];
}

void MyCamera::_rotate(float dx, float dy)
{
	float axis[3];
	float mag = sqrtf( dx * dx  + dy * dy );
	tb_angle = mag * 200.0f;

	axis[0] = dy;
	axis[1] = dx;
	axis[2] = 0.0f;

	_getWorldXYZ(axis);
	_atRotate();
}

void MyCamera::_atRotate(void)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(tb_angle, tb_axis[0], tb_axis[1], tb_axis[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
	glPopMatrix();
}

void MyCamera::_zoom(float dx, float dy)
{
    float axis[3];

	axis[0] = 0;
	axis[1] = 0;
	axis[2] = dy*50.0f;

	_getWorldXYZ(axis);
	_atZoom();
}

void MyCamera::_atZoom(void)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(tb_axis[0], tb_axis[1], tb_axis[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
	glPopMatrix();
}

void MyCamera::_twist(float dx, float dy)
{
	float axis[3];
	float mag = sqrtf( dx * dx  + dy * dy );
	
	axis[0] = 0.0f;
	axis[1] = 0.0f;
	axis[2] = 1.0f;

	float sign = (tb_prevPos[0]-tb_width/2) * (tb_currPos[1] - tb_height/2)
		- (tb_currPos[0]-tb_width/2) * (tb_prevPos[1]-tb_height/2);
    if ( abs(sign) < 1.0e-6f ) return;

	sign = -sign / abs(sign);

	tb_angle = (tb_prevPos[0]-tb_width/2) * (tb_currPos[0]-tb_width/2) 
		+ (tb_prevPos[1]-tb_height/2) * (tb_currPos[1]-tb_height/2);
	tb_angle = tb_angle/sqrt((tb_prevPos[0]-tb_width/2)*(tb_prevPos[0]-tb_width/2)
		+ (tb_prevPos[1]-tb_height/2)*(tb_prevPos[1]-tb_height/2));
	tb_angle = tb_angle/sqrt((tb_currPos[0]-tb_width/2)*(tb_currPos[0]-tb_width/2)
		+ (tb_currPos[1]-tb_height/2)*(tb_currPos[1]-tb_height/2));
	tb_angle = acosf(tb_angle);
	tb_angle = tb_angle * 57.295779513082323;
	tb_angle = sign * tb_angle;

	_getWorldXYZ(axis);
	_atTwist();
}

void MyCamera::_atTwist(void)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(tb_angle, tb_axis[0], tb_axis[1], tb_axis[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
	glPopMatrix();
}
void MyCamera::_pan(float dx, float dy)
{
	float axis[3];

	axis[0] = dx;
	axis[1] = -dy;
	axis[2] = 0;

	_getWorldXYZ(axis);

	float mag = sqrtf(tb_axis[0]*tb_axis[0] + tb_axis[1]*tb_axis[1] + tb_axis[2]*tb_axis[2]);
	for(int i=0; i<3; i++) tb_axis[i] = tb_axis[i] / mag; // normalize the tb_axis to be unit vector

	_atPan();
}

void MyCamera::_atPan(void)
{
	float H = -tb_accuTransform[14] * tanf(tb_viewAngle*0.017453292519943/2) * 2;
	float mag = sqrtf(( tb_currPos[0] - tb_prevPos[0] )*( tb_currPos[0] - tb_prevPos[0] )
		+ ( tb_currPos[1] - tb_prevPos[1] )*( tb_currPos[1] - tb_prevPos[1] ));
	mag = mag * H / tb_height;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(mag*tb_axis[0], mag*tb_axis[1], mag*tb_axis[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
	glPopMatrix();
}
