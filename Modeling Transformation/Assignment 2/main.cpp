#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <stack>
#include <vector>
#include <cmath>

#define COLUMN 'C'

using namespace std;

#define PI (2 * acos(0.0))
#define WIDTH 7

#define RAD(t) (t * PI / 180.0)

// Point is kept non-homogeneous because every operations done on matrices which are homogeneous (4*4) or (4*2)
class Point {
public:
	double x, y, z;//w=0

	Point() { x = y = z = 0; }

	Point(double x, double y, double z) : x(x), y(y), z(z) {}

	Point normalize() {
		double r = sqrt(x * x + y * y + z * z);
		return *this = (*this / r);
	}

	friend istream &operator>>(istream &is, Point &point) {
		is >> point.x >> point.y >> point.z;
		return is;
	}


	friend ostream &operator<<(ostream &os, const Point &point) {
//		os << "x: " << point.x << " y: " << point.y << " z: " << point.z;
		os << point.x << " " << point.y << " " << point.z << endl;
		return os;
	}

	Point operator-() const {
		return {-x, -y, -z};
	}

	Point operator+(const Point &right) const {
		return {x + right.x, y + right.y, z + right.z};
	}

	Point operator-(const Point &right) const {
		return {x - right.x, y - right.y, z - right.z};
	}

	Point operator*(const Point &right) const {
		return {x * right.x, y * right.y, z * right.z};
	}

	Point operator*(const double constant) const {
		return {x * constant, y * constant, z * constant};
	}

	Point operator/(const double constant) const {
		return {x / constant, y / constant, z / constant};
	}

	Point operator+=(const Point &inc) {
		return *this = *this + inc;
	}

	Point operator-=(const Point &dec) {
		return *this = *this - dec;
	}

	friend Point operator*(double constant, Point const &rhs);

	static Point cross(const Point &a, const Point &b) {
		return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
	}

	static double dot(const Point &a, const Point &b) {
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}
};

inline Point operator*(double constant, Point const &rhs) {
	return rhs * constant;
}

class Matrix {

private:

	void init(int r, int c) {
		vector<double> row(c, 0);
		for (int i = 0; i < r; i++) {
			matrix.push_back(row);
		}
	}

public:
	vector<vector<double>> matrix;

	Matrix(int r, int c) {
		init(r, c);
	}

	Matrix(const Matrix &m) : matrix(m.matrix) {}

	double &W() {
		if (matrix.empty()) cout << "Matrix Not Initialized" << endl;
		return matrix[matrix.size() - 1][matrix[0].size() - 1];
	}

	friend ostream &operator<<(ostream &os, const Matrix &m) {
		for (const auto &row : m.matrix) {
			for (auto cell : row) {
				os << cell << ' ';
			}
			os << endl;
		}
		return os;
	}

	Matrix operator*(const Matrix &B) const {
		const auto &A = *this;
		auto rA = A.matrix.size(), cA = A.matrix[0].size();
		auto rB = B.matrix.size(), cB = B.matrix[0].size();

		if (cA != rB) {
			cout << "Multiplication Dimension error!\n";
			return ZeroMatrix(rA, cB);
		}

		auto C = ZeroMatrix(rA, cB);

		for (auto i = 0; i < rA; i++) {
			for (auto j = 0; j < cB; j++) {
				for (auto k = 0; k < cA; k++) {
					C.matrix[i][j] += A.matrix[i][k] * B.matrix[k][j];
				}
			}
		}

		return C;
	}

	static Matrix ZeroMatrix(int r, int c) {
		return Matrix(r, c);
	}

	static Matrix ColumnMatrix(const Point &point) {
		auto a = ZeroMatrix(4, 1);

		a.matrix[0][0] = point.x;
		a.matrix[1][0] = point.y;
		a.matrix[2][0] = point.z;
		a.matrix[3][0] = 1;
		return a;
	}

	static Matrix IdentityMatrix(int r = 4, int c = 4) {
		auto a = ZeroMatrix(r, c);
		for (int i = 0; i < a.matrix.size(); i++) {
			a.matrix[i][i] = 1;
		}
		return a;
	}

	static Matrix TranslationMatrix(double dx, double dy, double dz) {
		auto a = IdentityMatrix(4, 4);

		a.matrix[0][3] = dx;
		a.matrix[1][3] = dy;
		a.matrix[2][3] = dz;
		return a;
	}

	static Matrix TranslationMatrix(Point d) {
		auto a = IdentityMatrix(4, 4);

		a.matrix[0][3] = d.x;
		a.matrix[1][3] = d.y;
		a.matrix[2][3] = d.z;
		return a;
	}

	static Matrix ScalingMatrix(double sx, double sy, double sz) {
		auto a = ZeroMatrix(4, 4);

		a.matrix[0][0] = sx;
		a.matrix[1][1] = sy;
		a.matrix[2][2] = sz;
		a.W() = 1;

		return a;
	}

	static Matrix ScalingMatrix(Point s) {
		auto a = ZeroMatrix(4, 4);

		a.matrix[0][0] = s.x;
		a.matrix[1][1] = s.y;
		a.matrix[2][2] = s.z;
		a.W() = 1;

		return a;
	}

	static Matrix RotationMatrix(Point col0, Point col1, Point col2) {

		auto a = ZeroMatrix(4, 4);

		vector<Point> col = {col0, col1, col2};

		for (int c = 0; c < 3; ++c) {
			a.matrix[0][c] = col[c].x;
			a.matrix[1][c] = col[c].y;
			a.matrix[2][c] = col[c].z;
		}

		a.W() = 1;
		return a;
	}

	static Matrix ProjectionMatrix(double near, double far, double r, double t) {
		auto a = ZeroMatrix(4, 4);

		a.matrix[0][0] = near / r;
		a.matrix[1][1] = near / t;

		a.matrix[2][2] = -(far + near) / (far - near);
		a.matrix[2][3] = -(2 * far * near) / (far - near);

		a.matrix[3][2] = -1;

		return a;
	}
};

Point transformPoint(const Matrix &M, const Point point) {

	auto result = (M * Matrix::ColumnMatrix(point));
	return Point(result.matrix[0][0], result.matrix[1][0], result.matrix[2][0]) / result.W();
}

Point formulaOfRodrigues(Point x, Point a, double angle) {

	auto cost = cos(RAD(angle));
	auto sint = sin(RAD(angle));

	return cost * x + (1 - cost) * Point::dot(a, x) * a + sint * Point::cross(a, x);
}

int main(int argc, char *argv[]) {

	FILE *fp = nullptr;
	if (argc > 1)
		fp = freopen(argv[1], "r", stdin);
	else
		fp = freopen("scene.txt", "r", stdin);

	if (fp == nullptr) {
		cout << "No Input File !!!" << endl;
		cout << "executable.exe <scene-file>" << endl;
		exit(0);
	}

	ofstream stage1, stage2, stage3;
	stage1.open("stage1.txt");
	stage2.open("stage2.txt");
	stage3.open("stage3.txt");

	stage1 << setprecision(WIDTH) << fixed;
	stage2 << setprecision(WIDTH) << fixed;
	stage3 << setprecision(WIDTH) << fixed;

	Point eye, lookat, up;

	cin >> eye >> lookat >> up;

	//view transformation
	//l = lookat - eye
	auto l = (lookat - eye).normalize();

	//r = l X up
	auto right = Point::cross(l, up).normalize();

	//u = r X l
	auto u = Point::cross(right, l);

	auto translateEye = Matrix::TranslationMatrix(-eye);

	auto col1 = Point(right.x, u.x, -l.x);
	auto col2 = Point(right.y, u.y, -l.y);
	auto col3 = Point(right.z, u.z, -l.z);

	auto viewTranformation = (Matrix::RotationMatrix(col1, col2, col3) * Matrix::TranslationMatrix(-eye));

	double fovY, aspectRatio, near, far;
	cin >> fovY >> aspectRatio >> near >> far;

	//projection transformation
	//fovX = fovY * aspectRatio
	auto fovX = fovY * aspectRatio;

	//t = near * tan(fovY/2)
	auto t = near * tan(RAD(fovY / 2));

	//r = near * tan(fovX/2)
	auto r = near * tan(RAD(fovX / 2));

	auto projectionMatrix = Matrix::ProjectionMatrix(near, far, r, t);

	//modeling transformation
	stack<Matrix> S;

	stack<stack<Matrix>> lastState;

	//S.push(identity matrix)

	S.push(Matrix::IdentityMatrix(4, 4));

	string command;
	while (true) {
		cin >> command;

		if (command == "triangle") {

			Point points[3];
			for (auto &point : points) {
				cin >> point;
			}

			for (const auto &point : points) {

				//P' <- transformPoint(S.top,P)
				auto model = transformPoint(S.top(), point);
				auto view = transformPoint(viewTranformation, model);
				auto projection = transformPoint(projectionMatrix, view);

				stage1 << model;

				stage2 << view;

				stage3 << projection;
			}

			stage1 << endl;
			stage2 << endl;
			stage3 << endl;
		} else if (command == "translate") {

			double tx, ty, tz;
			cin >> tx >> ty >> tz;

			//S.push(product(S.top,T))

			S.push((S.top() * Matrix::TranslationMatrix(tx, ty, tz)));
		} else if (command == "scale") {

			double sx, sy, sz;
			cin >> sx >> sy >> sz;

			//generate the corresponding scaling matrix T
			//S.push(product(S.top,T))
			S.push((S.top() * Matrix::ScalingMatrix(sx, sy, sz)));
		} else if (command == "rotate") {

			double angle;
			Point axis;
			cin >> angle >> axis;

			axis = axis.normalize();

			auto i = Point(1, 0, 0);
			auto j = Point(0, 1, 0);
			auto k = Point(0, 0, 1);

			auto col1 = formulaOfRodrigues(i, axis, angle);
			auto col2 = formulaOfRodrigues(j, axis, angle);
			auto col3 = formulaOfRodrigues(k, axis, angle);

			//S.push(product(S.top,T))
			S.push((S.top() * Matrix::RotationMatrix(col1, col2, col3)));
		} else if (command == "push") {
			lastState.push(S);
		} else if (command == "pop") {
			S = lastState.top();
			lastState.pop();
		} else if (command == "end") {
			break;
		}
	}

	stage1.close();
	stage2.close();
	stage3.close();

	return 0;
}
