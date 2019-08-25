#include "BallObject.h"

BallObject::BallObject() : GameObject(), radius(12.5f), stuck(true)
{ }

BallObject::BallObject(glm::vec2 pos, GLfloat rad, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, glm::vec2(rad * 2, rad * 2), sprite, glm::vec3(1.f), velocity), radius(rad), stuck(true)
{ }

glm::vec2 BallObject::move(GLfloat dt, GLuint windowWidth)
{
	// if not stuck to player board
	if (!this->stuck) {
		// move the ball
		this->position += this->velocity * dt;
		if (this->position.x <= 0.f) {
			this->velocity.x = -this->velocity.x;
			this->position.x = 0.f;
		}
		else if (this->position.x + this->size.x >= windowWidth) {
			this->velocity.x = -this->velocity.x;
			this->position.x = windowWidth - this->size.x;
		}
		if (this->position.y <= 0.f) {
			this->velocity.y = -this->velocity.y;
			this->position.y = 0.f;
		}
	}
	return this->position;
}

// resets the ball to initial stuck pos
void BallObject::reset(glm::vec2 position, glm::vec2 velocity)
{
	this->position = position;
	this->velocity = velocity;
	this->stuck = true;
}
