// include GLAD before GLFW
// include file for GLAD includes required OpenGL headers
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader_s.h"
#include "stb_image.h"

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

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "MAX NUMBER OF VERT ATTRIBS: " << nrAttributes << std::endl;

	// vertices for our rectangle
	float vertices[] = {
		// positions      // colors        // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
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
	//			the next position data is located 6 times the size of a float away 
	//		the offset of where the position data begins in the buffer. type void*

	// which VBO a vertex attribute takes its data from: the VBO currently bound to GL_ARRAY_BUFFER 
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // enable the vertex attribute; par: the vertex attribute location
	// color attribute -> have to set the offset to (3 * sizeof(float))
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// generating a texture
	unsigned int texture1, texture2;
	// pars: num of textures to generate, store them in a unsigned int array
	glGenTextures(1, &texture1);
	// bind it so subsequent texture commands configure the currently bound texture
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// loading an image
	int width, height, nrChannels;
	// tell stbi to flip loaded textures on the y axis
	stbi_set_flip_vertically_on_load(true);
	// stbi_load: location. width, height, number of color channels
	unsigned char* data = stbi_load("../../4. textures/firstOpenGL/pic1.png", &width, &height, &nrChannels, 0);

	if (data) {
		// generate a texture using previously loaded image data
		/*
		pars:
			texture target
			mipmaplevel for which to create a texture for
			what format to store the texture
			width
			height
			0 (legacy stuff)
			format of the source image
			data type of the source image
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, data);
		// automatically generate all the required mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);
		// free the image memory
		
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}
	stbi_image_free(data);

	// generate 2nd texture
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// wrapping parms
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// filter parms
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// loading 2nd image
	data = stbi_load("../../4. textures/firstOpenGL/pic2.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}
	stbi_image_free(data);

	Shader testShader("../../4. textures/firstOpenGL/shaders/shader.vert", "../../4. textures/firstOpenGL/shaders/shader.frag");

	// set which texture unit each shader sampler belongs to by setting each sampler
	// only have to be set once
	testShader.use(); // DO NOT FORGET TO ACTIVATE/USE THE SHDAER BEFORE SETTING UNIFORMS
	glUniform1i(glGetUniformLocation(testShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(testShader.ID, "texture2"), 1);

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

		
		
		//glUseProgram(shaderProgram);



		// activate texture unit GL_TEXTURE0 to GL_TEXTURE15 avaialbe
		glActiveTexture(GL_TEXTURE0);
		// automatically assign the texture to eh frag shader's sampler
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		testShader.use();
		// setting the uniform 
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		testShader.setFloat("lerpVal", greenValue);
		//std::cout << greenValue << std::endl;

		

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