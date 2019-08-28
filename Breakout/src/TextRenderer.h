#pragma once

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "textures/Texture.h"
#include "shaders/Shader.h"

struct Character
{
	GLuint textureID;
	glm::ivec2 size; // size of glyph
	glm::ivec2 bearing; // offset from baseline to left/top of glyph
	GLuint advance; // horizontal offset to advance to next glyph
};


class TextRenderer
{
public:
	std::map<GLchar, Character> characters;
	Shader textShader;

	TextRenderer(GLuint width, GLuint height);

	void load(std::string font, GLuint fontSize);
	void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.f));

private:
	// render state
	GLuint VAO, VBO;
};