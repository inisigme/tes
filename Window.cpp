#include "Window.h"
#include <conio.h>

GLFWwindow * Window::window;
View * Window::view;
System2 *Window::s2;
Window::Window()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1024, 768, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(432432);
	}
	glfwMakeContextCurrent(window);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSizeCallback(window, Resize);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseClickCallback);
	glfwSetCursorPosCallback(window, MouseMotionCallback);

	GLenum error = glGetError();

	if (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error: " << error << std::endl;
	}

	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		getchar();
		exit(EXIT_FAILURE);
	}


	glfwSwapInterval(0);
	view = new View();
	s2 = new System2();

	//glDisable(GL_BLEND);
	mainLoop();
}
void Window::mainLoop() {
	while(1){
		ERR
		renderScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	
	}
}

void Window::renderScene() {
	printFPS(glfwGetTime());
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	s2->render(this->view->MVPMatrix);
}

void Window::Resize(GLFWwindow* window, int w, int h) {
	if (h < 1)
		h = 1;
	glViewport(0, 0, w, h);
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_W:
		view->stepFoward();
		
		break;
	case GLFW_KEY_S:
		view->stepBack();
		break;
	case GLFW_KEY_E:
		static float time = glfwGetTime();
		if (time + 0.5 < glfwGetTime()) {
			s2->process();
			time = glfwGetTime();
		}
		break;
	default:
		break;
	}
}

void Window::MouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
	
}

void Window::MouseMotionCallback(GLFWwindow* window, double x, double y) {
	static int lastx = -1;
	static int lasty = -1;
	double deltax =  lastx-x;
	double deltay = lasty - y;
	lastx = x;
	lasty = y;
	view->rotateHorizontal((GLfloat)deltax);
	view->rotateVertical((GLfloat)deltay);
}

void Window::error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

Window::~Window()
{
	delete view;
	glfwDestroyWindow(window);
}
