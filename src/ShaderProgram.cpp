#include "shaderProgram.hpp"

#include <string>
#include <iostream>
#include <algorithm>
#include "tools.hpp"
#include <glm\gtc\type_ptr.hpp>

#define INFOLOG_SIZE 512

void ShaderProgram::addShader(const char* filename, GLenum type) {

	// Prosses file
	std::string shaderCode;
	const char * shaderCodeStr;
	GLuint shader;

	shaderCode = get_file_contents(filename);
	shaderCodeStr = shaderCode.c_str();

	// Compile shader
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCodeStr, NULL);
	glCompileShader(shader);

	// Check for compliation errors
	int success;
	char infoLog[INFOLOG_SIZE];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Add shader to list 
	compliedShaders.push_back(shader);

	return;
}

/**
 * @brief Add shaders from linked list and link them to the shader program then delet them
 * 
 */
void ShaderProgram::link(){
	
	// Creat new shader program
	program = glCreateProgram();

	// Attach each shader
	for(auto shader : compliedShaders){
		glAttachShader(program, shader);
	}

	// Link shaders
	glLinkProgram(program);

	// Check for compliation errors
	int success;
	char infoLog[INFOLOG_SIZE];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delet old shaders
	std::for_each(compliedShaders.begin(), compliedShaders.end(), [](GLuint s){glDeleteShader(s);});
	compliedShaders.clear();
}

GLuint ShaderProgram::get(){
	
	if (!program){
		std::cout << "Error no linked shader program\n";
		throw errno;
	}
	return program;
}

void ShaderProgram::use(){

	if (!program){
		std::cout << "Error no linked shader program\n";
		throw errno;
	}
	glUseProgram(program);
	return;
}

void ShaderProgram::deleteProgram(){
	glDeleteProgram(program);
	program = NULL;
	return;
}

ShaderProgram::~ShaderProgram(){
	if (program)
		deleteProgram();
}

void ShaderProgram::setUniform(const std::string &name, int value){
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string &name, float value){
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string &name, float x, float y, float z){
	glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void ShaderProgram::setUniform(const std::string& name, glm::mat4 mat){
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
