#ifndef SPHERE_H
#define SPHERE_H

#include "Vec.h"
#include "Object.h"

class Sphere : public Object {
public:
	Vec center;
	double r;
	bool isTransparent;
	Sphere(
			const Vec &center_, 
			double radius_, 
			const Vec &color,
			double brightness,
			MaterialType type,
			double idxOfRefr = 1
			) : 
	      Object(color, brightness, type, idxOfRefr),
				center(center_),
				r(radius_)
	{}
	bool intersect(const Vec &p, const Vec &dir, double &t) const;
	void getDir(const Vec &p, Vec &dir) const;
	void getNorm(const Vec &p, const Vec &dir, Vec &norm) const;
};

#endif
