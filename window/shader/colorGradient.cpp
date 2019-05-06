// 使用uniform 全局变量

#define __CUSTOM__TEST__ false
#if true == __CUSTOM__TEST__

#include<iostream>
#include "glad.c"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow*, int height, int w);
void processInput(GLFWwindow *window);
void getShaderiv(unsigned int shader, const char* name);
void getProgramiv(unsigned int program, const char* name);

const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform float redColor;\n"
"uniform float greenColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(redColor, greenColor, 0.0f, 1.0f);\n"
"}";

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
	//// 着色器声明顶点属性的上限
	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 创建一个着色器对象
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// 着色器源码附加到着色器对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// 检测编译是否成功
	getShaderiv(vertexShader, "vertexShader");

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	getShaderiv(fragmentShader, "fragmentShader");

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	getProgramiv(shaderProgram, "shaderProgram");

	// 链接成功之后不再需要顶点和片段着色器可以删除了
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//// 激活程序对象
	//glUseProgram(shaderProgram);
	//// 在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象


	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	// 绑定VAO
	glBindVertexArray(VAO);

	// 使用EBO
	float vertices[] = {
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
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


	// 所有数据以及数据链接都保存到VAO的引用中了
	// 解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 解绑VAO
	glBindVertexArray(0);

	// 获取着色器颜色属性vertexColor全局变量
	int redColorLocation = glGetUniformLocation(shaderProgram, "redColor");
	int greenColorLocation = glGetUniformLocation(shaderProgram, "greenColor");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		// 修改片段着色器颜色的全局变量
		float timeValue = glfwGetTime();
		float redValue = (sin(timeValue) / 2.0f) + 0.5f;
		glUniform1f(redColorLocation, redValue);
		glUniform1f(greenColorLocation, sin(timeValue));

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

void getShaderiv(unsigned int shader, const char* name)
{
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << name << ":" << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void getProgramiv(unsigned int program, const char* name)
{
	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << name << ":" << "ERROR::PROGRAM::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

#endif // true == __CUSTOM__TEST__
