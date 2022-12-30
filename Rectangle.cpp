#include "Rectangle.h"
#include "Object.h"

Rectangle::Rectangle(const Vec &p0, const Vec &p1, const Vec &p2, const Vec &color, double brightness, MaterialType type) : Object(color, brightness, type),
	tri1(p0, p1, p2, color, brightness, type),
	tri2(p1,p0 + (p1-p0) + (p2-p0), p2, color, brightness, type),
	normal(tri1.normal) {}

bool Rectangle::intersect(const Vec &p, const Vec &dir, double &t) const {
	double t1, t2;
	bool int1 = tri1.intersect(p, dir, t1);
	bool int2 = tri2.intersect(p, dir, t2);
	if (int1) {
		t = t1;
		return true;
	} else if (int2) {
		t = t2;
		return true;
	} else
		return false;
}

void Rectangle::getDir(const Vec &p, Vec &dir) const {
	tri1.getDir(p, dir);
}

void Rectangle::getNorm(const Vec &p, const Vec &dir, Vec &norm) const {
	tri1.getNorm(p, dir, norm);
}
