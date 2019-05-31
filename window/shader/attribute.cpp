// ������ɫ����ɫ������ͬ

#define __CUSTOM__TEST__ true
#if true == __CUSTOM__TEST__

#include<iostream>
#include "glad.c"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <learnopengl/shader.h>

void framebuffer_size_callback(GLFWwindow*, int height, int w);
void processInput(GLFWwindow *window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpgl", NULL, NULL);
	if (NULL == window)
	{
		std::cout << "Create window is fail!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed initialize GLAD" << std::endl;
		return -1;
	}
	//// ��ɫ�������������Ե�����
	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ����һ����ɫ������s
	Shader shader("3.6.vertex.glsl", "3.6.fragment.glsl");

	//// ����������
	//glUseProgram(shaderProgram);
	//// ��glUseProgram��������֮��ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������


	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	// ��VAO
	glBindVertexArray(VAO);

	// ʹ��EBO
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		-0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int indices[]
	{
		0, 1, 2,
		//2, 3, 1
	};
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// ���������Լ��������Ӷ����浽VAO����������
	// ���VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// ���VAO
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow*, int height, int width)
{
	glViewport(0, 0, height, width);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

#endif // true == __CUSTOM__TEST__
