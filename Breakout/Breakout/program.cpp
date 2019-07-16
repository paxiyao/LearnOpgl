#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "resource_manager.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The width of the screen
const GLuint SCREEN_WIDTH = 800;
// The height of the screen
const GLuint SCREEN_HEIGHT = 600;

Game BreakOut(SCREEN_WIDTH, SCREEN_HEIGHT);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window create
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BreakOut", NULL, NULL);
	glfwMakeContextCurrent(window);

	// glew init 
	//glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	glfwSetKeyCallback(window, key_callback);

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize game
	BreakOut.Init();

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// Start Game within Menu State
	BreakOut.State = GAME_ACTIVE;

	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// deltaTime = 0.001f
		// manage user input
		BreakOut.ProcessInput(deltaTime);

		// update game state
		BreakOut.Update(deltaTime);

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		BreakOut.Render();

		glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using the resource manager
	ResourceManager::Clear();
	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key > 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			BreakOut.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			BreakOut.Keys[key] = GL_FALSE;
	}
}