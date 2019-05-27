#pragma once
#ifndef SHADER_H
#define SHADER_H

#define INFOLEN 512

#include "glad/glad.h"
#include "fstream"
#include "sstream"
#include "iostream"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// ����ID
	unsigned int ID;
	// ��������ȡ��������ɫ��
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// ������ɫ���������
	void getShaderiv(unsigned int shader, const char* name);
	// ������ɫ������������
	void getProgramiv(unsigned int program, const char* name);
	// ʹ�û򼤻����
	void use();
	// uniform ���ߺ���
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4& mat) const;
private:
	char infoLog[INFOLEN];
};

#endif // !SHADER_H
