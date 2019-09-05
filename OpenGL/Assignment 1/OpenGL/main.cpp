#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include "glut.h"
#define pi (2*acos(0.0))

struct point
{
	double x, y, z;
};

struct point cameraPosition;
struct point upDirection;
struct point rightDirection;
struct point lookDirection;

double sideLength;
double radius;

struct point getCrossProduct(struct point a, struct point b)
{
	struct point result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;

	return result;
}

struct point getNormalizedVector(struct point a)
{

	double length = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	struct point result;
	result.x = a.x / length;
	result.y = a.y / length;
	result.z = a.z / length;

	return result;
}

struct point getSum(struct point a, struct point b)
{
	struct point result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

struct point negateVector(struct point a)
{
	struct point result;
	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;
	return result;
}

double getLength(struct point a)
{
	double length = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	return length;
}

struct point getScaledVector(struct point a, double scalar)
{
	if (scalar != 0)
	{
		a.x = a.x * scalar;
		a.y = a.y * scalar;
		a.z = a.z * scalar;
	}

	return a;
}


void printPoint(struct point a)
{
	printf("(%f, %f, %f)\n", a.x, a.y, a.z);
}

void printPLRU()
{
	printf("Camera position= ");
	printPoint(cameraPosition);
	printf("Look Direction= ");
	printPoint(lookDirection);
	printf("Right Direction= ");
	printPoint(rightDirection);
	printf("Up Direction= ");
	printPoint(upDirection);

	printf("\n");
}

struct point getRotatedVector(struct point axis, struct point input, double angleInDegrees)
{
	//axis and input are both unit vectors and they are orthogonal to each other
	struct point crossProduct = getCrossProduct(axis, input);
	return getSum(getScaledVector(input, cos(angleInDegrees * pi / 180)), getScaledVector(crossProduct, sin(angleInDegrees * pi / 180.0)));
}



void drawAxes()
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES); {
		glColor3f(1, 0, 0);
		glVertex3f(100, 0, 0);
		glVertex3f(-100, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, -100, 0);
		glVertex3f(0, 100, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 100);
		glVertex3f(0, 0, -100);
	}glEnd();
}


void drawSquare(double a)
{
	glBegin(GL_QUADS); {
		glVertex3f(a, a, 0);
		glVertex3f(a, -a, 0);
		glVertex3f(-a, -a, 0);
		glVertex3f(-a, a, 0);
	}glEnd();
}

void drawquarterCylinder(double radius, double height, int segments)
{
	int i;
	struct point pointsUpper[100];
	struct point pointsLower[100];

	for (i = 0; i <= segments; i++)
	{
		pointsUpper[i].x = radius * cos(((double)i / (double)segments) * pi / 2);
		pointsUpper[i].y = radius * sin(((double)i / (double)segments) * pi / 2);
		pointsUpper[i].z = height / 2;

		pointsLower[i].x = pointsUpper[i].x;
		pointsLower[i].y = pointsUpper[i].y;
		pointsLower[i].z = -height / 2;
	}
	for (i = 0; i < segments; i++)
	{
		glColor3f(0, 1, 0);

		glBegin(GL_QUADS);
		{
			glVertex3f(pointsUpper[i].x, pointsUpper[i].y, pointsUpper[i].z);
			glVertex3f(pointsLower[i].x, pointsLower[i].y, pointsLower[i].z);
			glVertex3f(pointsLower[i + 1].x, pointsLower[i + 1].y, pointsLower[i + 1].z);
			glVertex3f(pointsUpper[i + 1].x, pointsUpper[i + 1].y, pointsUpper[i + 1].z);
		}
		glEnd();
	}
}

void drawoneEighthSphere(double radius, int slices, int stacks)
{
	struct point points[100][100];
	int i, j;
	double h, r;

	for (i = 0; i <= stacks; i++)
	{
		h = radius * sin(((double)i / (double)stacks) * (pi / 2));
		r = radius * cos(((double)i / (double)stacks) * (pi / 2));
		for (j = 0; j <= slices; j++)
		{
			points[i][j].x = r * cos(((double)j / (double)slices) * pi / 2);
			points[i][j].y = r * sin(((double)j / (double)slices) * pi / 2);
			points[i][j].z = h;
		}
	}
	for (i = 0; i < stacks; i++)
	{
		glColor3f(1, 0, 0);
		for (j = 0; j < slices; j++)
		{
			glBegin(GL_QUADS); {
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
			}glEnd();
		}
	}
}

void drawFaces()
{

	glColor3f(1, 1, 1);

	glPushMatrix();

	glTranslatef(0, 0, -sideLength / 2);
	drawSquare(sideLength / 2 - radius);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0, 0, sideLength / 2);
	drawSquare(sideLength / 2 - radius);

	glPopMatrix();

	glPushMatrix();

	glRotatef(90, 0, -1, 0);
	glTranslatef(0, 0, sideLength / 2);
	drawSquare(sideLength / 2 - radius);

	glPopMatrix();

	glPushMatrix();

	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, sideLength / 2);
	drawSquare(sideLength / 2 - radius);

	glPopMatrix();

	glPushMatrix();

	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, sideLength / 2);
	drawSquare(sideLength / 2 - radius);

	glPopMatrix();

	glPushMatrix();

	glRotatef(90, -1, 0, 0);
	glTranslatef(0, 0, sideLength / 2);
	drawSquare(sideLength / 2 - radius);

	glPopMatrix();
}

void obj() {
	glBegin(GL_TRIANGLES); {
		glVertex3f(5, 0, 0);
		glVertex3f(0, 12, 0);
		glVertex3f(0, 0, 13);
	}glEnd();
}

void drawVertices()
{
	glPushMatrix();

	glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, sideLength / 2 - radius);
	drawoneEighthSphere(radius, 24, 20);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, -sideLength / 2 + radius);
	//glRotatef(180, 1, 1, 0);
	glRotatef(-90, 1, 0, 0);
	drawoneEighthSphere(radius, 24, 20);

	glPopMatrix();



	//glPushMatrix();

	//glRotatef(90, 0, 0, 1);
	//glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, sideLength / 2 - radius);
	//drawoneEighthSphere(radius, 24, 20);

	//glPopMatrix();

	//glPushMatrix();

	//glRotatef(180, 0, 0, 1);
	//glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, sideLength / 2 - radius);
	//drawoneEighthSphere(radius, 24, 20);

	//glPopMatrix();

	//glPushMatrix();

	//glRotatef(270, 0, 0, 1);
	//glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, sideLength / 2 - radius);
	//drawoneEighthSphere(radius, 24, 20);

	//glPopMatrix();

	//glPushMatrix();

	//glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, -sideLength / 2 + radius);
	//glRotatef(180, 1, 1, 0);
	//drawoneEighthSphere(radius, 24, 20);

	//glPopMatrix();

	//glPushMatrix();

	//glRotatef(90, 0, 0, 1);
	//glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, -sideLength / 2 + radius);
	//glRotatef(180, 1, 1, 0);
	//drawoneEighthSphere(radius, 24, 20);

	//glPopMatrix();

	//glPushMatrix();

	//glRotatef(180, 0, 0, 1);
	//glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, -sideLength / 2 + radius);
	//glRotatef(180, 1, 1, 0);
	//drawoneEighthSphere(radius, 24, 20);

	//glPopMatrix();

	//glPushMatrix();

	//glRotatef(270, 0, 0, 1);
	//glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, -sideLength / 2 + radius);
	//glRotatef(180, 1, 1, 0);
	//drawoneEighthSphere(radius, 24, 20);

	//glPopMatrix();

}


void drawEdges()
{
	glPushMatrix();

	glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	glPushMatrix();

	glRotatef(90, 0, 0, 1);
	glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	glPushMatrix();

	glRotatef(180, 0, 0, 1);
	glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	glPushMatrix();

	glRotatef(270, 0, 0, 1);
	glTranslatef(sideLength / 2 - radius, sideLength / 2 - radius, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	//--

	glPushMatrix();

	glTranslatef(0, sideLength / 2 - radius, sideLength / 2 - radius);
	glRotatef(90, 0, -1, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	glPushMatrix();

	glRotatef(90, 0, 0, 1);
	glTranslatef(0, sideLength / 2 - radius, sideLength / 2 - radius);
	glRotatef(90, 0, -1, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	glPushMatrix();

	glRotatef(180, 0, 0, 1);
	glTranslatef(0, sideLength / 2 - radius, sideLength / 2 - radius);
	glRotatef(90, 0, -1, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	glPushMatrix();

	glRotatef(270, 0, 0, 1);
	glTranslatef(0, sideLength / 2 - radius, sideLength / 2 - radius);
	glRotatef(90, 0, -1, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	//--

	glPushMatrix();

	glTranslatef(0, sideLength / 2 - radius, -sideLength / 2 + radius);
	glRotatef(90, 0, 1, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	glPushMatrix();

	glRotatef(90, 0, 0, 1);
	glTranslatef(0, sideLength / 2 - radius, -sideLength / 2 + radius);
	glRotatef(90, 0, 1, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	glPushMatrix();

	glRotatef(180, 0, 0, 1);
	glTranslatef(0, sideLength / 2 - radius, -sideLength / 2 + radius);
	glRotatef(90, 0, 1, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

	glPushMatrix();

	glRotatef(270, 0, 0, 1);
	glTranslatef(0, sideLength / 2 - radius, -sideLength / 2 + radius);
	glRotatef(90, 0, 1, 0);
	drawquarterCylinder(radius, sideLength - 2 * radius, 20);

	glPopMatrix();

}

void keyboardListener(unsigned char key, int x, int y) {
	switch (key) {

	case '1':
		lookDirection = getRotatedVector(upDirection, lookDirection, 3.0);
		rightDirection = getCrossProduct(lookDirection, upDirection);
		printPLRU();
		break;
	case '2':
		lookDirection = getRotatedVector(upDirection, lookDirection, -3.0);
		rightDirection = getCrossProduct(lookDirection, upDirection);
		printPLRU();
		break;
	case '3':
		lookDirection = getRotatedVector(rightDirection, lookDirection, 3.0);
		upDirection = getCrossProduct(rightDirection, lookDirection);
		printPLRU();
		break;
	case '4':
		lookDirection = getRotatedVector(rightDirection, lookDirection, -3.0);
		upDirection = getCrossProduct(rightDirection, lookDirection);
		printPLRU();
		break;
	case '5':
		rightDirection = getRotatedVector(lookDirection, rightDirection, -3.0);
		upDirection = getCrossProduct(rightDirection, lookDirection);
		printPLRU();
		break;
	case '6':
		rightDirection = getRotatedVector(lookDirection, rightDirection, 3.0);
		upDirection = getCrossProduct(rightDirection, lookDirection);
		printPLRU();
	default:
		break;
	}
}


void specialKeyListener(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		cameraPosition = getSum(cameraPosition, negateVector(lookDirection));
		printPLRU();
		break;
	case GLUT_KEY_UP:
		cameraPosition = getSum(cameraPosition, lookDirection);
		printPLRU();
		break;
	case GLUT_KEY_RIGHT:
		cameraPosition = getSum(cameraPosition, rightDirection);
		printPLRU();
		break;
	case GLUT_KEY_LEFT:
		cameraPosition = getSum(cameraPosition, negateVector(rightDirection));
		printPLRU();
		break;
	case GLUT_KEY_PAGE_UP:
		cameraPosition = getSum(cameraPosition, upDirection);
		printPLRU();
		break;
	case GLUT_KEY_PAGE_DOWN:
		cameraPosition = getSum(cameraPosition, negateVector(upDirection));
		printPLRU();
		break;
	case GLUT_KEY_INSERT:
		break;
	case GLUT_KEY_HOME:
		if (radius < sideLength / 2) radius += 0.5;
		break;
	case GLUT_KEY_END:
		if (radius > 0) radius -= 0.5;
		break;
	default:
		break;
	}
}

void display() {

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z, cameraPosition.x + lookDirection.x, cameraPosition.y + lookDirection.y, cameraPosition.z + lookDirection.z, upDirection.x, upDirection.y, upDirection.z);

	glMatrixMode(GL_MODELVIEW);

	drawAxes();

	//glPushMatrix();
	//drawFaces();
	//glPopMatrix();

	//glPushMatrix();
	//drawVertices();
	//glPopMatrix();

	//glPushMatrix();
	//drawEdges();
	//glPopMatrix();

	
	obj();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glColor3f(1, 0, 0);
	obj();
	glPopMatrix();

	glutSwapBuffers();
}


void animate() {
	glutPostRedisplay();
}

void init() {

	//initialize camera position
	cameraPosition.x = 100;
	cameraPosition.y = 100;
	cameraPosition.z = 0;

	//initialize upwards direction
	upDirection.x = 0;
	upDirection.y = 0;
	upDirection.z = 1;

	//initialize downwards direction
	rightDirection.x = -1 / sqrt(2.0);
	rightDirection.y = 1 / sqrt(2.0);
	rightDirection.z = 0;

	//initialize look direction
	lookDirection = getCrossProduct(upDirection, rightDirection);


	sideLength = 50;
	radius = 0;

	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, 1, 1, 1000.0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	glutCreateWindow("Cube to Sphere Transformation");
	init();
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(display);
	glutIdleFunc(animate);
	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMainLoop();
	return 0;
}

