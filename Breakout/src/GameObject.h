#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include "textures\Texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	// object state
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	GLfloat rotation;
	GLboolean isSolid;
	GLboolean destroyed;
	// render state
	Texture2D sprite;
	// ctors 
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.f), glm::vec2 velocity = glm::vec2(0.f, 0.f));
	// draw sprite
	virtual void draw(SpriteRenderer& renderer);
};