#include <cmath>
#include "Vec.h"

Vec::Vec(double v) : x(v), y(v), z(v) {}

Vec::Vec(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}

Vec Vec::operator+(const Vec& rhs) const { return Vec(x + rhs.x, y + rhs.y, z + rhs.z); }
Vec Vec::operator-(const Vec& rhs) const { return Vec(x - rhs.x, y - rhs.y, z - rhs.z); }

Vec operator*(const double& scalar, const Vec& v) { 
	return Vec(v.x * scalar, v.y * scalar, v.z * scalar);
}

Vec Vec::operator*(const Vec& rhs) const {
	return Vec(x * rhs.x, y * rhs.y, z * rhs.z);
}

std::ostream& operator<<(std::ostream& os, const Vec& v) {
	os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
	return os;
}

double Vec::dot(const Vec &rhs) const {
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

double Vec::mag2() const { return this->dot(*this); }
double Vec::mag() const { return std::sqrt(mag2()); }

void Vec::normalize() {
	double m = mag();
	x = x/m;
	y = y/m;
	z = z/m;
}

Vec Vec::cross(const Vec &rhs) const {
	return Vec(y*rhs.z-z*rhs.y, z*rhs.x-x*rhs.z, x*rhs.y-y*rhs.x);
}
