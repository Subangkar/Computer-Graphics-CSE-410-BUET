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
#define CAMERA_INIT_POS point(120, 120, 40)
#define CAMERA_INIT_L point(-1 / sqrt(2), -1 / sqrt(2), 0)
#define CAMERA_INIT_R point(-1 / sqrt(2), 1 / sqrt(2), 0)
#define CAMERA_INIT_U point(0, 0, 1)
#define CAMERA_MOVE_STEP 2
#define CAMERA_ANGLE_STEP 2
#define SPHERE_SLICE_SIZE 24
#define SPHERE_STACK_SIZE 20
#define CYLINDER_SLICE_SIZE 24
#define CYLINDER_STACK_SIZE 40
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
		return { -x, -y, -z };
	}

	point operator+(const point& right) const {
		return { x + right.x, y + right.y, z + right.z };
	}

	point operator-(const point& right) const {
		return { x - right.x, y - right.y, z - right.z };
	}

	point operator*(const point& right) const {
		return { x * right.x, y * right.y, z * right.z };
	}

	point operator*(const double constant) const {
		return { x * constant, y * constant, z * constant };
	}

	point operator/(const double constant) const {
		return { x / constant, y / constant, z / constant };
	}

	point operator+=(const point& inc) {
		return *this = *this + inc;
	}

	point operator-=(const point& dec) {
		return *this = *this - dec;
	}

	friend point operator*(double constant, point const& rhs);
};

inline point operator*(double constant, point const& rhs) {
	return rhs * constant;
}

class camera_t {
public:
	void lookUp() {
		u = u * cos(RAD(CAMERA_ANGLE_STEP)) - l * sin(RAD(CAMERA_ANGLE_STEP));
		l = l * cos(RAD(CAMERA_ANGLE_STEP)) + u * sin(RAD(CAMERA_ANGLE_STEP));
	}

	void lookDown() {
		u = u * cos(-RAD(CAMERA_ANGLE_STEP)) - l * sin(-RAD(CAMERA_ANGLE_STEP));
		l = l * cos(-RAD(CAMERA_ANGLE_STEP)) + u * sin(-RAD(CAMERA_ANGLE_STEP));
	}

	void lookLeft() {
		l = l * cos(RAD(CAMERA_ANGLE_STEP)) - r * sin(RAD(CAMERA_ANGLE_STEP));
		r = r * cos(RAD(CAMERA_ANGLE_STEP)) + l * sin(RAD(CAMERA_ANGLE_STEP));
	}

	void lookRight() {
		l = l * cos(-RAD(CAMERA_ANGLE_STEP)) - r * sin(-RAD(CAMERA_ANGLE_STEP));
		r = r * cos(-RAD(CAMERA_ANGLE_STEP)) + l * sin(-RAD(CAMERA_ANGLE_STEP));
	}

	void tiltClockwise() {
		r = r * cos(RAD(CAMERA_ANGLE_STEP)) - u * sin(RAD(CAMERA_ANGLE_STEP));
		u = u * cos(RAD(CAMERA_ANGLE_STEP)) + r * sin(RAD(CAMERA_ANGLE_STEP));
	}

	void tiltCounterClockwise() {
		r = r * cos(-RAD(CAMERA_ANGLE_STEP)) - u * sin(-RAD(CAMERA_ANGLE_STEP));
		u = u * cos(-RAD(CAMERA_ANGLE_STEP)) + r * sin(-RAD(CAMERA_ANGLE_STEP));
	}

	void moveForward() {
		pos += CAMERA_MOVE_STEP * l;
	}

	void moveBackward() {
		pos -= CAMERA_MOVE_STEP * l;
	}

	void moveLeft() {
		pos -= CAMERA_MOVE_STEP * r;
	}

	void moveRight() {
		pos += CAMERA_MOVE_STEP * r;
	}

	void moveUp() {
		pos += CAMERA_MOVE_STEP * u;
	}

	void moveDown() {
		pos -= CAMERA_MOVE_STEP * u;
	}

	const point& position() const {
		return pos;
	}

	const point& left() const {
		return l;
	}

	const point& up() const {
		return u;
	}

private:
	point pos = CAMERA_INIT_POS;
	point l = CAMERA_INIT_L, r = CAMERA_INIT_R, u = CAMERA_INIT_U;

} camera;

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

void clearDisplay() {
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);    //color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void drawSquare(double a) {
	glBegin(GL_QUADS);
	{
		glVertex3f(a, a, 0);
		glVertex3f(a, -a, 0);
		glVertex3f(-a, -a, 0);
		glVertex3f(-a, a, 0);
	}
	glEnd();
}

/* centering (0,0,0) at +x,+y,+z */
void drawOneEighthSphere(double radius, int slices = SPHERE_SLICE_SIZE, int stacks = SPHERE_STACK_SIZE) {
	vector<vector<point>> points(stacks + 2, vector<point>(slices + 2));
	int i, j;
	double h, r;
	//generate points
	for (i = 0; i <= stacks; i++) {
		h = radius * sin(((double)i / (double)stacks) * (PI / 2));
		r = radius * cos(((double)i / (double)stacks) * (PI / 2));
		for (j = 0; j <= slices; j++) {
			points[i][j].x = r * cos(((double)j / (double)slices) * .5 * PI);
			points[i][j].y = r * sin(((double)j / (double)slices) * .5 * PI);
			points[i][j].z = h;
		}
	}
	//draw quads using generated points
	for (i = 0; i < stacks; i++) {
		for (j = 0; j < slices; j++) {
			glBegin(GL_QUADS);
			{
				//upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
			}
			glEnd();
		}
	}

}

/* centering (0,0,0) at +x,+y, Z : height at z-axis, curved surface at +x,+y*/
void drawOneFourthCylinder(double radius, double height, int slices = CYLINDER_SLICE_SIZE,
	int stacks = CYLINDER_STACK_SIZE) {
	vector<vector<point>> points(stacks + 2, vector<point>(slices + 2));
	int i, j;
	double h, r;
	//generate points
	for (i = 0; i <= stacks; i++) {
		h = height * sin(((double)i / (double)stacks) * (PI / 2));
		r = radius;
		for (j = 0; j <= slices; j++) {
			points[i][j].x = r * cos(((double)j / (double)slices) * .5 * PI);
			points[i][j].y = r * sin(((double)j / (double)slices) * .5 * PI);
			points[i][j].z = h;
		}
	}
	//draw quads using generated points
	for (i = 0; i < stacks; i++) {
		for (j = 0; j < slices; j++) {
			glBegin(GL_QUADS);
			{
				//upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
				//lower hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
			}
			glEnd();
		}
	}

}

/* at +z axis */
void drawTranslatedQuarterSphere(double dCenter, double radius) {
	// vertical (+x,+y) (+x,-y) (-x,+y) (-x,-y)
	point translation[] = { point(1, 1, 1), point(1, -1, 1), point(-1, 1, 1), point(-1, -1, 1) };
	GLdouble angles[] = { 0, 270, 90, 180 };
	for (auto i = 0; i < 4; i++) {
		auto transVector = point(dCenter, dCenter, dCenter) * translation[i];
		glPushMatrix();
		{
			translate(transVector);
			rotate(angles[i], Z_AXIS);
			drawOneEighthSphere(radius);  //x -ve, y +ve, z +ve
		}
		glPopMatrix();
	}
}

// 8 Spheres
void translatedSpheres(double dCenter, double radius) {
	setColor(RED);

	drawTranslatedQuarterSphere(dCenter, radius);

	glPushMatrix();
	{
		rotate(180, X_AXIS);// lower hemi at -Z
		drawTranslatedQuarterSphere(dCenter, radius);// +Z
	}
	glPopMatrix();
}

// 4 Squares
void translatedSquare(double dCenter, double width) {
	setColor(WHITE);

	// +X, -X, +Y, -Y, +Z, -Z
	point directions[] = { X_AXIS, -X_AXIS, Y_AXIS, -Y_AXIS, Z_AXIS, -Z_AXIS };
	point rotations[] = { Y_AXIS, Y_AXIS, X_AXIS, X_AXIS, point(), point() };
	double rot_angles[] = { 90, 90, 90, 90, 0, 0 };

	for (auto i = 0; i < 6; i++) {
		auto transVector =
			point(dCenter + width, dCenter + width, dCenter + width) *
			directions[i];
		glPushMatrix();
		{
			translate(transVector);
			rotate(rot_angles[i], rotations[i]);
			drawSquare(dCenter);
		}
		glPopMatrix();
	}
}

/*  around y-axis on x,z 
	@dCenter = center(dCenter, dCenter, 0)
	@radius = radius
*/
void drawOneFaceCylinders(double dCenter, double radius) {
	// vertical Z[(+x,+y) (+x,-y)] Horizontal X[(+x,+z) (+x,-z)]
	point vertical_direction[] = { point(1, 1, 0), point(1, -1, 0) };
	GLdouble vertical_angles[] = { 0, 270 };
	for (auto i = 0; i < 4; i++) {
		auto transVector = point(dCenter, dCenter, 0) * vertical_direction[i % 2];
		glPushMatrix();
		{
			if (i > 1) rotate(90, X_AXIS);// draws horizontal ones
			translate(transVector);
			rotate(vertical_angles[i % 2], Z_AXIS);
			drawOneFourthCylinder(radius, dCenter);
		}
		glPopMatrix();
	}
}

// 12 Cylinders
void translatedCylinders(double dCenter, double radius) {
	setColor(GREEN);

	vector<GLdouble> angles = { 0, 90, 180, 270 };
	for (auto& angle : angles) {
		glPushMatrix();
		{
			rotate(angle, Z_AXIS);
			drawOneFaceCylinders(dCenter, radius);
		}
		glPopMatrix();
	}
}

double dCenter = 20, rCircleSquare = 20;

void drawObjects() {

	drawAxes();
	setColor(GREY);
	drawGrid();

	translatedSpheres(dCenter, rCircleSquare);

	translatedCylinders(dCenter, rCircleSquare);

	translatedSquare(dCenter, rCircleSquare);
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
	glutPostRedisplay();
}

void keyboardListener(unsigned char key, int x, int y);

void specialKeyListener(int key, int x, int y);

void mouseListener(int button, int state, int x, int y);//x, y is the x-y of the screen (2D)

void init() {

	//codes for initialization
	drawgrid = 0;
	drawaxes = 1;

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
		camera.moveBackward();
		break;
	case GLUT_KEY_UP:        // up arrow key
		camera.moveForward();
		break;

	case GLUT_KEY_RIGHT:
		camera.moveRight();
		break;
	case GLUT_KEY_LEFT:
		camera.moveLeft();
		break;

	case GLUT_KEY_PAGE_UP:
		camera.moveUp();
		break;
	case GLUT_KEY_PAGE_DOWN:
		camera.moveDown();
		break;

	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:
		if (dCenter != 0) {
			dCenter -= 1;
			rCircleSquare += 1;
		}

		break;
	case GLUT_KEY_END:
		if (rCircleSquare != 0) {
			dCenter += 1;
			rCircleSquare -= 1;
		}
		break;

	default:
		break;
	}
}

void keyboardListener(unsigned char key, int x, int y) {
	switch (key) {

	case '1': //rotate/look left
		camera.lookLeft();
		break;
	case '2': //rotate/look right
		camera.lookRight();
		break;

	case '3': //look up
		camera.lookUp();
		break;
	case '4': //look down
		camera.lookDown();
		break;

	case '5':
		camera.tiltClockwise();
		break;
	case '6':
		camera.tiltCounterClockwise();
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
	//	gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
	gluLookAt(camera.position().x, camera.position().y, camera.position().z, camera.position().x + camera.left().x,
		camera.position().y + camera.left().y, camera.position().z + camera.left().z, camera.up().x,
		camera.up().y, camera.up().z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);
}
