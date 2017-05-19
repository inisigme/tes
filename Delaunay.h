#pragma once
#include <array>
#include <vector>
#include <iostream>
#include <tuple>
#include <math.h>
using std::get;




class Point {
public:
	float x, y, z;
	Point(float _x, float _y, float _z);
	Point();
    static Point cross(const Point a, const Point b);
	static float len(const Point &);
	static float distance(const Point &, const Point &);
	static float dot(const Point &a, const Point& b);

	Point operator-(Point &p);
	Point operator+(Point &p);
	bool SameSide(Point p1, Point p2, Point p3, Point p4, Point p);
	bool PointInTetrahedron(Point p1, Point p2, Point p3, Point p4, Point p);
};


bool operator==(const Point &p1, const Point &p2);

bool operator==(std::tuple<Point, Point, Point> arg1, std::tuple<Point, Point, Point> arg2);


class Tetrahedron {
public:
	Point vertices[4];
	Tetrahedron(Point _p1, Point _p2, Point _p3, Point _p4);
	bool enclude(Point);
};

bool operator==(const Tetrahedron &t1, const Tetrahedron &t2);

class Delaunay {
public:
	std::vector<Point> points = std::vector<Point>();
	std::vector<Tetrahedron> tetrahedrons = std::vector<Tetrahedron>();

	void end();
	Delaunay() {};
	Delaunay(std::vector<float> & arg);
	void BowyerWatsonAdd(float *points, unsigned int no, std::vector<float> &);
	std::vector < std::tuple<Point, Point, Point>> GetBoundary(std::vector<Tetrahedron> arg);
};