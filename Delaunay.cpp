#pragma once
#include "Delaunay.h"
#include <math.h>
using std::get;
#define EPS 10000.0
typedef struct {
	double x, y, z;
} XYZ;

int LineLineIntersect(
	XYZ p1, XYZ p2, XYZ p3, XYZ p4, XYZ *pa, XYZ *pb,
	double *mua, double *mub)
{
	XYZ p13, p43, p21;
	double d1343, d4321, d1321, d4343, d2121;
	double numer, denom;

	p13.x = p1.x - p3.x;
	p13.y = p1.y - p3.y;
	p13.z = p1.z - p3.z;
	p43.x = p4.x - p3.x;
	p43.y = p4.y - p3.y;
	p43.z = p4.z - p3.z;
	if (abs(p43.x) < EPS && abs(p43.y) < EPS && abs(p43.z) < EPS)
		return(false);
	p21.x = p2.x - p1.x;
	p21.y = p2.y - p1.y;
	p21.z = p2.z - p1.z;
	if (abs(p21.x) < EPS && abs(p21.y) < EPS && abs(p21.z) < EPS)
		return(false);

	d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
	d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
	d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
	d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
	d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

	denom = d2121 * d4343 - d4321 * d4321;
	if (abs(denom) < EPS)
		return(false);
	numer = d1343 * d4321 - d1321 * d4343;

	*mua = numer / denom;
	*mub = (d1343 + d4321 * (*mua)) / d4343;

	pa->x = p1.x + *mua * p21.x;
	pa->y = p1.y + *mua * p21.y;
	pa->z = p1.z + *mua * p21.z;
	pb->x = p3.x + *mub * p43.x;
	pb->y = p3.y + *mub * p43.y;
	pb->z = p3.z + *mub * p43.z;

	return(true);
}

Point::Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
Point::Point() {};

Point Point::operator-(Point &p) {
	return Point(x - p.x, y - p.y, z - p.z);
}

Point Point::operator+(Point &p) {
	return Point(x + p.x, y + p.y, z + p.z);
}

float Point::dot(const Point &a, const Point& b)
{
	return (a.x*b.x + a.y*b.y + a.z*b.z);
}

Point Point::cross(const Point a, const Point b)
{
	return Point(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

float Point::len(const Point &a)
{
	return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}

float Point::distance(const Point &a, const Point &b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

Point operator * (const float & f, const Point &p) {
	return Point(f*p.x, f*p.y, f*p.z);
}

bool Point::SameSide(Point p1, Point p2, Point p3, Point p4, Point p)
{
	auto normal = cross(p2 - p1, p3 - p1);
	auto dotV4 = dot(normal, p4 - p1);
	auto dotP = dot(normal, p - p1);
	return dotV4 * dotP > 0;
}

bool Point::PointInTetrahedron(Point p1, Point p2, Point p3, Point p4, Point p)
{
	return SameSide(p1, p2, p3, p4, p) &&
		SameSide(p2, p3, p4, p1, p) &&
		SameSide(p3, p4, p1, p2, p) &&
		SameSide(p4, p1, p2, p3, p) ;
}


bool operator==(const Point &p1, const Point &p2) {
	return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}

//tak, wiem jak to wyglada
bool operator==(std::tuple<Point, Point, Point> arg1, std::tuple<Point, Point, Point> arg2) {
	return (get<0>(arg1) == get<0>(arg2) || get<0>(arg1) == get<1>(arg2) || get<0>(arg1) == get<2>(arg2)) &&
		(get<1>(arg1) == get<0>(arg2) || get<1>(arg1) == get<1>(arg2) || get<1>(arg1) == get<2>(arg2)) &&
		(get<2>(arg1) == get<0>(arg2) || get<2>(arg1) == get<1>(arg2) || get<2>(arg1) == get<2>(arg2));
};

Tetrahedron::Tetrahedron(Point _p1, Point _p2, Point _p3, Point _p4) {
	vertices[0] = _p1; vertices[1] = _p2;
	vertices[2] = _p3; vertices[3] = _p4;
}

bool operator==(const Tetrahedron &t1, const Tetrahedron &t2) {
	int l = 0;
	for (auto v : t1.vertices)
		for (auto vv : t2.vertices)
			if (v == vv) l++;
	return l == 4;
}

void Delaunay::end()
{
		
}

bool Tetrahedron::enclude(Point doSprawdzenia)
{
	Point P1(vertices[0]);
	Point P2(vertices[1]);
	Point P3(vertices[2]);
	Point P4(vertices[3]);

	float alpha = pow(Point::len(P2 - P3),2) * Point::dot(P1 - P2,P1-P3) / 2 / pow ( Point::len(Point::cross(P1-P2,P2-P3)) , 2);
	float beta = pow(Point::len(P1 - P3), 2) * Point::dot(P2 - P1, P2 - P3) / 2 / pow(Point::len(Point::cross(P1 - P2, P2 - P3)), 2);
	float theta = pow(Point::len(P1 - P2), 2) * Point::dot(P3 - P1, P3 - P2) / 2 / pow(Point::len(Point::cross(P1 - P2, P2 - P3)), 2);

	Point center1 = alpha * P1 + beta * P2 + theta * P3;

	Point direction = Point::cross(P1 - P2, P1 - P3);
	Point anotherCenter1(center1 + direction);

	P1 = Point(vertices[3]);
	P2 = Point(vertices[2]);
	P3 = Point(vertices[1]);
	P4 = Point(vertices[0]);

	alpha = pow(Point::len(P2 - P3), 2) * Point::dot(P1 - P2, P1 - P3) / 2 / pow(Point::len(Point::cross(P1 - P2, P2 - P3)), 2);
	beta = pow(Point::len(P1 - P3), 2) * Point::dot(P2 - P1, P2 - P3) / 2 / pow(Point::len(Point::cross(P1 - P2, P2 - P3)), 2);
	theta = pow(Point::len(P1 - P2), 2) * Point::dot(P3 - P1, P3 - P2) / 2 / pow(Point::len(Point::cross(P1 - P2, P2 - P3)), 2);

	Point center2 = alpha * P1 + beta * P2 + theta * P3;
	direction = Point::cross(P3 - P2, P3 - P1);
	Point anotherCenter2(center1 + direction);


	XYZ p1 = { center1.x, center1.y, center1.z };
	XYZ p2 = { anotherCenter1.x, anotherCenter1.y, anotherCenter1.z };
	XYZ p3 = { center2.x, center2.y, center2.z };
	XYZ p4 = { anotherCenter2.x, anotherCenter2.y, anotherCenter2.z };

	XYZ pa, pb;
	double mua, mub;

	auto ret = LineLineIntersect(p1, p2, p3, p4, &pa, &pb, &mua, &mub);


	Point center(pa.x, pa.y, pa.z);
	if (Point::distance(center, doSprawdzenia) < Point::distance(vertices[0], center))
		return true;
	else
		return false;
}

Delaunay::Delaunay(std::vector<float> &arg) {
	tetrahedrons.push_back(Tetrahedron(Point(0.f, 0.f, 0.f), Point(400.f, 0.f, 0.f),
		Point(200.f, 400.f, 200.f), Point(0.f, 0.f, 400.f)));
	arg.clear();
	for (auto t : tetrahedrons) {
		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				if (i < j && t.vertices[i].y > 2.0 && t.vertices[i].y < 190 && t.vertices[j].y > 2.0 && t.vertices[j].y < 190) {
					arg.push_back(t.vertices[i].x);
					arg.push_back(t.vertices[i].y);
					arg.push_back(t.vertices[i].z);

					arg.push_back(t.vertices[j].x);
					arg.push_back(t.vertices[j].y);
					arg.push_back(t.vertices[j].z);
				}
			}
		}
	}
}


void Delaunay::BowyerWatsonAdd(float *points, unsigned int no, std::vector<float> &arg) {
	Point p(points[no * 3], points[no * 3 + 1], points[no * 3 + 2]);
	std::vector<Tetrahedron> bad_tetrahedrons;

	for (auto t : tetrahedrons) {
		if (t.enclude(p))
			bad_tetrahedrons.push_back(t);
	}

	
	std::vector<std::tuple<Point, Point, Point>> new_tetrahedrons = GetBoundary(bad_tetrahedrons);
	for (auto t : bad_tetrahedrons)
		tetrahedrons.erase(std::remove(tetrahedrons.begin(), tetrahedrons.end(), t), tetrahedrons.end());
	
	for (auto t : new_tetrahedrons)
		tetrahedrons.push_back(Tetrahedron(get<0>(t), get<1>(t), get<2>(t), p));

	unsigned int licznik = 0;
	arg.clear();
	for (unsigned int ww = 0;ww < this->tetrahedrons.size(); ww++) {

	for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				if ( i < j ) {
					arg.push_back(this->tetrahedrons[ww].vertices[i].x);
					arg.push_back(this->tetrahedrons[ww].vertices[i].y);
					arg.push_back(this->tetrahedrons[ww].vertices[i].z);

					arg.push_back(this->tetrahedrons[ww].vertices[j].x);
					arg.push_back(this->tetrahedrons[ww].vertices[j].y);
					arg.push_back(this->tetrahedrons[ww].vertices[j].z);
				}
			}
		}
	}
}

std::vector < std::tuple<Point, Point, Point>> Delaunay::GetBoundary(std::vector<Tetrahedron> arg) {

	std::vector<std::tuple<Point, Point, Point>> all;

	for (auto t : arg)
		for (unsigned int i : {0, 1, 2, 3}) {
			all.push_back(std::tuple<Point, Point, Point>(t.vertices[i], t.vertices[(i + 1) % 4], t.vertices[(i + 2) % 4]));
		}
	static int a = 0;
		for (unsigned int i = 0; i < all.size(); i++)
			for (unsigned int j = 0; j < all.size(); j++)
				if (i != j)
					if (all[i] == all[j]) {
						if (i < j) {
							all.erase(all.begin() + j); j--;
							all.erase(all.begin() + i); i--;
						}
						else {
							all.erase(all.begin() + i); i--;
							all.erase(all.begin() + j); j--;
						}
					}
		a = 0;
	return all;
}
