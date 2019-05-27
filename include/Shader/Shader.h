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
	// 程序ID
	unsigned int ID;
	// 构造器读取并构建着色器
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// 检验着色器编译错误
	void getShaderiv(unsigned int shader, const char* name);
	// 检验着色器程序编译错误
	void getProgramiv(unsigned int program, const char* name);
	// 使用或激活程序
	void use();
	// uniform 工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4& mat) const;
private:
	char infoLog[INFOLEN];
};

#endif // !SHADER_H
