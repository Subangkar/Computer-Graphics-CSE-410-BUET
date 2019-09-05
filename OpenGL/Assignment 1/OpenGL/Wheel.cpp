#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#include <vector>

using std::vector;

#define PI (2*acos(0.0))
#define RAD(deg) (deg * PI / 180)

#define AXIS_LENGTH 200
#define GRID_DIFF 15
#define GRID_LENGTH (AXIS_LENGTH-GRID_DIFF)
#define WHEEL_RAD 30
#define WHEEL_HEIGHT 15
#define WHEEL_STEP_DIST 3
#define WHEEL_STEP_ANGLE 2
#define WHEEL_CYLINDER_SEGMENTS 25
#define WHEEL_INNER_RECT_FACTOR ((double)1/2)
#define WHEEL_INNER_RECT_HEIGHT_FACTOR ((double)0.99)
#define WHEEL_DIR_ANGLE_INIT 0//-45
#define X_AXIS point(1,0,0)
#define Y_AXIS point(0,1,0)
#define Z_AXIS point(0,0,1)

enum color_t {
	RED, GREEN, BLUE, GREY, DARK_GREY, WHITE, BLACK, GOLDEN
};

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point {
	double x, y, z;

	point() { x = y = z = 0; }

	point(double x, double y, double z) : x(x), y(y), z(z) {}
};

// axial rotation = inner rotation
// directional rotation
class {
	double angleDirection = WHEEL_DIR_ANGLE_INIT;
	double angleAxial = 0;
	point translationVector;

public:
	const double radius = WHEEL_RAD, height = WHEEL_HEIGHT, stepOfDist = WHEEL_STEP_DIST, stepOfAngle = WHEEL_STEP_ANGLE;

	void moveForward() {
		translationVector.x += stepOfDist * cos(RAD(angleDirection));
		translationVector.y += stepOfDist * sin(RAD(angleDirection));
		angleAxial -= axialAngularStep();
	}

	void moveBackward() {
		translationVector.x -= stepOfDist * cos(RAD(angleDirection));
		translationVector.y -= stepOfDist * sin(RAD(angleDirection));
		angleAxial += axialAngularStep();
	}

	void rotateLeft() {
		angleDirection -= stepOfAngle;
	}

	void rotateRight() {
		angleDirection += stepOfAngle;
	}

	point transVector() { return translationVector; }

	GLdouble rotationAngle() { return angleDirection; }

	GLdouble internalRotationAngle() { return angleAxial; }

	GLdouble axialAngularStep() {
		return 360 * (stepOfDist / (2 * PI * radius));
	}
} wheel;

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
	case DARK_GREY:
		glColor3f(0.5, 0.5, 0.5);
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

void translate(const point& vector) {
	glTranslated(vector.x, vector.y, vector.z);
}

void rotate(double angle, const point& axis) {
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


void drawGrid() {
	if (drawgrid == 1) {
		setColor(GREY);
		glBegin(GL_LINES);
		{
			for (auto i = -(GRID_LENGTH / GRID_DIFF); i <= (GRID_LENGTH / GRID_DIFF); i++) {
				//lines parallel to Y-axis
				glVertex3f(i * GRID_DIFF, -GRID_LENGTH, 0);
				glVertex3f(i * GRID_DIFF, GRID_LENGTH, 0);

				//lines parallel to X-axis
				glVertex3f(-GRID_LENGTH, i * GRID_DIFF, 0);
				glVertex3f(GRID_LENGTH, i * GRID_DIFF, 0);
			}
		}
		glEnd();
	}
}

// xy-plane centering (0,0,0)
void drawRectangle(double width, double height) {
	double x = width / 2, y = height / 2;
	glBegin(GL_QUADS);
	{
		glVertex3f(x, y, 0);
		glVertex3f(x, -y, 0);
		glVertex3f(-x, -y, 0);
		glVertex3f(-x, y, 0);
	}
	glEnd();
}

void clearDisplay() {
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);    //color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// base @ xy-plane, height @ z-plane, base's center @centerZ
void drawOpenCylinder(double radius, double height, double centerZ, int segments = WHEEL_CYLINDER_SEGMENTS) {
	vector<point> points(segments + 1);
	//generate points
	// xy-plane
	for (auto i = 0; i <= segments; i++) {
		points[i].x = radius * cos(((double)i / (double)segments) * 2 * PI);
		points[i].y = radius * sin(((double)i / (double)segments) * 2 * PI);
	}

	//draw segments using generated points
	for (auto i = 0; i < segments; i++) {
		glBegin(GL_QUADS);
		{
			double shade;
			if (i < segments / 2)shade = 2 * (double)i / (double)segments;
			else shade = 2 * (1.0 - (double)i / (double)segments);
			glColor3f(shade, shade, shade);
			// upper circle
			glVertex3f(points[i + 1].x, points[i + 1].y, centerZ + height);
			glVertex3f(points[i].x, points[i].y, centerZ + height);
			// lower circle
			glVertex3f(points[i].x, points[i].y, centerZ);
			glVertex3f(points[i + 1].x, points[i + 1].y, centerZ);
		}
		glEnd();
	}
}

void drawWheelObjects(double r, double h) {
	glPushMatrix();
	{
		rotate(90, Y_AXIS);//rotate to xz-plane height @ y-axis
		rotate(90, Z_AXIS);//rotate to xy-plane height @ y-axis
		// xy-plane height @ x-axis
		setColor(DARK_GREY);
		drawRectangle(2 * r * WHEEL_INNER_RECT_HEIGHT_FACTOR, h * WHEEL_INNER_RECT_FACTOR);
	}
	glPopMatrix();
	glPushMatrix();
	{
		rotate(90, X_AXIS);//rotate to xz-plane height @ x-axis
		// xy-plane height @ x-axis
		setColor(DARK_GREY);
		drawRectangle(2 * r * WHEEL_INNER_RECT_HEIGHT_FACTOR, h * WHEEL_INNER_RECT_FACTOR);
	}
	glPopMatrix();

	// centering z-axis @ xy plane
	setColor(GREY);
	drawOpenCylinder(r, h, -h / 2);
}

void drawObjects() {

	drawAxes();
	drawGrid();

	//printf("(%lf, %lf, %lf)\n", wheel.transVector().x, wheel.transVector().y, wheel.transVector().z);
	translate(wheel.transVector()); // current pos
	rotate(wheel.rotationAngle(), Z_AXIS); // directional rotation

	translate(point(0, 0, wheel.radius));// shift to upper xy-plane
	rotate(90, X_AXIS);//height along y-axis

	rotate(wheel.internalRotationAngle(), Z_AXIS); // axial rotation around height

	drawWheelObjects(wheel.radius, wheel.height);// height along z-axis
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
	angle += 0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void keyboardListener(unsigned char key, int x, int y);

void specialKeyListener(int key, int x, int y);

void mouseListener(int button, int state, int x, int y);//x, y is the x-y of the screen (2D)

void init() {
	//codes for initialization
	drawgrid = 1;
	drawaxes = 0;
	cameraHeight = 150.0;
	cameraAngle = 1.0;
	angle = 0;

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

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color

	glutCreateWindow("Wheel Simulation");

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
		//........
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

	case '1':
		drawgrid = 1 - drawgrid;
		break;

	case 'w':
		wheel.moveBackward();
		break;
	case 's':
		wheel.moveForward();
		break;
	case 'a':
		wheel.rotateRight();
		break;
	case 'd':
		wheel.rotateLeft();
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
	gluLookAt(200 * cos(cameraAngle), 200 * sin(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);
	//	gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);
}
