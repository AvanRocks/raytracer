#include "Sphere.h"
#include <cmath>
#include <limits>

bool Sphere::intersect(const Vec &p, const Vec &dir, double &t) const {
	Vec v1 = center - p;

	double r2 = r*r;
	double c2 = v1.mag2();
	double b = v1.dot(dir);

	// p is outside the sphere and dir is pointing away
	if ((c2 > r2) && (b < 0)) return false;

	double a2 = c2 - b*b;

	if (a2 > r2) return false;

	double d = sqrt(r2 - a2);

	if (c2 > r2) {
		// p is outside the sphere
		t = b - d;
		//tfar = b + d;
	} else if (b >= 0) {
		// p is inside the sphere and dir is facing the center
		t = b + d;
		//tfar = std::numeric_limits<double>::max();
	} else {
		// p is inside the sphere and dir is facing away from the center
		t = - b + d;
		//tfar = std::numeric_limits<double>::max();
	}

	return true;
}

void Sphere::getDir(const Vec &p, Vec &dir) const {
	dir = (center - p);
	dir.normalize();
}

void Sphere::getNorm(const Vec &p, const Vec &dir, Vec &norm) const {
	norm = p - center;
	norm.normalize();
}

