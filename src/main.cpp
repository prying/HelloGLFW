#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "linmath.h"

#include "ShaderProgram.h"

#define VIEWPORT_W 800
#define VIEWPORT_H 600

#define VSYNC_ON 1

// Prototypes
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void){

	char buffer[100];
	char* answer = getcwd(buffer, sizeof(buffer));
	std::string s_cwd;
	if (answer)
		s_cwd = answer;
	std::cout << "pwd " << s_cwd << std::endl;

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

	// Load shaders
	ShaderProgram shaderProgram;

	shaderProgram.addShader("shaders/vertex.vert", GL_VERTEX_SHADER);
	shaderProgram.addShader("shaders/fragment.frag", GL_FRAGMENT_SHADER);

	shaderProgram.link();

	// Set up vertex data (and buffers) and config vertex attibutes
	float vertices[] = {
		-0.5f, 0.5f, 0.0f,
		-0.75f, -0.5f, 0.0f,
		-0.25f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.25f, -0.5f, 0.0f,
		0.75f, -0.5f, 0.0f 
	};

	float colours[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f
	};

	unsigned int indices[] = { 
		0, 1, 2,   // first triangle
		3, 4, 5    // second triangle
	};

	GLuint VBO, cVBO, VAO, EBO;
	glGenVertexArrays(1, &VAO); // All the settings live in the VAO
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &cVBO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	// Setup colours vbo
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. then set the vertex attributes pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	// render loop
	while (!glfwWindowShouldClose(window)) {
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram.get());
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// Check and call events + swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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
