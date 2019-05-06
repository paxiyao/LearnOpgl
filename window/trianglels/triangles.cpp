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

	if (!bEBO)
	{
		// ʹ�� VBO
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

		// ����һ�����������id���滺���������ã�ʵ�ʶ���洢�ں�̨
		glGenBuffers(1, &VBO);
		// �����Ļ���󶨵�GL_ARRAY_BUFFERĿ��
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// ���������ݸ��Ƶ����������
		/*
		*	GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
		*	GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
		*	GL_STREAM_DRAW ������ÿ�λ���ʱ����ı�
		*/
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// �����ڴ��еĶ�������,�������������ӵ�������ɫ��������
		/*
			��һ������ָ������Ҫ���õĶ������ԡ����ǵ������ڶ�����ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)�������԰Ѷ������Ե�λ��ֵ����Ϊ0����Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
			�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
			����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
			�¸��������������Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
			�����������������(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��Ҫע�������������֪����������ǽ������еģ���������������֮��û�п�϶������Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���һ�������и���Ķ������ԣ����Ǿͱ����С�ĵض���ÿ����������֮��ļ���������ں���ῴ����������ӣ���ע: �����������˼��˵���Ǵ�������Եڶ��γ��ֵĵط�����������0λ��֮���ж����ֽڣ���
			���һ��������������void*��������Ҫ���ǽ��������ֵ�ǿ������ת��������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0�����ǻ��ں�����ϸ�������������
		*/
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		// ������������
		glEnableVertexAttribArray(0);
	}
	else
	{
		// ʹ��EBO
		float vertices[] = {
			-0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
		};
		// ����һ�����������id���滺���������ã�ʵ�ʶ���洢�ں�̨
		glGenBuffers(1, &VBO);
		// �����Ļ���󶨵�GL_ARRAY_BUFFERĿ��
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// ���������ݸ��Ƶ����������
		/*
		*	GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
		*	GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
		*	GL_STREAM_DRAW ������ÿ�λ���ʱ����ı�
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


	// ���������Լ��������Ӷ����浽VAO����������
	// ���VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// ���VAO
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		// �������
		processInput(window);
		// ������ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩

		// ��Ⱦָ�� ����Ҫ�����е���Ⱦ(Rendering)�����ŵ���Ⱦѭ���У���Ϊ����������Щ��Ⱦָ����ÿ����Ⱦѭ��������ʱ���ܱ�ִ��
		// glClearColor������һ��״̬���ú�������glClear��������һ��״̬ʹ�õĺ�������ʹ���˵�ǰ��״̬����ȡӦ�����Ϊ����ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ����������
		glUseProgram(shaderProgram);
		// ��glUseProgram��������֮��ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������
		glBindVertexArray(VAO);
		/*
			�߿�ģʽ(Wireframe Mode)
			Ҫ�����߿�ģʽ������������Σ������ͨ��glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)��������OpenGL��λ���ͼԪ��
			��һ��������ʾ���Ǵ��㽫��Ӧ�õ����е������ε�����ͱ��棬
			�ڶ������������������������ơ�֮��Ļ��Ƶ��û�һֱ���߿�ģʽ���������Σ�
			ֱ��������glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)�������û�Ĭ��ģʽ
		*/
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (!bEBO)
		{
			/*
				��һ�����������Ǵ�����Ƶ�OpenGLͼԪ�����͡�����������һ��ʼʱ˵��������ϣ�����Ƶ���һ�������Σ����ﴫ��GL_TRIANGLES������
				�ڶ�������ָ���˶����������ʼ����������������0��
				���һ������ָ�����Ǵ�����ƶ��ٸ����㣬������3������ֻ�����ǵ���������Ⱦһ�������Σ���ֻ��3�����㳤
			*/
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else
		{
			/*
				��һ������ָ�������ǻ��Ƶ�ģʽ�������glDrawArrays��һ����
				�ڶ������������Ǵ�����ƶ���ĸ�����������6��Ҳ����˵����һ����Ҫ����6�����㡣
				���������������������ͣ�������GL_UNSIGNED_INT�����һ�����������ǿ���ָ��EBO�е�ƫ���������ߴ���һ���������飬�������ǵ��㲻��ʹ��������������ʱ�򣩣��������ǻ���������д0
			*/
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// �����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����
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