#ifndef OBJECT_H
#define OBJECT_H

#include "Vec.h"

enum MaterialType {
	Diff, Refl, ReflAndRefr
};

class Object {
public:
	Vec color;
	double brightness;
	MaterialType type;
	double idxOfRefr;
	Object(const Vec &color_, double brightness_, MaterialType type_, double idxOfRefr_ = 1) 
		: color(color_), brightness(brightness_), type(type_), idxOfRefr(idxOfRefr_) {}
	virtual bool intersect(const Vec &p, const Vec &dir, double &t) const = 0;
	virtual void getDir(const Vec &p, Vec &dir) const = 0;
	virtual void getNorm(const Vec &p, const Vec &dir, Vec &norm) const = 0;
};

#endif
