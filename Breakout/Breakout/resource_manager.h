#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <GL/glew.h>

#include "shader.h"
#include "texture.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;
	static Shader LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, const std::string& name);
	static Shader GetShader(const std::string& name);
	static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, const std::string& name);
	static Texture2D GetTexture(const std::string& name);
	static void Clear();
private:
	ResourceManager() {}
	static Shader loadShaderFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
	static Texture2D loadTextureFile(const GLchar* file, GLboolean alpha);
};

#endif // !RESOURCE_MANAGER_H
