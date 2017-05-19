#include "View.h"
#include <glm/gtx/rotate_vector.hpp>

View::View()
{
	projection	= glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.f);
	view		= glm::lookAt(eyes, centers, up);
	MVPMatrix	= projection * view;
}


View::~View()
{
}


void View::stepFoward() {
	glm::vec3 wersor = glm::normalize(centers - eyes);
	wersor *= distance;
	eyes += wersor;
	centers += wersor;
	view = glm::lookAt(eyes, centers, up);
	MVPMatrix = projection * view;
}

void View::stepBack() {
	glm::vec3 wersor = glm::normalize(centers - eyes);
	wersor *= distance;
	eyes -= wersor;
	centers -= wersor;
	view = glm::lookAt(eyes, centers, up);
	MVPMatrix = projection * view;
}
void View::rotateHorizontal(GLfloat direction) {
	glm::mat4x4 mat = glm::mat4x4();
	mat = glm::translate(mat, eyes - centers)	;
	mat = glm::rotate(mat, glm::radians(angle*direction), up);
	mat = glm::translate(mat, centers - eyes);
	centers = mat * glm::vec4(centers,1.0);
	view = glm::lookAt(eyes, centers, up);
	MVPMatrix = projection * view;
}

void View::rotateVertical(GLfloat direction) {
	glm::mat4x4 mat = glm::mat4x4();
	mat = glm::translate(mat, eyes - centers);
	mat = glm::rotate(mat, glm::radians(angle*direction), (glm::cross((centers - eyes), up)));
	mat = glm::translate(mat, centers - eyes);
	centers = mat * glm::vec4(centers, 1.0);
	view = glm::lookAt(eyes, centers, up);
	MVPMatrix = projection * view;
}

void View::setDistance(GLfloat z) {
	distance = z;
}
void View::setAngle(GLfloat z) {
	angle = z;
}