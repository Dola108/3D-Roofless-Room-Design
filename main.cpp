#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "RGBpixmap.cpp"
#define PI 3.14159
/* GLUT callback Handlers */

static int slices = 16;
static int stacks = 16;
double k = 0, b = 0, c = 0;

int en = 1, sp = 0, sp2 = 0, start = 1;

int depth = 1;

double zoom = 0.7;
double xx = 0, yy = 0, zz = 0;

RGBpixmap pix[9];

//camera vars
static GLdouble camRadius = 15;
static GLdouble camTheta = 0;

static GLdouble camHeight = 0;

static GLdouble camRoll = 90;

static GLdouble camCenterX = 0;
static GLdouble camCenterY = 0;
static GLdouble camCenterZ = 0;

const GLfloat no_material_light[] = { 0.0f, 0.0f, 0.0f, 0.1f };
const GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
const GLfloat spot2_direction[] = { -0.4, 0.5, -0.6 };

const GLfloat spot_ambient[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat spot_diffuse[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat spot_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat spot_position[] = { 1.0f, 0.3f, 5.0f, 1.0f };

const GLfloat spot2_ambient[]  = { 0.9f, 0.7f, 0.3f, 1.0f };
const GLfloat spot2_diffuse[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat spot2_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat spot2_position[] = { 5.0f, -11.0f, 16.0f, 1.0f };

const GLfloat light_ambient[]  = { 0.9f, 0.9f, 0.9f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 1.0f, 4.0f, 2.0f, 0.0f };

const GLfloat t_mat_ambient[]    = { 0.5f, 0.4f, 0.3f, 1.0f };
const GLfloat t_mat_diffuse[]    = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat t_mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat t_high_shininess[] = { 100.0f };

const GLfloat tl_mat_ambient[]    = { 0.3f, 0.3f, 0.3f, 1.0f };
const GLfloat tl_mat_diffuse[]    = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat tl_mat_specular[]   = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat tl_high_shininess[] = { 100.0f };

const GLfloat w_mat_ambient[]    = { 0.8f, 0.7f, 0.6f, 1.0f };
const GLfloat w_mat_diffuse[]    = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat w_mat_specular[]   = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat w_high_shininess[] = { 100.0f };

const GLfloat w2_mat_ambient[]    = { 0.4f, 0.5f, 0.3f, 1.0f };
const GLfloat w2_mat_diffuse[]    = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat w2_mat_specular[]   = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat w2_high_shininess[] = { 100.0f };

const GLfloat q1_mat_ambient[]    = { 0.7f, 0.54f, 0.4f, 1.0f };
const GLfloat q2_mat_ambient[]    = { 0.3f, 0.54f, 0.4f, 1.0f };
const GLfloat q3_mat_ambient[]    = { 0.3f, 0.54f, 0.6f, 1.0f };
const GLfloat q4_mat_ambient[]    = { 0.1f, 0.5f, 0.3f, 1.0f };
const GLfloat q5_mat_ambient[]    = { 0.1f, 0.5f, 0.1f, 1.0f };
const GLfloat q6_mat_ambient[]    = { 0.2f, 0.1f, 0.1f, 1.0f };
const GLfloat q0_mat_ambient[]    = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat j_mat_ambient[]    = { 0.1f, 0.5f, 0.3f, 1.0f };

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void camera()
{
    GLdouble angleInRadians = camTheta * PI / 180.0;

    GLdouble cosAngle = cos(angleInRadians);
    GLdouble sinAngle = sin(angleInRadians);
    GLdouble camZ = camRadius * cosAngle;
    GLdouble camX = camRadius * sinAngle;
    GLdouble camY = camHeight;

    GLdouble upX = 1, upY = 0, upZ = 0;
    GLdouble tX, tY, tZ;

    GLdouble rollInRadians = camRoll * PI / 180.0;
    tX = cos(rollInRadians) * upX - sin(rollInRadians) * upY;
    tY = sin(rollInRadians) * upX + cos(rollInRadians) * upY;
    upX = tX, upY = tY;

    tX = cosAngle * upX + sinAngle * upZ;
    tZ = -sinAngle * upX + cosAngle * upZ;
    upX = tX, upZ = tZ;

    gluLookAt(camX, camY, camZ, camCenterX, camCenterY, camCenterZ, upX, upY, upZ);
}

void polygon(int x) {
    if (x == 1) {
        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q1_mat_ambient);
            glVertex3f(4,0,-0.15);
            glVertex3f(4,.2,-0.15);
            glVertex3f(1,.2,-0.15);
            glVertex3f(1,0,-0.15);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q1_mat_ambient);
            glVertex3f(4,0,-1);
            glVertex3f(1,0,-1);
            glVertex3f(1,.2,-1);
            glVertex3f(4,.2,-1);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(4,.2,-0.15);
            glVertex3f(4,.2,-1);
            glVertex3f(1,.2,-1);
            glVertex3f(1,.2,-0.15);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(1,0,-0.15);
            glVertex3f(1,0,-1);
            glVertex3f(4,0,-1);
            glVertex3f(4,0,-0.15);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(1,0,-0.15);
            glVertex3f(1,.2,-0.15);
            glVertex3f(1,.2,-1);
            glVertex3f(1,0,-1);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(4,0,-0.15);
            glVertex3f(4,0,-1);
            glVertex3f(4,.2,-1);
            glVertex3f(4,.2,-0.15);
        glEnd();
    }

    if(x == 2) {
        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q1_mat_ambient);
            glVertex3f(3.5,.2,-.5);
            glVertex3f(3.5,1.2,-.5);
            glVertex3f(1.5,1.2,-.5);
            glVertex3f(1.5,.2,-.5);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q2_mat_ambient);
            glVertex3f(3.5,.2,-.7);
            glVertex3f(1.5,.2,-.7);
            glVertex3f(1.5,1.2,-.7);
            glVertex3f(3.5,1.2,-.7);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q3_mat_ambient);
            glVertex3f(3.5,.2,-.7);
            glVertex3f(3.5,1.2,-.7);
            glVertex3f(3.5,1.2,-.5);
            glVertex3f(3.5,.2,-.5);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q3_mat_ambient);
            glVertex3f(1.5,.2,-.5);
            glVertex3f(1.5,1.2,-.5);
            glVertex3f(1.5,1.2,-.7);
            glVertex3f(1.5,.2,-.7);
        glEnd();
    }

    if(x == 3) {
        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q3_mat_ambient);
            glVertex3f(4,1.2,-0.2);
            glVertex3f(4,1.2,-3);
            glVertex3f(0.5,1.2,-3);
            glVertex3f(0.5,1.2,-0.2);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(0.5,1.2,-0.2);
            glVertex3f(0.5,1.2,-3);
            glVertex3f(4,1.2,-3);
            glVertex3f(4,1.2,-0.2);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q2_mat_ambient);
            glVertex3f(4, 1.2, -0.2);
            glVertex3f(4, 1.2, -3);
            glVertex3f(4, 3.2, -3);
            glVertex3f(4, 3.2, -0.2);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q4_mat_ambient);
            glVertex3f(4, 3.2, -0.2);
            glVertex3f(4, 3.2, -3);
            glVertex3f(4, 1.2, -3);
            glVertex3f(4, 1.2, -0.2);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q4_mat_ambient);
            glVertex3f(0.5, 2, -0.2);
            glVertex3f(0.5, 2, -3);
            glVertex3f(0.5, 1.2, -3);
            glVertex3f(0.5, 1.2, -0.2);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q5_mat_ambient);
            glVertex3f(0.5, 1.2, -0.2);
            glVertex3f(0.5, 1.2, -3);
            glVertex3f(0.5, 2, -3);
            glVertex3f(0.5, 2, -0.2);
        glEnd();
    }

    if(x ==4 ) {
        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(4,1.2,-0.2);
            glVertex3f(4, 2.3, -0.2);
            glVertex3f(1.3, 2.3, -0.2);
            glVertex3f(0.5, 2, -0.2);
            glVertex3f(0.5,1.2,-0.2);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(0.5,1.2,-3);
            glVertex3f(0.5, 2, -3);
            glVertex3f(1.3, 2.3, -3);
            glVertex3f(4, 2.3, -3);
            glVertex3f(4,1.2,-3);
        glEnd();

        glBegin(GL_POLYGON);
            glColor3f(0.5,0.7,0.5);
            glVertex3f(0.5, 2,-0.2);
            glVertex3f(1.3, 2.3, -0.2);
            glVertex3f(1.3, 2.3, -3);
            glVertex3f(0.5, 2, -3);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q1_mat_ambient);
            glVertex3f(1.3, 2.3, -3);
            glVertex3f(1.3, 2.3, -0.2);
            glVertex3f(2.1, 2.7, -0.2);
            glVertex3f(2.1, 2.7, -3);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(2.1, 2.7, -0.2);
            glVertex3f(1.3, 2.3, -0.2);
            glVertex3f(4, 2.3, -0.2);
            glVertex3f(4, 2.7, -0.2);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(4, 2.7, -3);
            glVertex3f(4, 2.3, -3);
            glVertex3f(1.3, 2.3, -3);
            glVertex3f(2.1, 2.7, -3);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(4, 3.2, -0.2);
            glVertex3f(3.2, 3.1, -0.2);
            glVertex3f(4, 2.7, -0.2);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(3.2, 3.1, -0.2);
            glVertex3f(2.1, 2.7, -0.2);
            glVertex3f(4, 2.7, -0.2);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(4, 2.7, -3);
            glVertex3f(2.1, 2.7, -3);
            glVertex3f(3.2, 3.1, -3);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(4, 2.7, -3);
            glVertex3f(3.2, 3.1, -3);
            glVertex3f(4, 3.2, -3);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q1_mat_ambient);
            glVertex3f(4, 3.2, -0.2);
            glVertex3f(4, 3.2, -3);
            glVertex3f(3.2, 3.1, -3);
            glVertex3f(3.2, 3.1, -0.2);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(0.5,1.2,-0.2);
            glVertex3f(0.5, 2, -0.2);
            glVertex3f(1.3, 2.3, -0.2);
            glVertex3f(2.1, 2.7, -0.2);
            glVertex3f(3.2, 3.1, -0.2);
            glVertex3f(4, 3.2, -0.2);
            glVertex3f(4,1.2,-0.2);
        glEnd();

        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(4,1.2,-3);
            glVertex3f(4, 3.2, -3);
            glVertex3f(3.2, 3.1, -3);
            glVertex3f(2.1, 2.7, -3);
            glVertex3f(1.3, 2.3, -3);
            glVertex3f(0.5, 2, -3);
            glVertex3f(0.5,1.2,-3);
        glEnd();
    }

    if (x == 5) {
        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q1_mat_ambient);
            glVertex3f(4,0,-0.15);
            glVertex3f(4,.2,-0.15);
            glVertex3f(1,.2,-0.15);
            glVertex3f(1,0,-0.15);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q1_mat_ambient);
            glVertex3f(4,0,-1);
            glVertex3f(1,0,-1);
            glVertex3f(1,.2,-1);
            glVertex3f(4,.2,-1);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(4,.2,-0.15);
            glVertex3f(4,.2,-1);
            glVertex3f(1,.2,-1);
            glVertex3f(1,.2,-0.15);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(1,0,-0.15);
            glVertex3f(1,0,-1);
            glVertex3f(4,0,-1);
            glVertex3f(4,0,-0.15);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(1,0,-0.15);
            glVertex3f(1,.2,-0.15);
            glVertex3f(1,.2,-1);
            glVertex3f(1,0,-1);
        glEnd();

        glBegin(GL_QUADS);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q0_mat_ambient);
            glVertex3f(4,0,-0.15);
            glVertex3f(4,0,-1);
            glVertex3f(4,.2,-1);
            glVertex3f(4,.2,-0.15);
        glEnd();
    }

    if(x == 6) {
        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(1, 0, 0);
            glVertex3f(2, 0, 0);
            glVertex3f(1, 1, 0);
        glEnd();
        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(1, 1, -.2);
            glVertex3f(2, 0, -.2);
            glVertex3f(1, 0, -.2);
        glEnd();
        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(1, 0, 0);
            glVertex3f(1, 1, 0);
            glVertex3f(1, 1, -.2);
            glVertex3f(1, 0, -.2);
        glEnd();
        glBegin(GL_POLYGON);
            glMaterialfv(GL_FRONT, GL_AMBIENT, q6_mat_ambient);
            glVertex3f(1, 1, -.2);
            glVertex3f(1, 1, 0);
            glVertex3f(2, 0, 0);
            glVertex3f(2, 0, -.2);
        glEnd();
    }
}

void halfcircle()
{
    double radius=2.9;
    glBegin(GL_POLYGON);
    for(double i=0; i<=PI; i+=PI/360)
    {
        glVertex3d(cos(i)*radius,sin(i)*radius,-1);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(double i=PI; i>=0; i-=PI/360)
    {
        glVertex3d(cos(i)*radius,sin(i)*radius,1);
    }
    glEnd();

    glBegin(GL_QUADS);
    for(double i=0; i<=PI; i+=PI/360)
    {
//        glColor3d(1,0,0);
            double x=cos(i)*radius,y=sin(i)*radius;
            double x1=cos(i==PI?0:i+PI/360)*radius,y1=sin(i==PI?0:i+PI/360)*radius;
            glVertex3d(x,y,1);
            glVertex3d(x1,y1,1);
            glVertex3d(x1,y1,-1);
            glVertex3d(x,y,-1);
    }
            double x=cos(0)*radius,y=sin(0)*radius;
            double x1=cos(PI)*radius,y1=sin(PI)*radius;
            glVertex3d(x,y,1);
            glVertex3d(x,y,-1);
            glVertex3d(x1,y1,-1);
            glVertex3d(x1,y1,1);
    glEnd();
}

void propeller(){
        glRotatef(90,1,0,0);
        glScalef(0.03, 0.03, 0.6);
        halfcircle();
        glRotatef(180,0,1,0);
        glRotatef(-180,1,0,0);
        glTranslated(0,0.2,0);
        halfcircle();

        glPushMatrix();
            glTranslated(-60,-22,-0.6);
            glScalef(25,40,1);
            glRotated(90,1,0,0);
            polygon(1);
            glTranslated(1.04,0.1,-0.55);
            glRotatef(-90,0,1,0);
            glRotatef(90,1,0,0);
            glScalef(0.15, 0.15, 0.15);
            halfcircle();
            glRotatef(180,0,1,0);
            glRotatef(-180,1,0,0);
            glTranslated(0,19.2,0);
            halfcircle();
            glTranslated(-12,-15.6,0.5);
            glRotatef(90,1,0,0);
            glScalef(5,5,10);
            polygon(1);
            glRotatef(-90,1,0,0);
            glRotatef(90,0,0,1);
            glScalef(0.145, 0.15, 0.145);
            glTranslated(3.9,-7,0);
            halfcircle();
            glRotatef(180,0,1,0);
            glRotatef(-180,1,0,0);
            glTranslated(0,19.2,0);
            halfcircle();
        glPopMatrix();
}

void sphere(){
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glPushMatrix();
        glTranslated(-2.4,1.2,-6);
        glRotated(45,0,0,1);
        glRotated(-20,1,0,0);
        glScaled(0.5, 2, 0.5);
        glutSolidSphere(1,slices,stacks);
        glTranslated(0,1,0);
        glScaled(1, 0.25, 1);
        glutSolidSphere(1,slices,stacks);
        glTranslated(0,-8,0);
        glutSolidSphere(1,slices,stacks);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-2.4,1.2,-6);
        glRotated(-45,0,0,1);
        glRotated(-20,1,0,0);
        glScaled(0.5, 2, 0.5);
        glutSolidSphere(1,slices,stacks);
        glTranslated(0,1,0);
        glScaled(1, 0.25, 1);
        glutSolidSphere(1,slices,stacks);
        glTranslated(0,-8,0);
        glutSolidSphere(1,slices,stacks);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-2.4,1.2,-6);
        glRotated(50,1,0,0);
        glRotated(-45,0,1,0);
        glScaled(0.5, 2, 0.5);
        glutSolidSphere(1,slices,stacks);
        glTranslated(0,1,0);
        glScaled(1, 0.25, 1);
        glutSolidSphere(1,slices,stacks);
        glTranslated(0,-8,0);
        glutSolidSphere(1,slices,stacks);
    glPopMatrix();
}

void cubeside(int x) {
    glPushMatrix();
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0*x, -1.0*x, 1.0*x);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0*x, 1.0*x, 1.0*x);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1.0*x, 1.0*x, 1.0*x);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(-1.0*x, -1.0*x, 1.0*x);
            glEnd();
    glPopMatrix();
}

void texcube(int tex, int x){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   w_mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   w_mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  w_mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, w_high_shininess);

    glBindTexture(GL_TEXTURE_2D, tex);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
            cubeside(x);
            glRotated(-180,0,1,0);
            cubeside(x);
            glRotated(-90,0,1,0);
            cubeside(x);
            glRotated(-180,0,1,0);
            cubeside(x);
            glRotated(-90,1,0,0);
            cubeside(x);
            glRotated(-180,1,0,0);
            cubeside(x);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void table() {
    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   t_mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   t_mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  t_mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, t_high_shininess);
        glTranslated(0,-1,0.5);
        glScaled(3,0.2,2);
        texcube(3, 1);
    glPopMatrix();

    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT,   tl_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   tl_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  tl_mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, tl_high_shininess);
        glTranslated(-2,-2.7,1.6);
        glScaled(0.2,1.5,0.2);
        texcube(3, 1);
        glTranslated(21,0,0);
        texcube(3, 1);
    glPopMatrix();

    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   tl_mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   tl_mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  tl_mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, tl_high_shininess);
        glTranslated(-2,-2.7,-0.7);
        glScaled(0.2,1.5,0.2);
        texcube(3, 1);
        glTranslated(21,0,0);
        texcube(3, 1);
    glPopMatrix();
}

void picture() {
    glBindTexture(GL_TEXTURE_2D, 6);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   w_mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   w_mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  w_mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, w_high_shininess);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0, 0.0, 0.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0, 1.0, 0.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(0.0, 1.0, 0.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(0.0, 0.0, 0.0);
            glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void sky(int t) {
    glBindTexture(GL_TEXTURE_2D, t);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   w_mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   w_mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  w_mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, w_high_shininess);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0, 0.0, 0.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0, 1.0, 0.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(0.0, 1.0, 0.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(0.0, 0.0, 0.0);
            glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void room() {
    glPushMatrix();
        glTranslated(2.9,-5.94,0);
        glRotated(90,1,0,0);
        glScaled(3.95,3.25,0.12);
        texcube(4, 2);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-2.2,-3.5,-6.68);
        glScaled(3.25,2.75,0.25);
        texcube(5, 1);
        glPushMatrix();
            glTranslated(3,0,0);
            texcube(5, 1);
//            glTranslated(2,0,0);
//            texcube(5, 1);
            glTranslated(-1.5,0,0);
            glScaled(0.5,1,1);
            texcube(5, 1);
        glPopMatrix();
        glPushMatrix();
//            glTranslated(3,2,0);
//            texcube(5, 1);
//            glTranslated(6,0,0);
//            texcube(5, 1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(0,2,0);
            texcube(5, 1);
            glTranslated(3,0,0);
            texcube(5, 1);
            glTranslated(-1.5,0.5,0);
            glScaled(0.5,0.5,1);
            texcube(5, 1);
        glPopMatrix();
    glPopMatrix();

//    glBindTexture(GL_TEXTURE_2D,5);
//    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslated(-5.2,-0.75,-0.05);
        glRotated(90,0,1,0);
        glScaled(3.25,2.75,0.125);
        texcube(5, 2);
    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);

//    glBindTexture(GL_TEXTURE_2D,5);
//    glEnable(GL_TEXTURE_2D);
//    glPushMatrix();
//        glTranslated(10.3,-0.25,-0.3);
//        glRotated(90,0,1,0);
//        glScaled(4.5,4,0.2);
//        glutSolidCube(3);
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
}

void objects() {
//    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double t = 0;
    (start == 1)? t = glutGet(GLUT_ELAPSED_TIME) / 1000.0 : t = glutGet(GLUT_ELAPSED_TIME) / 100000.0;
    const double d = t*90.0;

    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, j_mat_ambient);
        glTranslated(-3,-2.4,3);
        glScaled(0.5,0.5,0.5);
        sphere();
    glPopMatrix();

    glPushMatrix();
        glRotated(-60, 0,1, 0);
        glTranslated(xx,yy,zz);
        glScaled(zoom, zoom, zoom);
        glRotated(k, 1, 0, 0);
        glRotated(b, 0, 1, 0);
        glRotated(c, 0, 0, 1);

        glPushMatrix();
            glRotated(d,0,1,0);
            propeller();
        glPopMatrix();

        glTranslated(-3.5,-3.7,1.4);
        polygon(1);
        polygon(2);
        glTranslated(0,0,-2);
        polygon(1);
        polygon(2);
        glTranslated(0,0,2);
        polygon(3);
        polygon(4);
        glTranslated(2.804,2,-1.1);
        glScalef(1.2,4,1);
        polygon(1);

        glPushMatrix();
            glTranslatef(5.5, 0.5,0);
            glRotated(-90,0,1,0);
            glScaled(0.25, 0.1, 0.25);
            glRotated(-90,0,0,1);
            glRotated(d,0,1,0);
            propeller();
        glPopMatrix();

        glTranslated(3.5,-0.135,0);
        glRotatef(15,0,0,1);
        glScalef(0.5,1.05,1);
        polygon(5);
        glRotatef(-4,0,0,1);
        glTranslated(-7.5,0.6,0);
        glScalef(1,.08,1);
        glScalef(0.45,0.45,0.4);
        glTranslated(25.5, -6, -1.47);
        glRotatef(-90,1,0,0);
        glRotatef(-90,0,0,1);
        glRotatef(-90,0,1,0);
        halfcircle();

    glPopMatrix();
}

void ground() {
    glRotated(90,0,0,1);
    glRotated(90,0,1,0);
    glScaled(0.25,0.75,0.5);
    glTranslated(-1,-1,0);
    sky(8);
    glPushMatrix();
        glTranslated(-1,0,0);
        sky(8);
        glTranslated(-1,0,0);
        sky(8);
        glTranslated(-1,0,0);
        sky(8);
    glPopMatrix();
    glTranslated(0,-1,0);
    sky(8);
    glPushMatrix();
        glTranslated(-1,0,0);
        sky(8);
        glTranslated(-1,0,0);
        sky(8);
        glTranslated(-1,0,0);
        sky(8);
    glPopMatrix();
    glTranslated(0,-1,0);
    sky(8);
    glPushMatrix();
        glTranslated(-1,0,0);
        sky(8);
        glTranslated(-1,0,0);
        sky(8);
        glTranslated(-1,0,0);
        sky(8);
    glPopMatrix();
glPopMatrix();
}

void bg() {
    //left
        glPushMatrix();
        glRotated(90,0,1,0);
        glTranslated(-45,-5,-25);
        glScaled(70,30.5,20);
        sky(7);
//right
        glRotated(180,0,1,0);
        glTranslated(-1,0,-3);
        glPushMatrix();
            glTranslated(0,0,-0.5);
            sky(7);
        glPopMatrix();
//back
        glRotated(90,0,1,0);
        glTranslated(-1,0,0);
        glScaled(2,1,1);
        sky(7);
        glTranslated(-1,0,0);
        sky(7);
//gnd
        ground();
}


static inline void set_material(GLfloat c[], GLfloat s[], GLfloat sh[])
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
    glMaterialfv(GL_FRONT, GL_SPECULAR, s);
    glMaterialfv(GL_FRONT, GL_SHININESS, sh);
}

static void carpet_recursive(int level, GLdouble t, GLdouble b, GLdouble l, GLdouble r)
{
    if (level <= 0)
    {
        glVertex3d(l, t, 0.01);        /// left top vertex
        glVertex3d(l, b, 0.01);        /// left bottom vertex
        glVertex3d(r, b, 0.01);        /// right bottom vertex
        glVertex3d(r, t, 0.01);        /// right top vertex
        return;
    }

    GLdouble oneThirdX = l + (r-l)*1/3.0;
    GLdouble oneThirdY = t + (b-t)*1/3.0;
    GLdouble twoThirdX = l + (r-l)*2/3.0;
    GLdouble twoThirdY = t + (b-t)*2/3.0;

    GLdouble x[4] = {l, oneThirdX, twoThirdX, r};
    GLdouble y[4] = {t, oneThirdY, twoThirdY, b};

    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            if (i==1 && j==1) continue;
            carpet_recursive(level-1, y[i], y[i+1], x[j], x[j+1]);
        }
    }
}
const GLfloat matbg_mat_ambient[]  = { 0.4f, 0.25f, 0.3f, 1.0f };
const GLfloat matbgnot_mat_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };

static void carpet(int level)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, matbg_mat_ambient);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glVertex3f(-0.5, -0.5, 0);
        glVertex3f(0.5, -0.5, 0);
        glVertex3f(0.5, 0.5, 0);
        glVertex3f(-0.5, 0.5, 0);
    glEnd();

    glMaterialfv(GL_FRONT, GL_AMBIENT, matbgnot_mat_ambient);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        carpet_recursive(level, 0.4, -0.4, -0.4, 0.4);
    glEnd();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    camera();
    room();


    glPushMatrix();
        glTranslated(2,-1.5,-5);
        glTranslated(xx,yy,zz);
        table();

        glPushMatrix();
            glRotated(-90, 1,0,0);
            glTranslated(2,-6,-4);
            glScaled(8,8,8);
            glTranslated(-0.25,0.4,-0.015);
            carpet(depth);
        glPopMatrix();

        glPushMatrix();
            glTranslated(0.5,0.52,0.2);
            glScaled(0.5,0.5,0.5);
            objects();
        glPopMatrix();

        glPushMatrix();
            glRotated(90,0,1,0);
            glScaled(2,2.5,1);
            glTranslated(-3,1,-6.65);
            picture();

            glPushMatrix();
                glTranslated(0.5,0.5,-0.15);
                glScaled(0.55,0.6,0.1);
                texcube(9, 1);
            glPopMatrix();
        glPopMatrix();

        glTranslated(-10,2.5,5);
        glScaled(2,1.5,1);
        bg();

    glPopMatrix();


    if(en == 1) {
        glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    else if(en == 0){
        glLightfv(GL_LIGHT0, GL_AMBIENT,  no_material_light );
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  no_material_light );
        glLightfv(GL_LIGHT0, GL_SPECULAR,  no_material_light );
    }
    if(sp == 1) {
        glLightfv(GL_LIGHT1, GL_AMBIENT,  spot_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE,  spot_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, spot_specular);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
        glLightfv(GL_LIGHT1, GL_POSITION, spot_position);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0f);
//        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
//        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
//        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
    }
    else if(sp == 0) {
        glLightfv(GL_LIGHT1, GL_AMBIENT,  no_material_light );
        glLightfv(GL_LIGHT1, GL_DIFFUSE,  no_material_light );
        glLightfv(GL_LIGHT1, GL_SPECULAR,  no_material_light );
    }
    if(sp2 == 1) {
        glLightfv(GL_LIGHT2, GL_AMBIENT,  spot2_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE,  spot2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, spot2_specular);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot2_direction);
        glLightfv(GL_LIGHT2, GL_POSITION, spot2_position);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0f);
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 5.0f);
//        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
//        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
//        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
    }
    else if(sp2 == 0) {
        glLightfv(GL_LIGHT2, GL_AMBIENT,  no_material_light );
        glLightfv(GL_LIGHT2, GL_DIFFUSE,  no_material_light );
        glLightfv(GL_LIGHT2, GL_SPECULAR,  no_material_light );
    }

    glFlush();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
       case 'w':
            camRadius -= 1;
            break;
        case 's':
            camRadius += 1;
            break;
        case 'a':
            camTheta -= 5;
            break;
        case 'd':
            camTheta += 5;
            break;
        case 'h':
            camHeight += 1;
            break;
        case 'H':
            camHeight -= 1;
            break;

//       case 'W':
//            if(ap >=0)
//            ap -= 0.1;
//            break;
//        case 'S':
//            if(ap<=3.5)
//            ap += 0.1;
//            break;

        case 'D':
            depth+=1;
            break;
            /// center x y z control
        case 'x':
            camCenterX+=1;
            break;
        case 'y':
            camCenterY+=1;
            break;
        case 'z':
            camCenterZ+=1;
            break;
        case 'X':
            camCenterX-=1;
            break;
        case 'Y':
            camCenterY-=1;
            break;
        case 'Z':
            camCenterZ-=1;
            break;

            /// camera roll control
            /// basically up vector control
        case 'e':
            camRoll -= 5;
            break;
        case 'q':
            camRoll += 5;
            break;

        ///pitch
        //pitch with 10 unit limit
        case '1':
            if(camHeight<=10)
            camHeight += 1;
            break;
        case '2':
            if(camHeight>=-10)
            camHeight -= 1;
            break;

        //yaw with 30 degree limit
        case '5':
            if(camTheta >=-30)
            camTheta -= 5;
            break;
        case '6':
            if(camTheta<=30)
            camTheta += 5;
            break;
        case 'l':
            (en == 0)? en = 1: en = 0;
            break;
        case 'L':
            (sp == 0)? sp = 1: sp = 0;
            break;
        case 'p':
            (sp2 == 0)? sp2 = 1: sp2 = 0;
            break;
        case 'k':
            (start == 0)? start = 1: start = 0;
            break;

//        object moving
//        case 'I':
//            xx+=1;
//            break;
//        case 'O':
//            yy+=1;
//            break;
//        case 'P':
//            zz+=1;
//            break;

    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

void Init()
{
    pix[0].makeCheckImage();
	pix[0].setTexture(1);

    pix[1].readBMPFile("F:\\kuet\\4_2\\Graphics Lab\\window\\aaaa.bmp");
	pix[1].setTexture(5);
    pix[2].readBMPFile("F:\\kuet\\4_2\\Graphics Lab\\window\\floor.bmp");
	pix[2].setTexture(4);
    pix[3].readBMPFile("F:\\kuet\\4_2\\Graphics Lab\\window\\wood2.bmp");
	pix[3].setTexture(3);
    pix[4].readBMPFile("F:\\kuet\\4_2\\Graphics Lab\\window\\wood.bmp");
	pix[4].setTexture(2);
    pix[5].readBMPFile("F:\\kuet\\4_2\\Graphics Lab\\window\\pix.bmp");
	pix[5].setTexture(6);
    pix[6].readBMPFile("F:\\kuet\\4_2\\Graphics Lab\\window\\sky.bmp");
	pix[6].setTexture(7);
    pix[7].readBMPFile("F:\\kuet\\4_2\\Graphics Lab\\window\\grass.bmp");
	pix[7].setTexture(8);
    pix[8].readBMPFile("F:\\kuet\\4_2\\Graphics Lab\\window\\wood3.bmp");
	pix[8].setTexture(9);
}
/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

	Init();
    glutMainLoop();

    return EXIT_SUCCESS;
}
