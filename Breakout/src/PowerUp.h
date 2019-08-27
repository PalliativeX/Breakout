#pragma once

#include "GameObject.h"

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.f, 150.f);

class PowerUp : public GameObject
{
public:
	// PowerUp state
	std::string type;
	GLfloat     duration;
	GLboolean   activated;

	PowerUp(std::string t, glm::vec3 col, GLfloat dur, glm::vec2 pos, Texture2D tex) :
		GameObject(pos, SIZE, tex, col, VELOCITY), type(t), duration(dur), activated() {}
};