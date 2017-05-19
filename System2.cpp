#include "System2.h"
#include <iomanip>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>



System2::System2() {
	srand(time(NULL));
	buffor = std::vector<float>();
	d = Delaunay(buffor);
	for (int i = 0; i < SIZE; i++) {
		points[i] = rand() / (double)RAND_MAX * 150 + 20;
	}
	

	prog = glCreateProgram();
	glAttachShader(prog, LoadShader(GL_VERTEX_SHADER, "shaders/patyki_vs.glsl"));
	glAttachShader(prog, LoadShader(GL_FRAGMENT_SHADER, "shaders/patyki_fs.glsl"));


	LinkValidateProgram(prog);
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
}

void System2::process()
{
	d.BowyerWatsonAdd(points, count, buffor);
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	std::cout << buffor.size() << std::endl;
	if(buffor.size() != 0)
	glBufferSubData(GL_ARRAY_BUFFER, 0, buffor.size() * sizeof(float), &buffor[0]);
	glFlush();
	count++;
}

void System2::render(glm::mat4x4 MVP)
{
	glClearColor(1.0f, 1.0f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vertexArray);
	glUseProgram(prog);
	glLineWidth(1);
	glUniformMatrix4fv(glGetUniformLocation(prog, "modelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
	glDrawArrays(GL_LINES, 0, buffor.size()/3);
}

System2::~System2()
{
	glDeleteProgram(prog);
	glDeleteVertexArrays(1, &vertexArray);
}
