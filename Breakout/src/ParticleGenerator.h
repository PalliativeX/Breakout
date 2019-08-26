#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include <vector>

#include "shaders\Shader.h"
#include "textures\Texture.h"
#include "GameObject.h"

struct Particle
{
	glm::vec2 position, velocity;
	glm::vec4 color;
	GLfloat life;

	Particle() : position(0.f), velocity(0.f), color(1.f), life(0.f) {}
};


class ParticleGenerator
{
public:
	ParticleGenerator(Shader sh, Texture2D tex, GLuint am);
	// update all particles
	void update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.f, 0.f));
	// render particles
	void draw();

private:
	// state
	std::vector<Particle> particles;
	GLuint amount;
	// render state
	Shader shader;
	Texture2D texture;
	GLuint VAO;
	// initializes buffer and vertex attribs
	void init();
	// returns the 1st particle index that's currently unused
	GLuint firstUnusedParticle();
	// respawns particle
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.f, 0.f));
};