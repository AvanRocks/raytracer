#include <vector>
#include <fstream>
#include <limits>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Sphere.h"
#include "Vec.h"
#include "Triangle.h"
#include "Rectangle.h"
using namespace std;

int width = 3840, height = 2160; // 4K UHD, 16:9
//int width = 1920, height = 1080; // FHD, 16:9
//int width = 1366, height = 768; // HD, ~16:9 (my laptop screen)
//int width = 640, height = 480; // nHD, 16:9
//int width = 100, height = 100; // bruh
//Vec backgroundCol(0.235294, 0.67451, 0.843137); 
//Vec backgroundCol(0.7);
//Vec backgroundCol(0.53, 0.81, 0.92);
Vec backgroundCol = Vec(3) * Vec(0.53, 0.98, 1);
Vec eye(0, 0, -2*width); // projection point
int maxDepth = 10;

int numSpheres = 40;

// find closest point of intersection
bool findClosestInt(const Vec &p, const Vec&dir, const vector<Object*> &objects, const Object **object, double *distance) {

	*object = nullptr;
	double minT = numeric_limits<double>::max();
	for (const Object *curr : objects) {
		double t;
		if (curr->intersect(p, dir, t)) {
			if (t < minT) {
				minT = t;
				*object = curr;
			}
		}
	}

	*distance = minT;
	if (*object) return true;
	else return false;
}

double mix(double p, double q, double val ) {
	return (1-val) * p + val * q;
}

Vec getColor(const Vec &p, const Vec &dir, const vector<Object*> &objects, int depth) {
	if (depth == maxDepth) return Vec(0);

	// find closest point of intersection
  const Object * object = nullptr;
	double distance;
	findClosestInt(p, dir, objects, &object, &distance);

	// no point of intersection
	if (!object) return backgroundCol;

	// point of intersection
	Vec poi = p + distance * dir;

	// add an offset to prevent 'shading acne'
	Vec norm(0);
	object->getNorm(poi, dir, norm);
	double bias = 1e-8;

	if (object->type == Refl) {
		Vec reflDir = dir - 2 * dir.dot(norm) * norm;
		return object->color + getColor(poi + bias*norm, reflDir, objects, depth + 1);
	} else if (object->type == ReflAndRefr) {
		// reflection
		double cosi = dir.dot(norm);
		bool inside = false;
		if (cosi > 0) inside = true;
		
		if (inside) norm = -1 * norm;
		else cosi *= -1;

		Vec reflDir = dir + 2 * cosi * norm;
		Vec reflCol = getColor(poi + bias*norm, reflDir, objects, depth + 1);

		// refraction
		double n1 = 1, n2 = object->idxOfRefr;

		if (inside) {
			double tmp = n1;
			n1 = n2;
			n2 = tmp;
		}

		double eta = n1/n2;
		double c2tmp = 1 - eta*eta * (1 - cosi*cosi);
		if (c2tmp < 0) {
			// total internal reflection
			cout << "Total Internal Reflection\n";
			return reflCol;
		}
		double c2 = sqrt(c2tmp);
		Vec refrDir = eta * dir + (eta*cosi - c2) * norm;
		refrDir.normalize();
		Vec refrCol = getColor(poi - bias*norm, refrDir, objects, depth + 1);

		// fresnel
		double cos1 = cosi;
		double cos2 = -1 * refrDir.dot(norm);

		double Fr1 = pow( (n2*cos1-n1*cos2) / (n2*cos1+n1*cos2), 2);
		double Fr2 = pow( (n1*cos2-n2*cos1) / (n1*cos2+n2*cos1), 2);
		double Fr = 0.5 * (Fr1 + Fr2);

		Vec totalColor = Fr * reflCol + (1 - Fr) * refrCol;
		return object->color + totalColor;
	} else if (object->type == Diff) {
		// object is a diffuse object
		// find the directions to lights and calculate the surface color
		Vec totalColor = mix(object->brightness, 1, 0.3) * object->color;
		for (const Object *curr : objects) {
			// if curr has no brightness ignore (optimized out)
			if (curr->brightness != 0) {
				Vec toLight(0);
				curr->getDir(poi, toLight);

				const Object * o = nullptr;
				double d;
				findClosestInt(poi + bias*norm, toLight, objects, &o, &d);

				if (o != curr) continue;

				totalColor = totalColor + 
					(object->color * (abs(toLight.dot(norm)) * (curr->brightness * curr->color)));
			}
		}
		return totalColor;
	} else {
		cout << "Error\n";
		return Vec(0);
	}

}

Vec getPixelRay(int x, int y) {
	// compute ray through pixel (x,y)
	double centerX = x + 0.5, centerY = y + 0.5;
	Vec pixel(centerX - width/2, centerY - height/2, 0);
	Vec rayDir = pixel - eye;
	rayDir.normalize();
	return rayDir;
}


void render(const vector<Object*>& objects) {
	// top-left of screen is orgin (0,0)
	// positive y is downwards
	// positive x is rightwards


	// init output file
	ofstream ofs("./image.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";

	// for each pixel
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			if (x == 0) {
				cout << (int)round(100*(double)y/height) << "%" << '\r';
				cout.flush();
				//printf("%d%%\r", (int)round(100*(double)y/height));
			}

			Vec rayDir = getPixelRay(x, y);

			Vec oldColor = getColor(eye, rayDir, objects, 0);
		  Vec color = 2.0 / M_PI * Vec(atan(oldColor.x), atan(oldColor.y), atan(oldColor.z));
			/*
		  Vec color = Vec(clamp(oldColor.x, 0.0, 1.0), 
							        clamp(oldColor.y, 0.0, 1.0),
							        clamp(oldColor.z, 0.0, 1.0));
											*/
			if (color.x < 0 || color.x > 1 
					|| color.y < 0 || color.y > 1 
					|| color.z < 0 || color.z > 1) {
				cout << "Bad color " << color << " " << x << " " << y << '\n';
				cout << "old color: " << oldColor << '\n';
			}
			ofs << (unsigned char)(color.x * 255) << (unsigned char)(color.y * 255) << (unsigned char)(color.z * 255);
		}
	}

	ofs.close();
}

void loadMesh(vector<Object*> &objects, ifstream& file) {
	vector<Vec> vertices;
	string str,a,b,c;
	while(file >> str) {
		if (str == "v") {
			double x,y,z;
			//file >> x >> y >> z;
			file >> x >> y >> z;
			//y = -y;
			vertices.push_back(100 * Vec(x,y,z));
		} else if (str == "f") {
			file >> a >> b >> c;
			int v1 = stoi(a.substr(0, a.find("/")));
			int v2 = stoi(b.substr(0, b.find("/")));
			int v3 = stoi(c.substr(0, c.find("/")));
			v1--;
			v2--;
			v3--;
			objects.push_back(new Triangle(vertices[v1], vertices[v2], vertices[v3], 
						            Vec(1,0,0), 0, Diff));
		}
	}

}

void genScene(vector<Object*> &objects) {
	objects.push_back(new Sphere(Vec(-5*width,-5*height, eye.z - width), 100, Vec(1,1,1), 2, Diff));
	objects.push_back(new Sphere(Vec(2*width,-20*height, 10*width), 100, Vec(1,1,1), 1, Diff));

	// floor
	int floorY = 2*height;
	int size = 100*width;
	Rectangle *rec = new Rectangle(Vec(-size, floorY, -size),
																 Vec(size, floorY, -size),
																 Vec(-size, floorY, size),
																 Vec(1.5) * Vec(0.33, 0.62, 0.20),
																 0, Diff);
	objects.push_back(rec);

	vector<Sphere*> spheres;
	for (int i = 1; i <= numSpheres;) {
		Vec bottomLeft = getPixelRay(0, height);
		double t;
		rec->intersect(eye, bottomLeft, t);
		Vec pBottomLeft = (eye + t * bottomLeft);
		double minZ = pBottomLeft.z;
		double rangeX = -2 * pBottomLeft.x;
		double rangeXFac = rangeX / pBottomLeft.z;

		double r;
		if (i == 1) r = (rand() % int(rangeX/4)) + int(rangeX/3);
		else r = (rand() % int(rangeX/20)) + int(rangeX/20);

		double z;
		if (i == 1) z = (rand() % 20 * width) + r + minZ;
		else z = (rand() % 40 * width) + r + minZ;
		
		double x;
		if (i == 1) x = (rand() % int(0.8 * rangeXFac * z)) - int(0.8 * rangeXFac * z / 2);
		else x = (rand() % int(rangeXFac * z)) - int(rangeXFac * z/2);

		double y = floorY  - r;

		double c1 = double(rand() % 1000)/1000;
		double c2 = double(rand() % 1000)/1000;
		double c3 = double(rand() % 1000)/1000;

		MaterialType type;
		if (i == 1) type = Refl;
		else {
			int t = rand() % 100;
			if (t < 80) type = Diff;
			else if (t < 90) type = Refl;
			else if (t < 100) type = ReflAndRefr;
		}

		double idxOfRefr = 1;
		if (type == ReflAndRefr) {
			idxOfRefr = ((rand() % 30) + 10) / 2.0;
		}

		Vec coords(x,y,z);
		Vec col(c1,c2,c3);
		Sphere *s = new Sphere(coords, r, col, 0, type, idxOfRefr);

		bool inter = false;
		for (const Sphere *s : spheres) {
			Vec v = coords - s->center;
			if (v.mag() <= s->r + r + 100) {
				inter = true;
			}
		}

		if (!inter) {
			objects.push_back(s);
			spheres.push_back(s);
			++i;
		}
	}
}

int main(int argc, char **argv) {
	vector<Object*> objects;
	
	/*
	// red sphere
	objects.push_back(new Sphere(Vec(-500,0,500), 400, Vec(0.8,0.1,0.05), 0.1, ReflAndRefr, 4.5));

	// green sphere
	objects.push_back(new Sphere(Vec(1000,0,1000), 600, Vec(0.1,0.8,0.14), 0, Diff));

	// transparent sphere
	objects.push_back(new Sphere(Vec(200,300,400), 100, Vec(0.1,0.8,0.14), 0.3, Diff));

	// white light (top-right)
	objects.push_back(new Sphere(Vec(5000,-5000,500), 500, Vec(1,1,1), 5, Diff));

	// redish light (top-left)
	objects.push_back(new Sphere(Vec(-width,-height,0), 100, Vec(1,0.8,0.8), 2, Diff));

	Triangle tri(
							 Vec(600, 500, 300),
		         	 Vec(800, 400, 300),
							 Vec(700, 500, 500),
							 Vec(1, 0, 0),
							 0, Diff);
	objects.push_back(&tri);

	// floor
	Rectangle rec(
								Vec(-10*width, 2*height, 0),
			          Vec(10*width, 2*height, 0),
								Vec(-10*width, 2*height, 8000),
								Vec(0.15, 0.1, 0.7),
								0.2, Diff);
	objects.push_back(&rec);
	*/

	/*
	objects.push_back(new Sphere(Vec(0,-500,-3000), 100, Vec(1,1,1), 2, Diff));
	objects.push_back(new Sphere(Vec(0,-3000,500), 100, Vec(1,1,1), 2, Diff));

	ifstream file("input.obj");
	if (file.is_open()) {
		loadMesh(objects, file);
		file.close();
	} else
		cout << "Error loading mesh from file\n";
	*/

	if (argc > 1) srand(atol(argv[1]));
	else {
		int seed = time(NULL);
		srand(seed);
		cout << seed << '\n';
	}

	genScene(objects);

	render(objects);

	//for (Object* o : objects) delete o;

	return 0;
}
