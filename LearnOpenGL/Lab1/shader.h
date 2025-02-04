#ifndef SHADER_H
#define SHADER_H
// Include gl3w headers
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
// standard headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <cstdlib>

const char* load_shader(const char* shaderPath) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string shaderString;
	std::ifstream shaderFile;
	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		shaderFile.open(shaderPath);

		std::stringstream shaderStream;
		// read file’s buffer contents into streams
		shaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();

		// convert stream into string
		shaderString = shaderStream.str();
		const char * shaderCode = _strdup(shaderString.c_str());
		return shaderCode;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ:" << shaderPath << std::endl;
	}
	return (char *)'\0';
}


#endif