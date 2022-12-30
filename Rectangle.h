#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Triangle.h"

// actually a parallelogram 
class Rectangle : public Object {
public:
	Triangle tri1, tri2;
	Vec normal;
	Rectangle(const Vec &p0_, const Vec &p1_, const Vec &p2_, const Vec &color, double brightness, MaterialType type);
	bool intersect(const Vec &p, const Vec &dir, double &t) const;
	void getDir(const Vec &p, Vec &dir) const;
	void getNorm(const Vec &p, const Vec &dir, Vec &norm) const;
};

#endif
