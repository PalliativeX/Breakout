#include "ParticleGenerator.h"


ParticleGenerator::ParticleGenerator(Shader sh, Texture2D tex, GLuint am) : shader(sh), texture(tex), amount(am)
{
	this->init();
}


void ParticleGenerator::update(GLfloat dt, GameObject & object, GLuint newParticles, glm::vec2 offset)
{
	// add new particles
	for (GLuint i = 0; i < newParticles; i++) {
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}
	// update all particles
	for (GLuint i = 0; i < this->amount; i++) {
		Particle& p = this->particles[i];
		p.life -= dt; // reduce lifetime
		if (p.life > 0.f) {
			p.position -= p.velocity * dt;
			p.color.a -= dt * 2.5f;
		}
	}
}


// render all particles
void ParticleGenerator::draw()
{
	// use additive blending to give it a "glow" effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.use();
	for (Particle particle : this->particles) {
		if (particle.life > 0.f) {
			this->shader.setVector2f("offset", particle.position);
			this->shader.setVector4f("color", particle.color);
			this->texture.bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	// reset the default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void ParticleGenerator::init()
{
	// set up mesh and attrib properties
	GLuint VBO;
	GLfloat particleQuad[] = {
		0.f, 1.f, 0.f, 1.f,
		1.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 0.f,

		0.f, 1.f, 0.f, 1.f,
		1.f, 1.f, 1.f, 1.f,
		1.f, 0.f, 1.f, 0.f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// create this->amount particle instances
	for (GLuint i = 0; i < this->amount; i++)
		this->particles.push_back(Particle());
}


GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	// search from last used particle
	for (GLuint i = lastUsedParticle; i < this->amount; i++) {
		if (this->particles[i].life <= 0.f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// do linear search
	for (GLuint i = 0; i < lastUsedParticle; i++) {
		if (this->particles[i].life <= 0.f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// all particles are taken
	lastUsedParticle = 0;
	return 0;
}


// respawns a dead particle and gives it a new (slightly random) pos and velocity
void ParticleGenerator::respawnParticle(Particle & particle, GameObject & object, glm::vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.f);
	particle.position = object.position + random + offset;
	particle.color = glm::vec4(rColor, rColor, rColor, 1.f);
	particle.life = 1.f;
	particle.velocity = object.velocity * 0.1f;
}
