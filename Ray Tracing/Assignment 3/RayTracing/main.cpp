//
// Created by Subangkar on 24-Jul-19.
//

#include <cmath>
#include <vector>
#include <iostream>
#include "bitmap_image.hpp"

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))
#define RAD(t) (t * pi / 180.0)
#define INF 999999999999.0
#define PRECISION 0.10e-12
#define fovY 90//45
#define tangent (tan(RAD(fovY/2.0)))

using namespace std;

#include "Utils.hpp"

void drawGrid() {
	glBegin(GL_QUADS);
	{
		for (int i = -50; i < 50; i++) {
			for (int j = -50; j < 50; j++) {
				if ((i + j) % 2 == 0) glColor3f(1, 1, 1);
				else glColor3f(0, 0, 0);
				glVertex3f(i * 30, j * 30, 0);
				glVertex3f(i * 30 + 30, j * 30, 0);
				glVertex3f(i * 30 + 30, j * 30 + 30, 0);
				glVertex3f(i * 30, j * 30 + 30, 0);
			}
		}
	}
	glEnd();
}

void drawSphere(double radius, int slices, int stacks, double R, double G, double B) {
	glColor3f(R, G, B);
	Point points[100][100];
	int i, j;
	double h, r;
	for (i = 0; i <= stacks; i++) {
		h = radius * sin(((double) i / (double) stacks) * (pi / 2));
		r = radius * cos(((double) i / (double) stacks) * (pi / 2));
		for (j = 0; j <= slices; j++) {
			points[i][j] = Point(r * cos(((double) j / (double) slices) * 2 * pi),
			                     r * sin(((double) j / (double) slices) * 2 * pi), h);
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

void drawPyramid(double x0, double y0, double z0, double len, double height, double r, double g, double b) {
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f(x0, y0, z0);
		glVertex3f(x0 + len, y0, z0);
		glVertex3f(x0 + len / 2.0, y0 + len / 2.0, z0 + height);
		glVertex3f(x0 + len, y0, z0);
		glVertex3f(x0 + len, y0 + len, z0);
		glVertex3f(x0 + len / 2.0, y0 + len / 2.0, z0 + height);
		glVertex3f(x0 + len, y0 + len, z0);
		glVertex3f(x0, y0 + len, z0);
		glVertex3f(x0 + len / 2.0, y0 + len / 2.0, z0 + height);
		glVertex3f(x0, y0 + len, z0);
		glVertex3f(x0, y0, z0);
		glVertex3f(x0 + len / 2.0, y0 + len / 2.0, z0 + height);
	}
	glEnd();
	// base square
	glBegin(GL_QUADS);
	{
		glVertex3f(x0, y0, z0);
		glVertex3f(x0 + len, y0, z0);
		glVertex3f(x0 + len, y0 + len, z0);
		glVertex3f(x0, y0 + len, z0);
	}
	glEnd();
}


class Shape;

class Color {
public:
	double r{}, g{}, b{};

	Color();

	Color(double r, double g, double b);

	Color(const Shape &s);

	Color operator+(const Color &right) const;

	Color operator-(const Color &right) const;

	Color operator+(double k) const;

	Color operator-(double k) const;

	Color operator*(double k) const;

	Color operator+=(const Color &right);
};

Color min(const Color &l, const Color &r) {
	return {min(l.r, r.r), min(l.g, r.g), min(l.b, r.b)};
}

class Shape : public Point {
public:
	double r{}, g{}, b{};
	double ambient{}, diffuse{}, specular{}, reflection{}, specular_exp{};

	Shape() = default;

	Shape(double x, double y, double z, double r, double g, double b) : Point(x, y, z), r(r), g(g), b(b) {}

	virtual void draw() {}

	virtual double intersect(const Point &src, const Point &d, Color &c, int level = 0) { return -1; }

	// in normalized form
	virtual Point getNormal(const Point &intersectionPoint) { return Point(0, 0, 0); }

	void setProperties(double ambient, double diffuse, double specular, double reflection, double shininess) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->specular_exp = shininess;
		this->reflection = reflection;
	}

	virtual Point getReflected(Point dir, const Point &normal) {
		dir = dir.normalize();
		return dir - normal * (2.0 * Point::dot(normal, dir));
	}

	virtual Color
	getColor(const Point &intersectionPoint, const Color &reflectedOn, double lambart, double phong) {
		return min(
				Color(*this) * ambient + Color(*this) * diffuse * lambart + specular * phong + reflectedOn * reflection,
				Color(1.0, 1.0, 1.0));
	}
};

class Sphere : public Shape {
public:
	double radius;

	Sphere() : Shape() {}

	Sphere(double radius, double x, double y, double z, double r, double g, double b)
			: Shape(x, y, z, r, g, b), radius(radius) {}

	void draw() override {
		glPushMatrix();
		glTranslated(x, y, z);
		drawSphere(radius, 20, 20, r, g, b);
		glPopMatrix();
	}

	double intersect(const Point &src, const Point &d, Color &color, int level = 0) override {
		Sphere &s = *this;
		auto a = Point::dot(d, d);// a>0 for sure here
		auto b = 2.0 * Point::dot(d, (src - s));// translate eye to Ro
		auto c = Point::dot(src - s, src - s) - s.radius * s.radius;

		auto D = b * b - 4.0 * a * c;
		if (D < 0) return -1;
		auto det = sqrt(D);

		auto t = (-b - det) / (2.0 * a);
		return t > 0.0 ? t : (-b + det) / (2.0 * a);
	}

	Point getNormal(const Point &intersectionPoint) override {
		return (intersectionPoint - *this).normalize();
	}
};

class Pyramid : public Shape {
public:
	double len{};
	double height{};

	Pyramid() = default;

	Pyramid(double x, double y, double z, double len, double height, double r, double g, double b)
			: Shape(x, y, z, r, g, b), len(len), height(height) {}

	void draw() override {
		drawPyramid(x, y, z, len, height, r, g, b);
	}

	double intersect(const Point &src, const Point &d, Color &c, int level = 0) override {
		Pyramid &p = *this;
		vector<double> tS(5, -1);

		// determining the Point of intersection at the bottom
		if (d.z != 0) {
			double tt = (p.z - src.z) / d.z;

			double X = src.x + tt * d.x;
			double Y = src.y + tt * d.y;

			if (p.x < X && X < (p.x + p.len) && p.y < Y && Y < (p.y + p.len)) tS[0] = tt;
		}

		// five corner points of pyramid
		auto V0 = Point(p.x, p.y, p.z);
		auto V1 = Point(p.x + p.len, p.y, p.z);
		auto V2 = Point(p.x + p.len, p.y + p.len, p.z);
		auto V3 = Point(p.x, p.y + p.len, p.z);
		auto V4 = V0;
		vector<Point> arr = {V0, V1, V2, V3, V4};
		auto C = Point(p.x + p.len / 2.0, p.y + p.len / 2.0, p.z + p.height);//top point

		// for each triangles
		for (int a = 1; a < 5; a++) {
			const auto &A = arr[a - 1], B = arr[a];
			auto v1 = B - A;
			auto v2 = C - A;

			auto normal = Point::cross(v2, v1).normalize();

			auto dir = d;
			auto v = (C - src);
			if (double_equals(Point::dot(dir, normal), 0.0)) continue;
			double t = Point::dot(v, normal) / Point::dot(dir, normal);

			auto intersectionPoint = src + dir * t;

			auto v0 = C - A;
			v1 = B - A;
			v2 = intersectionPoint - A;

			auto dot00 = Point::dot(v0, v0);
			auto dot01 = Point::dot(v0, v1);
			auto dot02 = Point::dot(v0, v2);
			auto dot11 = Point::dot(v1, v1);
			auto dot12 = Point::dot(v1, v2);

			// Compute barycentric coordinates
			auto invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
			auto U = (dot11 * dot02 - dot01 * dot12) * invDenom;
			auto V = (dot00 * dot12 - dot01 * dot02) * invDenom;

			if ((U >= 0) && (V >= 0) && (U + V < 1)) tS[a] = t;
		}

		auto index = minIndex(tS, 0.0, INF);
		return index == -1 ? -1 : tS[index];
	}

	Point getNormal(const Point &intersectionPoint) override {
		Pyramid &p = *this;

		// five corner points of pyramid
		auto V0 = Point(p.x, p.y, p.z);
		auto V1 = Point(p.x + p.len, p.y, p.z);
		auto V2 = Point(p.x + p.len, p.y + p.len, p.z);
		auto V3 = Point(p.x, p.y + p.len, p.z);
		auto V4 = V0;
		vector<Point> arr = {V0, V1, V2, V3, V4};
		auto C = Point(p.x + p.len / 2.0, p.y + p.len / 2.0, p.z + p.height);//top point

		auto base_normal = Point::cross(V3 - V0, V1 - V0).normalize();
		if (double_equals(Point::dot(V0 - intersectionPoint, base_normal), 0.0)) return base_normal;

		// upper four triangles
		for (int a = 1; a < 5; a++) {
			const auto &A = arr[a - 1], B = arr[a];
			auto normal = Point::cross(C - A, B - A).normalize();

			Point v = (C - intersectionPoint).normalize();
			if (double_equals(Point::dot(v, normal), 0)) return normal.normalize();
		}
		cout << intersectionPoint << " Point is not intersecting any face of pyramid ";
		// upper four triangles
		for (int a = 1; a < 5; a++) {
			const auto &A = arr[a - 1], B = arr[a];
			auto normal = Point::cross(C - A, B - A).normalize();

			Point v = (C - intersectionPoint).normalize();
			cout << (Point::dot(v, normal)) << " ";
		}
		cout << endl;
		return Shape::getNormal(intersectionPoint);
	}
};

class CheckerBoard : public Shape {
public:
	CheckerBoard() : Shape(0, 0, 0, 1, 1, 1) {
		setProperties(0.3, 0.4, 0.0, 0.3, 0.0);
	}

	void draw() override {
		drawGrid();
	}

	double intersect(const Point &src, const Point &d, Color &color, int level = 0) override {
		double t = -1;
		if (!double_equals(d.z, 0.0)) t = src.z / (-d.z);
//		if (d.z != 0) t = src.z / (-d.z);
		if (level == 0) return t < 0 ? -1 : t;
		return t;
	}

	Point getNormal(const Point &intersectionPoint) override {
		return Point(0, 0, 1);
	}

	Color getColor(const Point &intersectionPoint, const Color &c, double lambart, double phong) override {
		int xindex = floor(abs(intersectionPoint.x) / 30.0) + (intersectionPoint.x < 0);
		int yindex = floor(abs(intersectionPoint.y) / 30.0) + (intersectionPoint.y < 0);
		r = g = b = !((xindex + yindex) % 2);// color=black for odd summed tiles
		return Shape::getColor(intersectionPoint, c, lambart, phong);
	}
};

class LightSource : public Point {
public:
	LightSource() = default;

	LightSource(double x, double y, double z) : Point(x, y, z) {}

	explicit LightSource(Point p) : Point(p) {}

	void draw() {
		glPushMatrix();
		glTranslated(x, y, z);
		drawSphere(5, 10, 10, 1, 1, 1);
		glPopMatrix();
	}
};

Color::Color() = default;

Color::Color(double r, double g, double b) : r(r), g(g), b(b) {}

Color::Color(const Shape &s) : r(s.r), g(s.g), b(s.b) {}

Color Color::operator+(const Color &right) const {
	return {r + right.r, g + right.g, b + right.b};
}

Color Color::operator-(const Color &right) const {
	return {r - right.r, g - right.g, b - right.b};
}

Color Color::operator+(const double k) const {
	return {r + k, g + k, b + k};
}

Color Color::operator-(const double k) const {
	return {r - k, g - k, b - k};
}

Color Color::operator*(const double k) const {
	return {r * k, g * k, b * k};
}

Color Color::operator+=(const Color &right) {
	return *this = *this + right;
}

Point l(-1 / sqrt(2.0), -1 / sqrt(2.0), 0);
Point r(-1 / sqrt(2.0), 1 / sqrt(2.0), 0);
Point u(0, 0, 1);

Point camera_position = {};

vector<Shape *> objects;
vector<LightSource> lightSourceVector;

int recursionDepth;
int screenSize;
int nearDistance = 1;


bool reachesLightSource(const Point &start, const Point &nextToStart, const LightSource &source) {
	double t_src = (source - start).magnitude() / (nextToStart - start).magnitude();

	auto d = (nextToStart - start).normalize();

	Color color;
	for (const auto &object : objects) {
		auto t = object->intersect(start, d, color);
		if (t > 0 && t <= t_src) return false;// object exists between light and start 
	}
	return true;
}

Color nextIntersectionPoint(const Point &start, const Point &nextToStart, int depth) {
	if (depth < 1) return {0, 0, 0};
	auto tDepths = vector<double>(objects.size(), -1);

	auto d = (nextToStart - start).normalize();

	Color color;
	for (auto i = 0; i < objects.size(); ++i) {
		auto t = objects[i]->intersect(start, d, color);
		if (t < 0) continue;
		tDepths[i] = t;
	}
	auto index = minIndex(tDepths, 0.0, INF);

	if (index == -1) return {0, 0, 0}; //ok

	double t = tDepths[index];
	auto intersectionPoint = start + t * d;

	Shape *object = objects[index];
	auto normal = object->getNormal(intersectionPoint);

	double lambart = 0.0;
	double phong = 0.0;

	auto R = object->getReflected(d, normal);
	for (auto lightSource : lightSourceVector) {
		auto light_source_direction = (lightSource - intersectionPoint).normalize();

		if (!reachesLightSource(intersectionPoint + light_source_direction * 0.05,// for checkerboard
		                        intersectionPoint + light_source_direction, lightSource))
			continue;

		lambart += max(Point::dot(normal, light_source_direction), 0.0);
		phong += pow(max(Point::dot(light_source_direction, R), 0.0), object->specular_exp);
	}

	Point start_ = intersectionPoint + R * 0.05;
	Point nextToStart_ = intersectionPoint + R * 1;
	Color c2 = nextIntersectionPoint(start_, nextToStart_, depth - 1);

	return object->getColor(intersectionPoint, c2, lambart, phong);
}

void renderImage() {

	cout << "Rendering started" << endl;

	auto pixelBuffer = matrix<Color>(screenSize, screenSize, Color(0, 0, 0));
	auto pointBuffer = matrix<Point>(screenSize, screenSize);
	Point p = camera_position + (l * nearDistance);// center of near plane

	double multiplier = nearDistance * tangent / (double) screenSize;
	cout << multiplier << " " << nearDistance << " " << tangent << " " << screenSize << endl;
	for (int i = screenSize / 2; i < screenSize; i++) {
		for (int j = screenSize / 2; j < screenSize; j++) {
			int incI = i - screenSize / 2;
			int incJ = j - screenSize / 2;
			Point vi = r * (multiplier * (2.0 * incI + 1));
			Point vj = u * (-1.0 * multiplier * (2.0 * incJ + 1));
			pointBuffer[i][j] = p + vi + vj;
			pointBuffer[i][screenSize - j] = p + vi - vj;
			pointBuffer[screenSize - i][j] = p - vi + vj;
			pointBuffer[screenSize - i][screenSize - j] = p - vi - vj;
		}
	}

	int percent = 0;
	int targetValue = 0;
	for (int i = 0; i < screenSize; i++) {
		for (int j = 0; j < screenSize; j++) {

			if (i == targetValue) {
				cout << "\r" << "Rendering " << percent << "%";
				percent += 10;
				targetValue += (screenSize / 10);
			}
			pixelBuffer[i][j] = nextIntersectionPoint(camera_position, pointBuffer[i][j], recursionDepth);
		}
	}
	cout << endl;

	bitmap_image image(screenSize, screenSize);
//	freopen("out.txt", "w", stdout);
	for (int x = 0; x < screenSize; x++) {
		for (int y = 0; y < screenSize; y++) {
			image.set_pixel(x, y, pixelBuffer[x][y].r * 255, pixelBuffer[x][y].g * 255, pixelBuffer[x][y].b * 255);
//			cout << pixelBuffer[x][y].r << "," << pixelBuffer[x][y].g << "," << pixelBuffer[x][y].b << " ";
		}
//		cout << endl;
	}
	image.save_image("output.bmp");

	cout << "Rendering complete" << endl;

}

void keyboardListener(unsigned char key, int x, int y) {

	double step = 0.8;

	switch (key) {

		case '1':
			r = r * cos(-RAD(step)) + l * sin(-RAD(step));
			l = r * (-sin(-RAD(step))) + l * cos(-RAD(step));
			break;

		case '2':
			//r := rcos + lsin
			r = r * cos(RAD(step)) + l * sin(RAD(step));
			l = r * (-sin(RAD(step))) + l * cos(RAD(step));
			break;

		case '3':
			l = l * cos(RAD(step)) + u * sin(RAD(step));
			u = l * (-sin(RAD(step))) + u * cos(RAD(step));
			break;

		case '4':
			l = l * cos(-RAD(step)) + u * sin(-RAD(step));
			u = l * (-sin(-RAD(step))) + u * cos(-RAD(step));
			break;

		case '5':
			r = r * cos(RAD(step)) + u * sin(RAD(step));
			u = r * (-sin(RAD(step))) + u * cos(RAD(step));
			break;

		case '6':
			r = r * cos(RAD(step)) + u * sin(-RAD(step));
			u = r * (sin(RAD(step))) + u * cos(RAD(step));
			break;
		case '0':
			renderImage();
			break;
	}
}

void specialKeyListener(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:        //down arrow key
			camera_position += 3 * l;
			break;

		case GLUT_KEY_DOWN:        // up arrow key
			camera_position -= 3 * l;
			break;

		case GLUT_KEY_RIGHT:
			camera_position += 3 * r;
			break;

		case GLUT_KEY_LEFT:
			camera_position -= 3 * r;
			break;

		case GLUT_KEY_PAGE_UP:
			camera_position += 3 * u;
			break;

		case GLUT_KEY_PAGE_DOWN:
			camera_position -= 3 * u;
			break;

		case GLUT_KEY_HOME:
			break;

		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y) {    //x, y is the x-y of the screen (2D)
}

void display() {

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);    //color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	gluLookAt(camera_position.x, camera_position.y, camera_position.z,
	          camera_position.x + l.x, camera_position.y + l.y, camera_position.z + l.z,
	          u.x, u.y, u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	for (const auto &object : objects) object->draw();
	for (auto &light : lightSourceVector) light.draw();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate() {
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init() {
	//codes for initialization
	camera_position = Point(50, 50, 50);

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
	gluPerspective(fovY, 1, 1, 1000.0);
}

void parseFile() {
	ifstream file;
	file.open("description.txt");

	file >> recursionDepth >> screenSize;

	int nObjects;
	file >> nObjects;
	objects.push_back(new CheckerBoard());
	while (nObjects--) {
		string str;
		file >> str;
		if (str == "sphere") {
			double x, y, z, r, R, G, B, ambient, diffuse, specular, reflection;
			int shininess;
			file >> x >> y >> z >> r >> R >> G >> B >> ambient >> diffuse >> specular >> reflection >> shininess;
			Sphere &s = *new Sphere(r, x, y, z, R, G, B);
			s.setProperties(ambient, diffuse, specular, reflection, shininess);
			objects.push_back(&s);
		} else if (str == "pyramid") {
			double x0, y0, z0, len, height, R, G, B, ambient, diffuse, specular, reflection;
			int shininess;
			file >> x0 >> y0 >> z0 >> len >> height >> R >> G >> B >> ambient >> diffuse >> specular >> reflection
			     >> shininess;
			Pyramid &p = *new Pyramid(x0, y0, z0, len, height, R, G, B);
			p.setProperties(ambient, diffuse, specular, reflection, shininess);
			objects.push_back(&p);
		}
	}

	int numLightSources;
	file >> numLightSources;
	while (numLightSources--) {
		double x, y, z;
		file >> x >> y >> z;
		lightSourceVector.emplace_back(x, y, z);
	}

}

int main(int argc, char **argv) {

	if (8.07731e-017 != 0.00) cout << "precision problem" << endl;
	if (double_equals(PRECISION * 0.1, 0.00)) cout << "precision set to 12 digits after decimal point" << endl;

	parseFile();

	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color

	glutCreateWindow("Ray Tracing");

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
