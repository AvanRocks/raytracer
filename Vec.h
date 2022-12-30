#ifndef VEC_H
#define VEC_H

#include <iostream>

class Vec {
public:
	double x, y, z;
	Vec(double v);
	Vec(double xx, double yy, double zz);
	Vec operator+(const Vec &rhs) const;
	Vec operator-(const Vec &rhs) const;
  friend Vec operator*(const double &scalar, const Vec& v);
  Vec operator*(const Vec& rhs) const;
  friend std::ostream& operator<<(std::ostream &os, const Vec &v);
	double dot(const Vec &rhs) const;
	double mag() const; // magnitude 
	double mag2() const; // magnitude squared
	void normalize();
	Vec cross(const Vec &rhs) const;
};

#endif
