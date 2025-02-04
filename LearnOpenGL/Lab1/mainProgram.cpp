
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
	// positions			// texture coords
	-0.5f, -0.5f, -0.5f,	 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f
};

GLuint indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

#define DISPLAY_W 640
#define DISPLAY_H 480

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

	GLFWwindow* window = glfwCreateWindow(DISPLAY_W, DISPLAY_H, "Learn OpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// attatch the resize function callback
	gl3wInit();


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
	glEnable(GL_DEPTH_TEST);
	glUniform1i(glGetUniformLocation(shaderProgram.programID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram.programID, "texture2"), 1);
	glUniform1f(glGetUniformLocation(shaderProgram.programID, "mixValue"), mixValue);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	static const GLfloat bgd[] = { 0.46f, 0.48f, 0.71f, 1.f };
	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{
		//Input
		processInput(window);

		// Rendering
		glClearBufferfv(GL_COLOR, 0, bgd); // clears the colour buffer writing the specified colour over the entire screen+
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glUniform1f(glGetUniformLocation(shaderProgram.programID, "mixValue"), mixValue);

		// Coordinate Space Matrices
		// Initialise as identity matrices

		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 1.0, 0.0));

		glfwGetWindowSize(window, &width, &height);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f),
			(float)width / (float)height, 0.1f, 100.0f);

		//Apply to the uniforms
		int viewLoc = glGetUniformLocation(shaderProgram.programID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(shaderProgram.programID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		//Drawing the triangles
		glBindVertexArray(VAOs[0]);
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0 * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle),
				glm::vec3(0.5f, 1.0f, 0.0f));
			int modelLoc = glGetUniformLocation(shaderProgram.programID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		

		
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
