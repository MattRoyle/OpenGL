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
	return nullptr;
}

class ShaderProgram
{
public: 
	GLuint programID;

	ShaderProgram(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
		GLint success;
		GLchar infoLog[512];
		const char* vertexShaderSource = load_shader(vertexPath);
		const char* fragmentShaderSource = load_shader(fragmentPath);
		// Vertex Shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);// call gl to create a shader returning the ID of the vertex 
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attatch the shader passing in the program as a string, the 4th param is optional a buffer to store the returned compiled code
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR:Vertex Shader failed to compile\n" << infoLog << std::endl;
		}

		// Fragment Shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR:Fragment Shader failed to compile\n" << infoLog << std::endl;
		}

		// Shader Program
		programID = glCreateProgram();
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);

		glGetProgramiv(programID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "ERROR:Program failed to link\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
};
#endif