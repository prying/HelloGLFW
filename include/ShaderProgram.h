#pragma once
#include <vector>
#include <string>

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
	 * @brief Adds and compiles the shader, needs to be called befor ShaderProgram::link()
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
	 * @brief Runs/uses the shader program (glUseProgram)
	 * 
	 */
	void use();

	/**
	 * @brief Deletes current shader program
	 * 
	 */
	void deleteProgram();

	/**
	 * @brief Set the Uniform object
	 * 
	 * @param name of the variable
	 * @param value 
	 */
	void setUniform(const std::string &name, int value);

	/**
	 * @brief Set the Uniform object
	 * 
	 * @param name of the variable
	 * @param value 
	 */
	void setUniform(const std::string &name, float value);

	/**
	 * @brief Set the Uniform object
	 * 
	 * @param name of the variable
	 * @param value 
	 */
	void setUniform(const std::string &name, float x, float y, float z);
};