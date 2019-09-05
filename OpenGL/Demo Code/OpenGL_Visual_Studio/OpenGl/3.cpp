#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};


double radius1, radius2, radius3;
double angleX1, angleX2, angleY, angleT, angleRF, angleLF;
double scale1, scale2, scale3;
struct point camPos, l, r, u;
double tempx, tempy;
double a;





void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawTriangle(double a)
{
    glColor3f(1,1,1);
	glBegin(GL_TRIANGLES);{
		glVertex3f( 0, 0, 0);
		glVertex3f( a/2, 0,-(sqrt(3)/2)*a);
		glVertex3f(-a/2, 0,-(sqrt(3)/2)*a);
	}glEnd();
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
		    if(angleX1 > -45)
            {
                angleX1 -= 2;
            }
			break;

        case '2':
            if(angleX1 < 45)
            {
                angleX1 += 2;
            }
			break;

        case '3':
		    if(angleX2 > -90)
            {
                angleX2 -= 2;
            }
			break;

        case '4':
            if(angleX2 < 0)
            {
                angleX2 += 2;
            }
			break;

        case '5':
            if(angleT < 45)
            {
                angleT += 2;
            }
            break;

        case '6':
            if(angleT > -45)
            {
                angleT -= 2;
            }
            break;

        case '7':
            if(angleRF < 90)
            {
                angleRF += 2;
            }
            break;

        case '8':
            if(angleRF > -90)
            {
                angleRF -= 2;
            }
            break;

        case '9':
            if(angleLF > -90)
            {
                angleLF -= 2;
            }
            break;

        case '0':
            if(angleLF < 0)
            {
                angleLF += 2;
            }
            break;

        case 'q':
            if(angleY < 90)
            {
                angleY += 2;
            }
            break;

        case 'w':
            if(angleY > 0)
            {
                angleY -= 2;
            }
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			camPos.z -= 4;
			break;
		case GLUT_KEY_UP:		// up arrow key
			camPos.z += 4;
			break;

		case GLUT_KEY_RIGHT:
		    tempx = camPos.x * cos(2*pi/180) - camPos.y * sin(2*pi/180);
		    tempy = camPos.x * sin(2*pi/180) + camPos.y * cos(2*pi/180);
			camPos.x = tempx;
			camPos.y = tempy;
			break;

		case GLUT_KEY_LEFT:
			tempx = camPos.x * cos(-2*pi/180) - camPos.y * sin(-2*pi/180);
		    tempy = camPos.x * sin(-2*pi/180) + camPos.y * cos(-2*pi/180);
			camPos.x = tempx;
			camPos.y = tempy;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void assignment3()
{
    //1st Ellipse
    glRotated(angleY, 0,1,0);
    glRotated(angleX1, 1,0,0);

    glTranslated(0,0,-scale1*radius1);
    glScaled(1,1,scale1);
    glutWireSphere(radius1, 15, 15);


    glScaled(1,1,1/scale1);
    glTranslated(0,0,scale1*radius1);


    //2nd Ellipse
    glTranslated(0,0,-2*scale1*radius1);
    glRotated(angleX2, 1,0,0);
    glTranslated(0,0, -scale2*radius2);
    glScaled(1,1,scale2);
    glutWireSphere(radius2, 15, 15);



    glScaled(1,1,1/scale2);
    glTranslated(0,0,scale2*radius2+2*scale1*radius1);

    //Triangle
    glTranslated(0,0,-2*scale2*radius2-2*scale1*radius1);
    glRotated(angleT, 0,0,1);
    drawTriangle(a);


    glTranslated(0,0,2*scale2*radius2+2*scale1*radius1);


    //Twisting Finger
    glPushMatrix();
    {
        glTranslated(-a/2, 0, -scale3*radius3-2*scale2*radius2-2*scale1*radius1-(sqrt(3)/2)*a);
        glRotated(angleRF, 0,0,1);
        glScaled(1,1,scale3);
        glutWireSphere(radius3, 8, 8);
    }
    glPopMatrix();


    //Indexing finger
    glTranslated(a/2, 0, -2*scale2*radius2-2*scale1*radius1-(sqrt(3)/2)*a);
    glRotated(angleLF, 1,0,0);
    glTranslated(0,0,-scale3*radius3);
    glScaled(1,1,scale3);
    glutWireSphere(radius3, 8, 8);

}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);
    gluLookAt(camPos.x,camPos.y,camPos.z,	0,0,0,	 u.x,u.y,u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

    glTranslated(0,0,50);
	drawAxes();


	assignment3();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}



void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	radius1 = 8;
	radius2 = 6;
	radius3 = 2;

	scale1 = 4;
	scale2 = 4;
	scale3 = 2;

	a = 14;

	angleX1 = 0;
	angleX2 = 0;
	angleY = 0;
	angleT = 0;
	angleRF = 0;
	angleLF = 0;


	camPos = {80, 80, -50};

	l = {-1/sqrt(2), -1/sqrt(2), 0};
	r = {-1/sqrt(2), 1/sqrt(2), 0};
    u = {0, 0, 1};




	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
