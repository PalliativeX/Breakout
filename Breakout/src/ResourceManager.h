#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "textures/Texture.h"
#include "shaders/Shader.h"

// a static singleton class that hosts functions to load Textures and Shaders
class ResourceManager
{
public:
	// resource storage
	static std::map<std::string, Shader>    shaders;
	static std::map<std::string, Texture2D> textures;
	// loads a shader program
	static Shader loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
	static Shader &getShader(std::string name);
	static Texture2D loadTexture(const GLchar* file, GLboolean alpha, std::string name);
	static Texture2D &getTexture(std::string name);
	// deallocates all loaded resources
	static void clear();

private:
	ResourceManager();
	static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};