
#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


GLfloat vertices[] = {
	0.5f, 0.5f, 0.0f, // top right
	0.5f, -0.5f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f // top left
};

GLuint indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
};


//Shaders are dynamically compiled at run time

//process the input vertices into triangles
const char* vertexShaderSource =
"#version 450 core\n"
"layout(location = 0) in vec3 aPos; // position has attribute position 0\n"
"out vec4 vertexColor; // specify a color output to the fragment shader\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0); // we give a vec3 to vec4’s constructor\n"
"	vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // output variable to dark-red\n"
"}\n";//GLSL (OpenGL Shading Language), in vertex shaders each variable is known as a vertex attribute

// process the pixel colour of the triangles
const char* fragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor; // we set this variable in the OpenGL code.\n"
"void main()\n"
"{\n"
"	FragColor = ourColor;\n"
"}\n";


#define NUM_BUFFERS 1
#define NUM_VAOS 1
#define NUM_EBOS 1
GLuint Buffers[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];
GLuint EBOs[NUM_EBOS];
void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height);
void processInput(GLFWwindow* window);

int main()
{
	
	/*----------Window creation----------*/
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(640, 480, "A Triangle", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// attatch the resize function callback
	gl3wInit();

	glCreateBuffers(NUM_BUFFERS, Buffers);
	glNamedBufferStorage(Buffers[0], sizeof(vertices), vertices, 0);
	
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum num of vertex attributes supported: " << nrAttributes << std::endl;
	/*----------Vertex shader----------*/
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);// call gl to create a shader returning the ID of the vertex 
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attatch the shader passing in the program as a string, the 4th param is optional a buffer to store the returned compiled code
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR:Vertex Shader failed to compile\n" << infoLog << std::endl;
	}

	/*---------- fragment shader----------*/
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR:Fragment Shader failed to compile\n" << infoLog << std::endl;
	}

	/*----------Shader Program-----------*/
	//link the compiled shaders as a single object
	//output of one shader is the input of the next
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR:programr failed to link\n" << infoLog << std::endl;
	}
	// once linked remove
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(program);

	
	//create the VAO (Vertex Attribute Object)
	glGenVertexArrays(NUM_VAOS, VAOs);
	// bind vertex attribute objects
	glBindVertexArray(VAOs[0]);
	// copy the vertices arrayinto a buffer that OpenGL can use 
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	
	//Create the EBOs (Element Buffer Objects)
	glGenBuffers(NUM_EBOS, EBOs);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set the vertex attribite pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//params:
	//1: the vertex attribute to apply the configuration to
	//2: the size of the vertex attribute
	//3: the type of data 
	//4: whether the data should be normalized
	//5: the stride of the vertex buffer (3 floats = 12)
	//6: the offset of the start of the data in the vertex buffer
	
	glEnableVertexAttribArray(0);

	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{
		GLdouble timeValue = glfwGetTime();
		GLfloat greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
		GLint vertexColorLocation = glGetUniformLocation(program, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		//Input
		processInput(window);

		// rendering commands
		static const GLfloat bgd[] = { 1.f, 0.f, 0.f, 1.f };
		
		glClearBufferfv(GL_COLOR, 0, bgd); // clears the colour buffer writing the specified colour over the entire screen+

		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//draws multiple primatives, 1: primative type, 2: start index, 3: number of vertices to draw

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
}
