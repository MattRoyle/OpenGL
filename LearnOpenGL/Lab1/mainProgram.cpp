
#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// GLM (OpenGL Mathematics) https://github.com/g-truc/glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLfloat vertices[] = {
	// positions	 // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
	0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
};

GLuint indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};


#define NUM_BUFFERS 1
#define NUM_VAOS 1
#define NUM_EBOS 1
GLuint VBOs[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];
GLuint EBOs[NUM_EBOS];

void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height);
void processInput(GLFWwindow* window);
GLfloat mixValue = 0.2f;
int main()
{
	/*----------Window creation----------*/
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(640, 480, "A Triangle", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// attatch the resize function callback
	gl3wInit();

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum num of vertex attributes supported: " << nrAttributes << std::endl;

	// Create the program
	ShaderProgram shaderProgram("vertex.shader", "fragment.shader");

	// Set up vertex data and Buffers
	glGenVertexArrays(NUM_VAOS, VAOs);
	glGenBuffers(NUM_EBOS, VBOs);
	glGenBuffers(NUM_EBOS, EBOs);

	glBindVertexArray(VAOs[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//create the VAO (Vertex Attribute Object)
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/*------Textures-----------*/
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);//number of textures to create
	glBindTexture(GL_TEXTURE_2D, texture1);

	//set the wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load the image
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	//generate the texture
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture1" << std::endl;
		exit(1);
	}
	stbi_image_free(data);
	data = nullptr;
	glGenTextures(1, &texture2);//number of textures to create
	glBindTexture(GL_TEXTURE_2D, texture2);

	//set the wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, data);//alpha channel included
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture2" << std::endl;
		exit(1);
	}
	stbi_image_free(data);
	glUseProgram(shaderProgram.programID);

	glUniform1i(glGetUniformLocation(shaderProgram.programID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram.programID, "texture2"), 1);
	glUniform1f(glGetUniformLocation(shaderProgram.programID, "mixValue"), mixValue);

	

	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{
		static const GLfloat bgd[] = { 1.f, 0.f, 0.f, 1.f };
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(),
			glm::vec3(0.0f, 1.0f, 0.0f));
		//Input
		processInput(window);

		// rendering commands
		
		glClearBufferfv(GL_COLOR, 0, bgd); // clears the colour buffer writing the specified colour over the entire screen+
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glUniform1f(glGetUniformLocation(shaderProgram.programID, "mixValue"), mixValue);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.programID, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
		glUseProgram(shaderProgram.programID);
		glBindVertexArray(VAOs[0]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//swapping buffers and polling events
		glfwSwapBuffers(window);// swap the new colour buffer
		glfwPollEvents(); //checks for inputs (mouse, keyboard)
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}

/*GLFW change window size*/
void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height)
{
	glViewport(0, 0, width, height);
}

/*Process inputs by querying the appropiate keys*/
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)// exit when esc is pressed
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixValue += 0.0001f;
		if (mixValue >= 1.0f) {
			mixValue == 1.0f;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixValue -= 0.0001f;
		if (mixValue <= 0.0f) {
			mixValue == 0.0f;
		}
	}
}
