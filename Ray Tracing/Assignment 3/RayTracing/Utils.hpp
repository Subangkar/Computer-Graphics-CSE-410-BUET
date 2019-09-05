//
// Created by Subangkar on 24-Jul-19.
//

#ifndef RAYTRACING_UTILS_HPP
#define RAYTRACING_UTILS_HPP

class Point {
public:
	double x, y, z;//w=0

	Point() { x = y = z = 0; }

	Point(double x, double y, double z) : x(x), y(y), z(z) {}

	Point normalize() {
		double r = sqrt(x * x + y * y + z * z);
		return *this = (*this / r);
	}

	double magnitude() {
		return sqrt(x * x + y * y + z * z);
	}

	friend istream &operator>>(istream &is, Point &point) {
		is >> point.x >> point.y >> point.z;
		return is;
	}


	friend ostream &operator<<(ostream &os, const Point &point) {
//		os << "x: " << point.x << " y: " << point.y << " z: " << point.z;
		os << point.x << " " << point.y << " " << point.z;
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

	friend bool operator==(const Point &lhs, const Point &rhs) {
		return lhs.x == rhs.x &&
		       lhs.y == rhs.y &&
		       lhs.z == rhs.z;
	}

	friend bool operator!=(const Point &lhs, const Point &rhs) {
		return !(rhs == lhs);
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

template<typename type>
vector<vector<type>> matrix(size_t r, size_t c) {
	return vector<vector<type >>(r, vector<type>(c));
}

template<typename type>
vector<vector<type>> matrix(size_t r, size_t c, type initval) {
	return vector<vector<type >>(r, vector<type>(c, initval));
}

bool double_equals(double a, double b, double epsilon = PRECISION) {
	return std::abs(a - b) < epsilon;
}

// minval & maxval exclusive
template<typename type>
int minIndex(const vector<type> &list, type minval, type maxval) {
	type cur_min = maxval;
	int minIdx = -1;
	for (int i = 0; i < list.size(); ++i) {
		if (list[i] > minval && list[i] < cur_min) {
			cur_min = list[i];
			minIdx = i;
		}
	}
	return minIdx;
}

#endif //RAYTRACING_UTILS_HPP
