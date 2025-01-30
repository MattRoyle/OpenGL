
#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


GLfloat vertices[] =
{
	//pos			
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

//Shaders are dynamically compiled at run time

//process the input vertices into triangles
const char* vertexShaderSource =
"#version 450 core\n"//the version of openGL (4.5)
"layout(location = 0) in vec4 vaPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vaPos;\n"
"}\n";//GLSL (OpenGL Shading Language)

// process the pixel colour of the triangles
const char* fragmentShaderSource =
"#version 450 core\n"
"layout(location = 0) out vec4 fColour;\n"
"void main()\n"
"{\n"
"	fColour = vec4(1.0f, 0.5f, 0.f, 1.0f);\n"
"}\n";


#define NUM_BUFFERS 1
#define NUM_VAOS 1
GLuint Buffers[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
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

	/*----------Vertex shader----------*/
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);// call gl to create a shader returning the ID of the vertex 
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attatch the shader passing in the program as a string, the 4th param is optional a buffer to store the returned compiled code
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR:Vertex Shader failed to compile\n" << infoLog << std::endl;
	}

	/*---------- fragment shader----------*/
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
	unsigned int program = glCreateProgram();
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

	
	glGenVertexArrays(NUM_VAOS, VAOs);
	// bind vertex attribute objects
	glBindVertexArray(VAOs[0]);
	// copy the vertices arrayinto a buffer that OpenGL can use 
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

	// set the vertex attribite pointers
	/*
	params:
	1: the vertex attribute to apply the configuration to
	2: the size of the vertex attribute
	3: the type of data 
	4: whether the data should be normalized
	5: the stride of the vertex buffer (3 floats = 12)
	6: the offset of the start of the data in the vertex buffer
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0);

	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{
		//Input
		processInput(window);

		// rendering commands
		static const GLfloat bgd[] = { 1.f, 0.f, 0.f, 1.f };
		
		glClearBufferfv(GL_COLOR, 0, bgd); // clears the colour buffer writing the specified colour over the entire screen+

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);//draws primative, 1: primative type, 2: start index, 3: number of vertices to draw

		//swapping buffers and polling events
		glfwSwapBuffers(window);// swap the new colour buffer
		glfwPollEvents(); //checks for inputs (mouse, keyboard)
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}

/*GLFW change window size*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/*Process inputs by querying the appropiate keys*/
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)// exit when esc is pressed
		glfwSetWindowShouldClose(window, true);
}
