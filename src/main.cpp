#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "linmath.h"

#define VIEWPORT_W 800
#define VIEWPORT_H 600

#define VSYNC_ON 1

// Prototypes
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void){
	if (!glfwInit()) {
		std::cout << "Failed to initialze GLFW\n";
		exit(EXIT_FAILURE);
	}

	// Set callback for errors
	glfwSetErrorCallback(error_callback);

	// Set the OpenGL verison as 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Mac OS X
	#if (__APPLE__ || __MACH__)
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
	#endif

	// Create a window
	GLFWwindow* window = glfwCreateWindow(VIEWPORT_W, VIEWPORT_H, "Hellow GLFW!!", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create window\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// Initilize GLAD
	if (!gladLoadGL(glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD2\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, VIEWPORT_W, VIEWPORT_H);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);

	// 'vsync'
	glfwSwapInterval(VSYNC_ON);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Check and call events + swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

/*
 * Prints GLFW errors
 */
void error_callback(int error, const char* description) {
	std::fprintf(stderr, "Error: %s\n", description);
}

/*
 * Callback for window resizing
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/*
 * Callback for key press
 */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}