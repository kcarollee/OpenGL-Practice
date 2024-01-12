// include GLAD before GLFW
// include file for GLAD includes required OpenGL headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// function prototypes

// register a callback function on then window that get called when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// input control
void processInput(GLFWwindow* window);

// intantiate the GLFW window
int main() {
	// initialize the GLFW library
	glfwInit();
	// setting the OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// tell GLFW to explicitly use core profile -> get access to a smaller subset op OpenGL features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// creating a window object
	// the window object holds all the windowing data
	GLFWwindow* window = glfwCreateWindow(800, 600, "LEARNING OPENGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "FALIED TO CREATE WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}
	// tell GLFW to make the context of our window the main context on the current thread
	glfwMakeContextCurrent(window);

	// GLAD manages function pointers for OpenGL
	// initialize GLAD before calling any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
		return -1;
	}

	// telling OpenGL the size of the rendering window
	// first 2 params: location of the lower left corner of the window
	// last 2 params: w and h of the rendering window in pixels -> set equl to GLFW's window size in this case
	glViewport(0, 0, 800, 600);

	// register a callback function on the window that gets called whenever the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// vertices for our triangle
	float vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f // top left

	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	// vertex array object: 
	//	think of VBO as an array of RAW DATA and VAO as an array of ATTRIBUTES
	//	VAO allows to attach an entire oarray of attributes by just the VAO's id
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// element buffer object
	// instead of drawing something with overlapping vertices, we use the indices to specify
	// the order in which the vertices to be drawn . an EBO is a buffer that stores indices 
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// vertex buffer object -> has a unique ID corresponding to the buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// buffer type of a vertex buffer: GL_ARRAY_BUFFER
	// bind the newly created buffer to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copy the previously defined vert data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// set the vertex attribute pointers
	// par: which vert attrib to configure (layout (location = 0)),
	//		size of the vertex attribute (vec3 -> composed of 3 values)
	//		specifies the type of data
	//		specifies if we want the data to be normalized
	//		stride: the space between consecutive vertex attributes. 
	//			the next position data is located 3 times the size of a float away 
	//		the offset of where the position data begins in the buffer. type void*

	// which VBO a vertex attribute takes its data from: the VBO currently bound to GL_ARRAY_BUFFER 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	const char* vertexShaderSource = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// creating the shader object, referenced by an ID
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// attach the shader source code to the shader obj and compile the shader
	// par: shd obj to compile to, how many strings we're passing, shader source code, null
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// checking for compile-time errors 
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		infoLog[0] = '\0';
	}
	else std::cout << "VERTEX SHADER COMPILED SUCCESSFULLY" << std::endl;

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main(){\n"
		" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// creating a SHADER PROGRAM OBJECT which links multiple shaders
	// the shader program has to be activated when rendering objects

	unsigned int shaderProgram;
	// creates a program and returns the ID ref to the newly created program obj.
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// every shader and rendering call after glUseProgram will now use this program obj
	// glUseProgram(shaderProgram);

	// delete the shader objects after linking them into the program object. they are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// check if linking the shader program failed 
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER LINKING FAILED\n" << infoLog << std::endl;
	}
	else std::cout << "SHADER LINK SUCCESSFUL" << std::endl;

	// a simple render loop 
	// check at the start of each loop if GLFW has been instructed to close
	while (!glfwWindowShouldClose(window)) {
		// check for input on every frame of the render loop
		processInput(window);

		// ----------- rendering commands go here -------------//

		// clear the screen's color buffer 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// the buffer is cleared to the color as specified by glClearColor
		// args passed: specity which buffer we want to clear
		// possible bits: GL_COLORBUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// par: primitive type, starting index, how many verts to draw
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		// indicate to render triangles from an index buffer
		// par: primitive type, number of elements, type of indices, offset
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// ----------------------------------------------------//

		// checks if any events are triggered (mouse or keyboard events for example)
		glfwPollEvents();
		// swap the color buffer, a large 2D buffer that contains the color values in GLFW's window
		glfwSwapBuffers(window);
	}

	// clean / delete allocated GLFW resources upon exiting the render loop
	glfwTerminate();
	return 0;
	
}

// params: GLFWwindow, new window dimensions
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	// check whether the user has pressed the escape key
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}