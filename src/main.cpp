#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "linmath.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.hpp"
#include "Camera.hpp"

#define VIEWPORT_W 800
#define VIEWPORT_H 600

#define VSYNC_ON 1

// Prototypes
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Camera
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Inital mouse click;
float lastX = VIEWPORT_W/2.0f;
float lastY = VIEWPORT_W/2.0f;

float deltaFrame = 0.0f;
float lastFrame = 0.0f;

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
	camera.setViewPort(window);
	
	// Initilize GLAD
	if (!gladLoadGL(glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD2\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, VIEWPORT_W, VIEWPORT_H);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);
	// test

	// 'vsync'
	glfwSwapInterval(VSYNC_ON);

	// Load shaders
	ShaderProgram shaderProgram;
	shaderProgram.addShader("shaders/vertex.vert", GL_VERTEX_SHADER);
	shaderProgram.addShader("shaders/fragment.frag", GL_FRAGMENT_SHADER);
	shaderProgram.link();

	// Set up vertex data (and buffers) and config vertex attibutes
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Cube locations
	glm::vec3 cubePos[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(1.5f, 1.0f, -1.5f)
	};

	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO); // All the settings live in the VAO
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	// Texture the object
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Bind settings to the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load sprite
	int width, height, nChannels;
	uint8_t * imageData = stbi_load("resources/container.jpg", &width, &height, &nChannels, 0);

	if (imageData){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(imageData);

	shaderProgram.use();
	shaderProgram.setUniform("texture1", 0);

	// Use z to work out what needs to be drawn
	glEnable(GL_DEPTH_TEST);

	// Mouse inputs 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// FPS
		float currentframe = glfwGetTime();
		deltaFrame = currentframe - lastFrame;
		lastFrame = currentframe;

		processInput(window);
		camera.updateVectors();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();
		shaderProgram.setUniform("view", camera.getViewMatrix());
		shaderProgram.setUniform("proj", camera.getProjectionMatrix());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO);

		for (int i = 0; i<5; i++){
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePos[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shaderProgram.setUniform("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Check and call events + swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	window = NULL;
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

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move(FORWARD, deltaFrame);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move(BACKWARD, deltaFrame);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.move(LEFT, deltaFrame);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.move(RIGHT, deltaFrame);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.move(UP, deltaFrame);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.move(DOWN, deltaFrame);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos){
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos; lastY = ypos;

	camera.rotate(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	camera.zoom(yoffset);
}