// include GLAD before GLFW
// include file for GLAD includes required OpenGL headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// intantiate the GLFW window
int main() {
	// initialize the GLFW library
	glfwInit();
	// setting the OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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

	// a simple render loop 
	// check at the start of each loop if GLFW has been instracted to close
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// ----------- rendering commands go here -------------//

		// clear the screen's color buffer 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// the buffer is cleared to the color as specified by glClearColor
		glClear(GL_COLOR_BUFFER_BIT);

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