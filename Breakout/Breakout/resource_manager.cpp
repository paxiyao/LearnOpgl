#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL.h>

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, const std::string& name)
{
	Shaders[name] = loadShaderFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(const std::string& name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar* file, GLboolean alpha, const std::string& name)
{
	Textures[name] = loadTextureFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(const std::string& name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	// (Porperly) delete all shader
	for (auto iter: Shaders)
		glDeleteProgram(iter.second.ID);
	// (Porperly) delete all texture
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream;
		std::stringstream fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (nullptr != gShaderFile)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* gShaderCode = geometryCode.c_str();

	// 2. Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}


Texture2D ResourceManager::loadTextureFile(const GLchar* file, GLboolean alpha)
{
	// Create Texture object
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Now generate texture
	texture.Generate(width, height, image);
	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}