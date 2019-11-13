#pragma once
#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

/**
 * @brief shader loader object loads shaders then links them
 * on ShaderLoader::link()
 * 
 */
class ShaderProgram {
private:
	std::vector<GLuint> compliedShaders;
	GLuint program = NULL;

public:
	~ShaderProgram();

	/**
	 * @brief Adds and compiles the shader
	 * 
	 * @param filename of the shader
	 * @param type i.e GL_VERTEX_SHADER
	 */
	void addShader(const char* filename, GLenum type);

	/**
	 * @brief Links the previously added shaders.
	 * If ShaderLoader::link() has previously been called befor none of the added shaders
	 * befor the call will be added
	 */
	void link();

	/**
	 * @brief Gets the shader program
	 * 
	 * @return complied shader
	 */
	GLuint get();

	/**
	 * @brief Deletes current shader program
	 * 
	 */
	void deleteProgram();
};