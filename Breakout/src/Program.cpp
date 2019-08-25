#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>

#include "Game.h"
#include "ResourceManager.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void errorCallback(int error, const char* description);

// screen height and width
const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

Game breakout(SCR_WIDTH, SCR_HEIGHT);

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Breakout", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetErrorCallback(errorCallback);

	// OpenGL configuration
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	breakout.init();

	// deltatime vars
	GLfloat deltaTime = 0.f;
	GLfloat lastFrame = 0.f;

	// start game within menu state
	breakout.state = GAME_ACTIVE;

	while (!glfwWindowShouldClose(window)) {
		// calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		// manage user input
		breakout.processInput(deltaTime);

		// update game state
		breakout.update(deltaTime);

		// render
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		breakout.render();

		glfwSwapBuffers(window);
	}

	// delete all resources loaded using the resource manager
	ResourceManager::clear();

	glfwTerminate();
	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			breakout.keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			breakout.keys[key] = GL_FALSE;
	}
}


void errorCallback(int error, const char* description)
{
	std::cerr << "GLFW error: " << description << std::endl;
}