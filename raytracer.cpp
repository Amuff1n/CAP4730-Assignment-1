//currently hardcoded with floats. Could use templates
#include <limits> //used for infinity

#include "CImg.h"
using namespace cimg_library;

//vector class
//will have object with 3 floats(?) to make 3 coords of vector
class Vector {
	public: 
	float x, y, z;
	//Vector() : x(0), y(0), z(0) {}
	explicit Vector (float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
	float dot_product (Vector other) { return (x * other.x) + (y * other.y) + (z * other.z); }
	Vector operator * (float f) { return Vector(x * f, y * f, z * f); }
	Vector operator + (Vector other) { return Vector(x + other.x, y + other.y, z + other.z); }
	Vector operator - (Vector other) { return Vector(x - other.x, y - other.y, z - other.z); }
	Vector operator - () { return Vector(-x, -y, -z); }
	Vector& normalize() {
		float temp = (x * x) + (y * y) + (z * z);
		float div = 1 / sqrt(temp);
		x = x * div;
		y = y * div;
		z = z * div;
		return *this;
	}
};

//ray class
//uses vector to create ray
class Ray {
	public:
	Vector p, d;
	//TODO i think d needs to be normalized 
	explicit Ray (Vector p, Vector d) : p(p), d(d) {}
	
};

//camera class
//will create camera with vector and ray
//origin of camera is corner, not center currently
class Camera {
	public:
	Vector orig, look_at;
	explicit Camera (Vector orig, Vector look_at) : orig(orig), look_at(look_at) {}
	
};

//light class
//will create light vector
class Light {
	public:
	Vector orig, light_dir;
	explicit Light (Vector orig, Vector light_dir) : orig(orig), light_dir(light_dir) {}
	
};

//sphere class
//will compute intersection of ray
class Sphere { 
	public:
	Vector origin, color; //rgb for color of sphere
	float radius, reflectivity;
	explicit Sphere (Vector origin, Vector color, float radius = 0, float reflectivity = 0) : origin(origin), color(color), radius(radius), reflectivity(reflectivity)  {}
	//computes t = -d . p +/- sqrt((d . p) ^ 2 - p . p + 1 ) , returns true if solution exists
	float intersect (Ray &ray, float t0, float t1) { 
		//the following algorithm was sourced from here https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
		ray.d.normalize();
		Vector L = origin - ray.p;
		float tca = L.dot_product(ray.d);
		if (tca < 0) return 0; //ray points away from sphere? i.e. will never intersect?
		float d2 = L.dot_product(L) - tca * tca;
		if (d2 > (radius * radius)) return 0; // inside sqrt will be greater than 0? i.e. wont be imaginary?
		float thc = sqrt((radius * radius) - d2);
		t0 = tca - thc;
		t1 = tca + thc;
		return (t0 < t1) ? t0 : t1;
	}
};

//shading method
//computes shading and outputs color vector to be rendered to img
Vector shade (Ray &ray, Sphere &sphere, Light &light, Camera &camera, float &t) {
	Vector I (50,50,50); //TODO hardcoded illumination for now, should be tied to light
	Vector Ia (5, 5, 5); //ambient lighting
	//calculate normal at intersection
	Vector point = ray.p + (ray.d * t);
	Vector normal = point - sphere.origin;
	normal.normalize();
	//compute ambient shading,
	Vector La = Ia * sphere.reflectivity;
	//compute diffuse shading,
	Vector Ld = I * sphere.reflectivity * std::max(0.f, normal.dot_product(light.light_dir));
	//compute specular shading
	Vector bisector = camera.look_at + light.light_dir;
	bisector = bisector.normalize();
	Vector Ls = I * sphere.reflectivity * std::max(0.f, normal.dot_product(bisector));
	//all together now
	Vector L = La + Ld + Ls;
	return L + sphere.color;
}

//create camera, spheres, display with Cimg
int main() {
	CImg<float> img(256,256,1,3); //define 256 by 256 color image with 8 bits per color
	img.fill(20); //set pixel values to 0 (black)

	Camera *eye = new Camera (Vector(0, 0, 255), Vector(0, 0, -1));
	Light *light = new Light (Vector(255, 255, 255), Vector(-1, -1, 0.75));
	Sphere *s = new Sphere (Vector(-10, -30, 0), Vector(0,  150, 0), 50, 2);
	Sphere *s2 = new Sphere (Vector(20, 50, 0), Vector(0, 0, 150), 40, 0.6);
	
	//for each pixel
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			Vector offset(i - 128, j - 128, 0);
			Ray *ray = new Ray ((eye->orig) + offset , eye->look_at);
			float t = s->intersect(*ray, 0, std::numeric_limits<float>::infinity()); //start at t = 0, go to t = infinity
			if (t != 0) {
				Vector temp = shade(*ray, *s, *light, *eye, t);
				float RGB[] = {temp.x, temp.y, temp.z};
				img.draw_point(i, j, RGB);
			}
		}
	}
	
	//for each pixel
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			Vector offset(i - 128, j - 128, 0);
			Ray *ray = new Ray ((eye->orig) + offset , eye->look_at);
			float t = s2->intersect(*ray, 0, std::numeric_limits<float>::infinity()); //start at t = 0, go to t = infinity
			if (t != 0) {
				Vector temp = shade(*ray, *s2, *light, *eye, t);
				float RGB[] = {temp.x, temp.y, temp.z};
				img.draw_point(i, j, RGB);
			}
		}
	}
	
	img.display("Assignment #1");
	return 0;
}