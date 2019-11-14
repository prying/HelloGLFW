#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "linmath.h"
#include "stb_image.h"

#include "ShaderProgram.h"

#define VIEWPORT_W 800
#define VIEWPORT_H 600

#define VSYNC_ON 1

float alphaMix = 0.0f;

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
		// Pos				// colours		  // Texture	
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // top left 
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Settings
	// wrap image contiously
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// how the image is scaled 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("resources/waterTile.png", &width, &height, &nChannels, 0);

	if (!data)
		std::cout << "Failed to load texture\n";

	// Bind texture to GL_TEXTURE_2D and create minimap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	// Sprite 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Settings
	// wrap image contiously
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// how the image is scaled 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Flip water texture
	stbi_set_flip_vertically_on_load(false);
	data = stbi_load("resources/container.jpg", &width, &height, &nChannels, 0);

	if (!data)
		std::cout << "Failed to load texture\n";

	// Bind texture to GL_TEXTURE_2D and create minimap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	// Tell opengl how to deal with the textures
	shaderProgram.use();
	shaderProgram.setUniform("texture1", 0);
	shaderProgram.setUniform("texture2", 1);

	alphaMix = 0.5f;
	shaderProgram.setUniform("alphaMix", alphaMix);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		
		glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shaderProgram.setUniform("alphaMix", alphaMix);

		glBindVertexArray(VAO);
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

	if (key == GLFW_KEY_UP)
		alphaMix = alphaMix < 1.0f ? alphaMix+0.02f: alphaMix;

	if (key == GLFW_KEY_DOWN)
		alphaMix = alphaMix > 0.0f ? alphaMix-0.02f: alphaMix;

}
