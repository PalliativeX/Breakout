#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include "textures/Texture.h"
#include "shaders\Shader.h"
#include "SpriteRenderer.h"

class PostProcessor
{
public:
	// state
	Shader postProcessingShader;
	Texture2D texture;
	GLuint width, height;
	// options
	GLboolean confuse, chaos, shake;
	
	PostProcessor(Shader sh, GLuint w, GLuint h);
	// prepares postprocessor's framebuffer operations
	void beginRender();
	// called after rendering the game so it stores all rendered data into a texture object
	void endRender();
	// renders postprocessed texture quad
	void render(GLfloat time);

private:
	// render state
	GLuint MSFBO, FBO; // Multisampled FBO and a regular FBO
	GLuint RBO;
	GLuint VAO;
	// initialize quad
	void initRenderData();
};