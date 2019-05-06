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
						"void main()\n"
						"{\n"
						"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
						"}";

bool bEBO = false;

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

	if (!bEBO)
	{
		// 使用 VBO
		float vertices[] = {
			/*-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f*/

		 // first triangle
		  -0.9f, -0.5f, 0.0f,  // left 
		  -0.0f, -0.5f, 0.0f,  // right
		  -0.45f, 0.5f, 0.0f,  // top 
		  // second triangle
		   0.0f, -0.5f, 0.0f,  // left
		   0.9f, -0.5f, 0.0f,  // right
		   0.45f, 0.5f, 0.0f   // top 
		};

		// 创建一个缓冲对象，用id保存缓冲对象的引用，实际对象存储在后台
		glGenBuffers(1, &VBO);
		// 创建的缓冲绑定到GL_ARRAY_BUFFER目标
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 将顶点数据复制到缓冲对象中
		/*
		*	GL_STATIC_DRAW ：数据不会或几乎不会改变。
		*	GL_DYNAMIC_DRAW：数据会被改变很多。
		*	GL_STREAM_DRAW ：数据每次绘制时都会改变
		*/
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 解释内存中的顶点数据,将顶点数据链接到顶点着色器的属性
		/*
			第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
			第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
			第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
			下个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
			第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子（译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
			最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。
		*/
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		// 启动顶点属性
		glEnableVertexAttribArray(0);
	}
	else
	{
		// 使用EBO
		float vertices[] = {
			-0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
		};
		// 创建一个缓冲对象，用id保存缓冲对象的引用，实际对象存储在后台
		glGenBuffers(1, &VBO);
		// 创建的缓冲绑定到GL_ARRAY_BUFFER目标
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 将顶点数据复制到缓冲对象中
		/*
		*	GL_STATIC_DRAW ：数据不会或几乎不会改变。
		*	GL_DYNAMIC_DRAW：数据会被改变很多。
		*	GL_STREAM_DRAW ：数据每次绘制时都会改变
		*/
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		unsigned int indices[]
		{
			0, 1, 2,
			2, 3, 1
		};
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}


	// 所有数据以及数据链接都保存到VAO的引用中了
	// 解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 解绑VAO
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		// 检测输入
		processInput(window);
		// 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲）

		// 渲染指令 我们要把所有的渲染(Rendering)操作放到渲染循环中，因为我们想让这些渲染指令在每次渲染循环迭代的时候都能被执行
		// glClearColor函数是一个状态设置函数，而glClear函数则是一个状态使用的函数，它使用了当前的状态来获取应该清除为的颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 激活程序对象
		glUseProgram(shaderProgram);
		// 在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象
		glBindVertexArray(VAO);
		/*
			线框模式(Wireframe Mode)
			要想用线框模式绘制你的三角形，你可以通过glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)函数配置OpenGL如何绘制图元。
			第一个参数表示我们打算将其应用到所有的三角形的正面和背面，
			第二个参数告诉我们用线来绘制。之后的绘制调用会一直以线框模式绘制三角形，
			直到我们用glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)将其设置回默认模式
		*/
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (!bEBO)
		{
			/*
				第一个参数是我们打算绘制的OpenGL图元的类型。由于我们在一开始时说过，我们希望绘制的是一个三角形，这里传递GL_TRIANGLES给它。
				第二个参数指定了顶点数组的起始索引，我们这里填0。
				最后一个参数指定我们打算绘制多少个顶点，这里是3（我们只从我们的数据中渲染一个三角形，它只有3个顶点长
			*/
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else
		{
			/*
				第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样。
				第二个参数是我们打算绘制顶点的个数，这里填6，也就是说我们一共需要绘制6个顶点。
				第三个参数是索引的类型，这里是GL_UNSIGNED_INT。最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候），但是我们会在这里填写0
			*/
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
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