//
// Created by Subangkar on 24-Jun-19.
//

//
// Created by Subangkar on 24-Jun-19.
//

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#include <vector>

using std::vector;
using namespace std;

#define PI (2*acos(0.0))
#define RAD(deg) (deg * PI / 180)

#define AXIS_LENGTH 200
#define X_AXIS point(1,0,0)
#define Y_AXIS point(0,1,0)
#define Z_AXIS point(0,0,1)


enum color_t {
	RED, GREEN, BLUE, GREY, WHITE, BLACK, GOLDEN
};

int drawgrid;
int drawaxes;

struct point {
	double x, y, z;

	point() { x = y = z = 0; }

	point(double x, double y, double z) : x(x), y(y), z(z) {}

	point operator-() const {
		return {-x, -y, -z};
	}

	point operator+(const point &right) const {
		return {x + right.x, y + right.y, z + right.z};
	}

	point operator-(const point &right) const {
		return {x - right.x, y - right.y, z - right.z};
	}

	point operator*(const point &right) const {
		return {x * right.x, y * right.y, z * right.z};
	}

	point operator*(const double constant) const {
		return {x * constant, y * constant, z * constant};
	}

	point operator/(const double constant) const {
		return {x / constant, y / constant, z / constant};
	}

	point operator+=(const point &inc) {
		return *this = *this + inc;
	}

	point operator-=(const point &dec) {
		return *this = *this - dec;
	}

	friend point operator*(double constant, point const &rhs);
};

inline point operator*(double constant, point const &rhs) {
	return rhs * constant;
}

double cameraHeight;
double cameraAngle;

void setColor(color_t color) {
	switch (color) {
		case RED:
			glColor3f(1, 0, 0);
			break;
		case GREEN:
			glColor3f(0, 1, 0);
			break;
		case BLUE:
			glColor3f(0, 0, 1);
			break;
		case GREY:
			glColor3f(0.7, 0.7, 0.7);
			break;
		case WHITE:
			glColor3f(1, 1, 1);
			break;
		case BLACK:
			glColor3f(0, 0, 0);
			break;
		case GOLDEN:
			glColor3f(0.9, 0.6, 0.1);
			break;
		default:
			break;
	}
}

void translate(const point &vector) {
	glTranslated(vector.x, vector.y, vector.z);
}

void rotate(double angle, const point &axis) {
	glRotated(angle, axis.x, axis.y, axis.z);
}

void drawAxes() {
	if (drawaxes == 1) {
		glBegin(GL_LINES);
		{
			// x-axis
			setColor(RED);
			glVertex3f(AXIS_LENGTH, 0, 0);
			glVertex3f(-AXIS_LENGTH, 0, 0);

			// y-axis
			setColor(GREEN);
			glVertex3f(0, -AXIS_LENGTH, 0);
			glVertex3f(0, AXIS_LENGTH, 0);

			// z-axis
			setColor(BLUE);
			glVertex3f(0, 0, AXIS_LENGTH);
			glVertex3f(0, 0, -AXIS_LENGTH);
		}
		glEnd();
	}
}


void clearDisplay() {
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);    //color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


#define ARROW_STEP_DIST 0.1
#define ARROW_STEP_ANGLE 2
#define ARROW_DIR_ANGLE_INIT 0//45


#define BOUNDARY 300


// axial rotation = inner rotation
// directional rotation
class {
	double angleDirection = ARROW_DIR_ANGLE_INIT;
	point translationVector;
	point dir = point(1, 1, 0);

public:
	const double stepOfDist = ARROW_STEP_DIST, stepOfAngle = ARROW_STEP_ANGLE;


	void move() {
//		printf("%lf x:%lf y:%lf\n", angleDirection, translationVector.x, translationVector.y);
		if (translationVector.x > BOUNDARY && (270 < angleDirection || angleDirection < 90)) {
			if (angleDirection == 0) angleDirection -= 180;
			else angleDirection -= 90;
		}
		if (translationVector.y > BOUNDARY && 0 < angleDirection && angleDirection < 180) {
			if (angleDirection == 90) angleDirection -= 180;
			else angleDirection -= 90;
		}

		if (translationVector.x < 0 && 90 < angleDirection && angleDirection < 270) {
			if (angleDirection == 180) angleDirection -= 180;
			else angleDirection -= 90;
		}
		if (translationVector.y < 0 && 180 < angleDirection && angleDirection < 360) {
			if (angleDirection == 270) angleDirection -= 180;
			else angleDirection -= 90;
		}
		angleDirection = int(angleDirection + 360) % 360;
		translationVector.x += dir.x * stepOfDist * cos(RAD(angleDirection));
		translationVector.y += dir.y * stepOfDist * sin(RAD(angleDirection));
	}

	void rotateLeft() {
		angleDirection -= stepOfAngle;
	}

	void rotateRight() {
		angleDirection += stepOfAngle;
	}

	point transVector() { return translationVector; }

	GLdouble rotationAngle() { return angleDirection; }

} arrow;


void drawSq(GLdouble a) {
	glBegin(GL_LINES);
	{
		glVertex3d(0, 0, 0);
		glVertex3d(a, 0, 0);
		glVertex3d(a, 0, 0);
		glVertex3d(a, a, 0);
		glVertex3d(a, a, 0);
		glVertex3d(0, a, 0);
		glVertex3d(0, a, 0);
		glVertex3d(0, 0, 0);
	}
	glEnd();
}

void drawTrig(GLdouble l) {
	glBegin(GL_TRIANGLES);
	{
		glVertex3d(0, l / 2, 0);
		glVertex3d(0, -l / 2, 0);
		glVertex3d(l, 0, 0);
	}
	glEnd();

}

void drawArrow(GLdouble l) {
	setColor(GREEN);
	glBegin(GL_LINES);
	{
		glVertex3d(0, 0, 0);
		glVertex3d(3 * l, 0, 0);
	}
	glEnd();
	translate(point(2 * l, 0, 0));
	setColor(RED);
	drawTrig(l);
}

void drawObjects() {

//	drawAxes();
	setColor(GREY);
//	drawGrid();

	glScaled(0.5, 0.5, 1);
	setColor(GREY);
	drawSq(BOUNDARY);


	translate(arrow.transVector());
	rotate(arrow.rotationAngle(), Z_AXIS); // directional rotation
	drawArrow(10);

}


void setupCamera();

void display() {

	clearDisplay();

	setupCamera();

	drawObjects();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate() {
	//codes for any changes in Models, Camera
	arrow.move();
	glutPostRedisplay();
}

void keyboardListener(unsigned char key, int x, int y);

void specialKeyListener(int key, int x, int y);

void mouseListener(int button, int state, int x, int y);//x, y is the x-y of the screen (2D)

void init() {

	//codes for initialization
	drawgrid = 0;
	drawaxes = 1;
	cameraHeight = 150.0;
	cameraAngle = 1.0;

	//clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80, 1, 1, 1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color

	glutCreateWindow("Sphere to/from Cylinder Simulation");

	init();

	glEnable(GL_DEPTH_TEST);    //enable Depth Testing

	glutDisplayFunc(display);    //display callback function
	glutIdleFunc(animate);        //what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();        //The main loop of OpenGL

	return 0;
}


#pragma region IOHandler

void mouseListener(int button, int state, int x, int y) {    //x, y is the x-y of the screen (2D)
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {        // 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes = 1 - drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {        // 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawgrid = !drawgrid;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void specialKeyListener(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_DOWN:        //down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:        // up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
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

void keyboardListener(unsigned char key, int x, int y) {
	switch (key) {

		case 'w':
			arrow.move();
			break;
		case 's':
			arrow.move();
			break;
		case 'a':
			arrow.rotateRight();
			break;
		case 'd':
			arrow.rotateLeft();
			break;
		default:
			break;
	}
}

#pragma endregion


void setupCamera() {
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
//	gluLookAt(200 * cos(cameraAngle), 200 * sin(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);
	gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
//	gluLookAt(camera.position().x, camera.position().y, camera.position().z, camera.position().x + camera.left().x,
//	          camera.position().y + camera.left().y, camera.position().z + camera.left().z, camera.up().x,
//	          camera.up().y, camera.up().z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);
}
