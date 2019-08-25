#include "SpriteRenderer.h"


SpriteRenderer::SpriteRenderer(Shader & shader)
{
	this->shader = shader;
	this->initRenderData();
}


SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}


void SpriteRenderer::initRenderData()
{
	// configure VAO & VBO
	GLuint VBO;
	GLfloat vertices[] = {
		// pos     // tex
		0.f, 1.f,  0.f, 1.f,
		1.f, 0.f,  1.f, 0.f,
		0.f, 0.f,  0.f, 0.f,

		0.f, 1.f,  0.f, 1.f,
		1.f, 1.f,  1.f, 1.f,
		1.f, 0.f,  1.f, 0.f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void SpriteRenderer::drawSprite(Texture2D & texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	// prepare transforms
	this->shader.use();
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
	model = glm::rotate(model, rotate, glm::vec3(0.f, 0.f, 1.f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));

	model = glm::scale(model, glm::vec3(size, 1.f));

	this->shader.setMatrix4("model", model);
	this->shader.setVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.bind();
		
	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

