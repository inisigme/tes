#pragma once
#include "wszystko.h"
#include <ctime>
#include "View.h"
#include <vector>
#include "Delaunay.h"

#define SIZE 10000
class System2
{
public:
	Delaunay d;
	unsigned int count = 1;
	float points[SIZE * 3];
	std::vector<float> buffor = std::vector<float>();
	System2();
	void process();
	int frame_layers = 1; // MOTION BLUR
	GLuint prog;
	GLuint vertexArray, vertexBuffer;
	void render(glm::mat4x4 MVPMatrix);
	~System2();
};

