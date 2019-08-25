#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// State
	GLuint ID;
	// Constructor
	Shader() { }
	// Sets the current shader as active
	Shader  &use();
	// geometry shader is optional
	void    compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);
	// Utility functions
	void    setFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
	void    setInteger(const GLchar *name, GLint value, GLboolean useShader = false);
	void    setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void    setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
	void    setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void    setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	void    setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void    setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
	void    setMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);

private:
	// Checks if compilation or linking failed and if so, print the error logs
	void    checkCompileErrors(GLuint object, std::string type);
};
