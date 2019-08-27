#pragma once

#include "GameObject.h"


class BallObject : public GameObject
{
public:
	// ball state
	GLfloat radius;
	GLboolean stuck; // if a ball is stuck on the platform (at the beginning)
	GLboolean sticky, passThrough;

	BallObject();
	BallObject(glm::vec2 pos, GLfloat rad, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 move(GLfloat dt, GLuint windowWidth);
	void reset(glm::vec2 position, glm::vec2 velocity);
};