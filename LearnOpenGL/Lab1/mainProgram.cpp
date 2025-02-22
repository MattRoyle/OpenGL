
#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// GLM (OpenGL Mathematics) https://github.com/g-truc/glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Custom Headers
#include "shader.h"
#include "camera.h"

GLfloat vertices[] = {
	// positions			// normals				// texture coords
	-0.5f,	 -0.5f,	 -0.5f,	 0.0f,	 0.0f,	 -1.0f,	 0.0f,	 0.0f,	
	0.5f,	 -0.5f,	 -0.5f,	 0.0f,	 0.0f,	 -1.0f,	 1.0f,	 0.0f,	
	0.5f,	 0.5f,	 -0.5f,	 0.0f,	 0.0f,	 -1.0f,	 1.0f,	 1.0f,	
	0.5f,	 0.5f,	 -0.5f,	 0.0f,	 0.0f,	 -1.0f,	 1.0f,	 1.0f,	
	-0.5f,	 0.5f,	 -0.5f,	 0.0f,	 0.0f,	 -1.0f,	 0.0f,	 1.0f,	
	-0.5f,	 -0.5f,	 -0.5f,	 0.0f,	 0.0f,	 -1.0f,	 0.0f,	 0.0f,	
	-0.5f,	 -0.5f,	 0.5f,	 0.0f,	 0.0f,	 1.0f,	 0.0f,	 0.0f,	
	0.5f,	 -0.5f,	 0.5f,	 0.0f,	 0.0f,	 1.0f,	 1.0f,	 0.0f,	
	0.5f,	 0.5f,	 0.5f,	 0.0f,	 0.0f,	 1.0f,	 1.0f,	 1.0f,	
	0.5f,	 0.5f,	 0.5f,	 0.0f,	 0.0f,	 1.0f,	 1.0f,	 1.0f,	
	-0.5f,	 0.5f,	 0.5f,	 0.0f,	 0.0f,	 1.0f,	 0.0f,	 1.0f,	
	-0.5f,	 -0.5f,	 0.5f,	 0.0f,	 0.0f,	 1.0f,	 0.0f,	 0.0f,	
	-0.5f,	 0.5f,	 0.5f,	 -1.0f,	 0.0f,	 0.0f,	 1.0f,	 0.0f,	
	-0.5f,	 0.5f,	 -0.5f,	 -1.0f,	 0.0f,	 0.0f,	 1.0f,	 1.0f,	
	-0.5f,	 -0.5f,	 -0.5f,	 -1.0f,	 0.0f,	 0.0f,	 0.0f,	 1.0f,	
	-0.5f,	 -0.5f,	 -0.5f,	 -1.0f,	 0.0f,	 0.0f,	 0.0f,	 1.0f,	
	-0.5f,	 -0.5f,	 0.5f,	 -1.0f,	 0.0f,	 0.0f,	 0.0f,	 0.0f,	
	-0.5f,	 0.5f,	 0.5f,	 -1.0f,	 0.0f,	 0.0f,	 1.0f,	 0.0f,	
	0.5f,	 0.5f,	 0.5f,	 1.0f,	 0.0f,	 0.0f,	 1.0f,	 0.0f,	
	0.5f,	 0.5f,	 -0.5f,	 1.0f,	 0.0f,	 0.0f,	 1.0f,	 1.0f,	
	0.5f,	 -0.5f,	 -0.5f,	 1.0f,	 0.0f,	 0.0f,	 0.0f,	 1.0f,	
	0.5f,	 -0.5f,	 -0.5f,	 1.0f,	 0.0f,	 0.0f,	 0.0f,	 1.0f,	
	0.5f,	 -0.5f,	 0.5f,	 1.0f,	 0.0f,	 0.0f,	 0.0f,	 0.0f,	
	0.5f,	 0.5f,	 0.5f,	 1.0f,	 0.0f,	 0.0f,	 1.0f,	 0.0f,	
	-0.5f,	 -0.5f,	 -0.5f,	 0.0f,	 -1.0f,	 0.0f,	 0.0f,	 1.0f,	
	0.5f,	 -0.5f,	 -0.5f,	 0.0f,	 -1.0f,	 0.0f,	 1.0f,	 1.0f,	
	0.5f,	 -0.5f,	 0.5f,	 0.0f,	 -1.0f,	 0.0f,	 1.0f,	 0.0f,	
	0.5f,	 -0.5f,	 0.5f,	 0.0f,	 -1.0f,	 0.0f,	 1.0f,	 0.0f,	
	-0.5f,	 -0.5f,	 0.5f,	 0.0f,	 -1.0f,	 0.0f,	 0.0f,	 0.0f,	
	-0.5f,	 -0.5f,	 -0.5f,	 0.0f,	 -1.0f,	 0.0f,	 0.0f,	 1.0f,	
	-0.5f,	 0.5f,	 -0.5f,	 0.0f,	 1.0f,	 0.0f,	 0.0f,	 1.0f,	
	0.5f,	 0.5f,	 -0.5f,	 0.0f,	 1.0f,	 0.0f,	 1.0f,	 1.0f,	
	0.5f,	 0.5f,	 0.5f,	 0.0f,	 1.0f,	 0.0f,	 1.0f,	 0.0f,	
	0.5f,	 0.5f,	 0.5f,	 0.0f,	 1.0f,	 0.0f,	 1.0f,	 0.0f,	
	-0.5f,	 0.5f,	 0.5f,	 0.0f,	 1.0f,	 0.0f,	 0.0f,	 0.0f,	
	-0.5f,	 0.5f,	 -0.5f,	 0.0f,	 1.0f,	 0.0f,	 0.0f,	 1.0f
};
glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
};
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

#define DISPLAY_W 640
#define DISPLAY_H 480

#define NUM_VBOS 1
#define NUM_VAOS 2
#define NUM_EBOS 1

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);

// Scene Global Variables
GLuint VBOs[NUM_VBOS];
GLuint VAOs[NUM_VAOS];
GLuint EBOs[NUM_EBOS];

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
Camera camera(glm::vec3(0.0f, 1.0f, 6.0f));
float lastX = DISPLAY_W/2, lastY = DISPLAY_H / 2; //position of the mouse on the previous update, intialised to the center
bool firstMouse = true;


int main()
{
	/*----------Window creation----------*/
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(DISPLAY_W, DISPLAY_H, "Learn OpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// attatch the resize function callback
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	gl3wInit();

	

	// Create the program
	ShaderProgram lightingShader("colour.vs", "colour.fs");
	ShaderProgram cubeLightShader("light.vs", "light.fs");


	// Set up vertex data and Buffers
	glGenVertexArrays(2, VAOs);
	glGenBuffers(1, VBOs);

	// Set up the cube
	glBindVertexArray(VAOs[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Set up the Light
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	
	

	// Done outside the render loop as they dont change
	glUseProgram(lightingShader.programID);
	
	
	
	glUniform1i(glGetUniformLocation(lightingShader.programID, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.programID, "material.specular"), 1);
	glUniform1f(glGetUniformLocation(lightingShader.programID, "material.shininess"),32.0f);

	glUniform3f(glGetUniformLocation(lightingShader.programID, "light.direction"),-0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(lightingShader.programID, "light.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(lightingShader.programID, "light.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(lightingShader.programID, "light.specular"), 1.0f, 1.0f, 1.0f);
	
	unsigned int diffuseMap = loadTexture("textures/container2.png");
	unsigned int specularMap = loadTexture("textures/container2_specular.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	int width, height;
	static const GLfloat bgd[] = { 0.46f, 0.48f, 0.71f, 1.f };
	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//Input
		processInput(window);

		// Rendering
		glClearBufferfv(GL_COLOR, 0, bgd); // clears the colour buffer writing the specified colour over the entire screen+
		glClear(GL_DEPTH_BUFFER_BIT);

		// Coordinate Space Matricesqq
		// Initialise as identity matrices

		glUseProgram(lightingShader.programID);
		glUniform3fv(glGetUniformLocation(lightingShader.programID, "viewPos"), 1, &camera.position[0]);
		// view/projection transformations
		glfwGetWindowSize(window, &width, &height);
		glm::mat4 projection = glm::perspective(camera.fov, (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		//Apply to the uniforms
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		
		//world transformations
		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glm::mat3 normalMatrix = glm::mat4(1.0f);
		//render the cube
		glBindVertexArray(VAOs[0]);
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			normalMatrix = glm::mat3(transpose(inverse(model)));
			glUniformMatrix3fv(glGetUniformLocation(lightingShader.programID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

			glUniformMatrix4fv(glGetUniformLocation(lightingShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		//Lamp Object
		/*glUseProgram(cubeLightShader.programID);
		glUniformMatrix4fv(glGetUniformLocation(cubeLightShader.programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(cubeLightShader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		glUniformMatrix4fv(glGetUniformLocation(cubeLightShader.programID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/
		//swapping buffers and polling events
		glfwSwapBuffers(window);// swap the new colour buffer
		glfwPollEvents(); //checks for inputs (mouse, keyboard)
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse){//prevent jumpyness
		lastX = xpos;
		lastY = ypos;
		firstMouse = false; 
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

/*GLFW change window size*/
void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height)
{
	glViewport(0, 0, width, height);
	firstMouse = true;
}

/*Process inputs by querying the appropiate keys*/
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)// exit when esc is pressed
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

}
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}