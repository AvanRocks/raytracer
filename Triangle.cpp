#include "Triangle.h"
#include "Vec.h"

Triangle::Triangle(const Vec &p0_, const Vec &p1_, const Vec &p2_, const Vec &color, double brightness, MaterialType type)
	: Object(color, brightness, type), p0(p0_), p1(p1_), p2(p2_), normal((p1_-p0_).cross(p2_-p1_)) {
  if (type == ReflAndRefr) this->type = Refl;	
	normal.normalize();
}

bool Triangle::intersect(const Vec &p, const Vec &dir, double &t) const {
	Vec perp = (p0.dot(normal) - p.dot(normal)) * normal;
	Vec perpNorm = perp;
	perpNorm.normalize();
	double d = dir.dot(perpNorm);
	if (d <= 0) return false;
	t = perp.mag() / d;
	Vec q = p + t * dir;
	double c1 = (p1-p0).cross(q-p0).dot(normal);
	double c2 = (p2-p1).cross(q-p1).dot(normal);
	double c3 = (p0-p2).cross(q-p2).dot(normal);
	if ((c1 >= 0 && c2 >= 0 && c3 >= 0) 
		||(c1 <= 0 && c2 <= 0 && c3 <= 0))
		return true;
	return false;
}
	
void Triangle::getDir(const Vec &p, Vec &dir) const {
	dir = (p.dot(normal) - p0.dot(normal)) * normal;
	dir.normalize();
}

void Triangle::getNorm(const Vec &p, const Vec &dir, Vec &norm) const {
	if (normal.dot(dir) > 0)
		norm = -1 * normal;
	else
		norm = normal;
}
