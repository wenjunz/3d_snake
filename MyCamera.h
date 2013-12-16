#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <gl.h>
#include <vector>

/* 
 *  Simple trackball-like motion adapted from 
 *  "Mathematical Elements for Computer Graphics" by David F. Rogers &
 *  Alan J. Adams 1976. Section 3-8, pp. 55 ~ 59.
 *  Implemented by Tsung-Pin Yeh 10/26/2005. Modified 10/11/2006
*/

#define ROTATE 101
#define ZOOM 102
#define TWIST 103
#define PAN 104
#define VIEWALL 201
#define CAPTURE 202
#define REPLAY 203

class MyCamera
{
public:
    MyCamera(void);
    ~MyCamera(void);

/* functions */
    void SetMode(int mode);
    void Init(GLuint button, GLfloat radius, GLfloat viewAngle);
    void Matrix();
    void Reshape(int width, int height);
    void Mouse(int button, int state, int x, int y);
    void Motion(int x, int y);
	void ViewAll(float radius);
	void Capture();
	void Replay();
    
protected:
    GLint     tb_mode;  // 101: rotate
                        // 102: Zoom
                        // 103: Twist
                        // 104: Pan

    GLint     tb_button;
    GLboolean tb_moving;
    GLfloat   tb_currPos[2];
    GLfloat   tb_prevPos[2];
    GLfloat   tb_transform[16];
    GLfloat   tb_accuTransform[16];
	GLfloat   tb_currDist;

    GLuint    tb_width;
    GLuint    tb_height;
    GLfloat   tb_angle;
    GLfloat   tb_axis[3];
	GLfloat   tb_viewAngle;

	std::vector<GLfloat*> tb_record;

    void _rotate(float dx, float dy);
    void _zoom(float dx, float dy);
    void _twist(float dx, float dy);
    void _pan(float dx, float dy);
    void _getWorldXYZ(float* axis);
    void _atRotate(void);
	void _atZoom(void);
	void _atTwist(void);
	void _atPan(void);
};
