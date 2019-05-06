// ʹ��uniform ȫ�ֱ���

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
	//// ��ɫ�������������Ե�����
	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ����һ����ɫ������
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// ��ɫ��Դ�븽�ӵ���ɫ������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// �������Ƿ�ɹ�
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

	// ���ӳɹ�֮������Ҫ�����Ƭ����ɫ������ɾ����
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//// ����������
	//glUseProgram(shaderProgram);
	//// ��glUseProgram��������֮��ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������


	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	// ��VAO
	glBindVertexArray(VAO);

	// ʹ��EBO
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


	// ���������Լ��������Ӷ����浽VAO����������
	// ���VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// ���VAO
	glBindVertexArray(0);

	// ��ȡ��ɫ����ɫ����vertexColorȫ�ֱ���
	int redColorLocation = glGetUniformLocation(shaderProgram, "redColor");
	int greenColorLocation = glGetUniformLocation(shaderProgram, "greenColor");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		// �޸�Ƭ����ɫ����ɫ��ȫ�ֱ���
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
