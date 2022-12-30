#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vec.h"
#include "Object.h"

class Triangle : public Object {
public:
	Vec p0, p1, p2;
	Vec normal;

	Triangle(const Vec &q0, const Vec &q1, const Vec &q2, const Vec &color, double brightness, MaterialType type);
	bool intersect(const Vec &p, const Vec &dir, double &t) const;
	void getDir(const Vec &p, Vec &dir) const;
	void getNorm(const Vec &p, const Vec &dir, Vec &norm) const;
};

#endif
