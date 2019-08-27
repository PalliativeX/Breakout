#include "Game.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"

#include <algorithm>

SpriteRenderer* renderer;
GameObject* player;
BallObject* ball;
ParticleGenerator* particles;
PostProcessor* effects;
irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();


GLfloat shakeTime = 0.f;



Game::Game(GLuint w, GLuint h)
	: state(GAME_ACTIVE), keys(), width(w), height(h)
{ }


Game::~Game()
{
	delete renderer;
	delete player;
	delete ball;
	delete particles;
	delete effects;
	soundEngine->drop();
}


void Game::init()
{
	// load shaders
	ResourceManager::loadShader("C:/dev/Breakout/Breakout/src/shaders/sprite.vert", "C:/dev/Breakout/Breakout/src/shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::loadShader("C:/dev/Breakout/Breakout/src/shaders/particle.vert", "C:/dev/Breakout/Breakout/src/shaders/particle.frag", nullptr, "particle");
	ResourceManager::loadShader("C:/dev/Breakout/Breakout/src/shaders/postprocessing.vert", "C:/dev/Breakout/Breakout/src/shaders/postprocessing.frag", nullptr, "postprocessing");
	// configure shaders
	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::ortho(0.f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.f, -1.f, 1.f);
	ResourceManager::getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getShader("sprite").setMatrix4("projection", projection);
	ResourceManager::getShader("particle").use().setInteger("sprite", 0);
	ResourceManager::getShader("particle").setMatrix4("projection", projection);

	// set render-specific controls
	renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

	// load textures
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/background.jpg", GL_FALSE, "background");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/block.png", GL_FALSE, "block");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/blockSolid.png", GL_FALSE, "blockSolid");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/paddle.png", GL_TRUE, "paddle");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/ball.png", GL_TRUE, "ball");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/particle.png", GL_TRUE, "particle");
	// powerups
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/powerups/speedPowerup.png", GL_TRUE, "speedPowerup");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/powerups/chaosPowerup.png", GL_TRUE, "chaosPowerup");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/powerups/confusePowerup.png", GL_TRUE, "confusePowerup");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/powerups/passthroughPowerup.png", GL_TRUE, "passthroughPowerup");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/powerups/stickyPowerup.png", GL_TRUE, "stickyPowerup");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/powerups/increasePowerup.png", GL_TRUE, "increasePowerup");


	// load levels
	GameLevel one;   one.load("C:/dev/Breakout/Breakout/src/levels/one.lvl", this->width, this->height * 0.5);
	GameLevel two;   two.load("C:/dev/Breakout/Breakout/src/levels/two.lvl", this->width, this->height * 0.5);
	GameLevel three; three.load("C:/dev/Breakout/Breakout/src/levels/three.lvl", this->width, this->height * 0.5);
	GameLevel four;  four.load("C:/dev/Breakout/Breakout/src/levels/four.lvl", this->width, this->height * 0.5);
	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);
	this->currentLevel = 2;

	// initializing player
	glm::vec2 playerPos = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
	player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));

	// initializing ball
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("ball"));

	// creating a particle generator
	particles = new ParticleGenerator(ResourceManager::getShader("particle"), ResourceManager::getTexture("particle"), 500);

	effects = new PostProcessor(ResourceManager::getShader("postprocessing"), this->width, this->height);

	// turning on the soundtrack
	soundEngine->setSoundVolume(soundEngine->getSoundVolume() * 0.4);
	soundEngine->play2D("C:/dev/Breakout/Breakout/src/audio/breakout.mp3", GL_TRUE);
}


void Game::update(GLfloat dt)
{
	ball->move(dt, this->width);
	// check for collisions
	this->doCollisions();

	// update particles
	particles->update(dt, *ball, 2, glm::vec2(ball->radius / 2));

	// update powerups
	this->updatePowerUps(dt);

	// reduce shake time
	if (shakeTime > 0.f) {
		shakeTime -= dt;
		if (shakeTime <= 0.f)
			effects->shake = false;
	}

	// reset the game when ball hits the bottom
	if (ball->position.y >= this->height) {
		this->resetLevel();
		this->resetPlayer();
	}
}


void Game::processInput(GLfloat dt) // dt - deltaTime
{
	if (this->state == GAME_ACTIVE) {
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// move playerboard
		if (this->keys[GLFW_KEY_A]) {
			if (player->position.x >= 0) {
				player->position.x -= velocity;
				if (ball->stuck)
					ball->position.x -= velocity;
			}
		}
		if (this->keys[GLFW_KEY_D])
			if (player->position.x <= this->width - player->size.x) {
				player->position.x += velocity;
				if (ball->stuck)
					ball->position.x += velocity;
			}
	}
	if (this->keys[GLFW_KEY_SPACE])
		ball->stuck = false;
}


void Game::render()
{
	if (this->state == GAME_ACTIVE)
	{
		effects->beginRender();
			// draw background
			renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0, 0), glm::vec2(this->width, this->height), 0.f);
			// draw level
			this->levels[this->currentLevel].draw(*renderer);
			// draw powerups
			for (PowerUp& powerup : this->powerUps)
				if (!powerup.destroyed)
					powerup.draw(*renderer);
			// draw paddle
			player->draw(*renderer);
			// draw particles
			particles->draw();
			// draw ball
			ball->draw(*renderer);
		effects->endRender();
		// render postprocessing quad
		effects->render(glfwGetTime());
	}
}


Direction vectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.f, 1.f),  // up
		glm::vec2(1.f, 0.f),  // right
		glm::vec2(0.f, -1.f), // down
		glm::vec2(-1.f, 0.f)   // left
	};
	GLfloat max = 0.f;
	GLuint bestMatch = -1;
	for (GLuint i = 0; i < 4; i++) {
		GLfloat dotProduct = glm::dot(glm::normalize(target), compass[i]);
		if (dotProduct > max) {
			max = dotProduct;
			bestMatch = i;
		}
	}
	return (Direction)bestMatch;
}


GLboolean checkCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
	// Collision x-axis?
	bool collisionX = one.position.x + one.size.x >= two.position.x &&
		two.position.x + two.size.x >= one.position.x;
	// Collision y-axis?
	bool collisionY = one.position.y + one.size.y >= two.position.y &&
		two.position.y + two.size.y >= one.position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}


Collision checkCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
	// Get center point circle first 
	glm::vec2 center(one.position + one.radius);
	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.size.x / 2, two.size.y / 2);
	glm::vec2 aabb_center(two.position.x + aabb_half_extents.x, two.position.y + aabb_half_extents.y);
	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;

	if (glm::length(difference) < one.radius)
		return std::make_tuple(GL_TRUE, vectorDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}


void activatePowerUp(PowerUp& powerup);

// check for collisions between the ball and bricks
void Game::doCollisions()
{
	for (GameObject& box : this->levels[this->currentLevel].bricks) {
		if (!box.destroyed) {
			Collision collision = checkCollision(*ball, box);
			if (std::get<0>(collision)) { // if a collision occured
				if (!box.isSolid) {
					box.destroyed = GL_TRUE;
					this->spawnPowerUps(box);
					soundEngine->play2D("C:/dev/Breakout/Breakout/src/audio/bleep.mp3", GL_FALSE);
				}
				else {
					// if block is solid, enable shake effect
					shakeTime = 0.05f;
					effects->shake = true;
					soundEngine->play2D("C:/dev/Breakout/Breakout/src/audio/solid.wav", GL_FALSE);
				}
				// collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diffVec = std::get<2>(collision);
				if (!(ball->passThrough && !box.isSolid)) { // don't do collision resolution if passThrough is activated
					if (dir == LEFT || dir == RIGHT) {
						ball->velocity.x = -ball->velocity.x; // reverse horizontal velocity
						// relocate
						GLfloat penetration = ball->radius - std::abs(diffVec.x);
						if (dir == LEFT)
							ball->position.x += penetration;
						else
							ball->position.x -= penetration;
					}
					else { // vertical collision
						ball->velocity.y = -ball->velocity.y;
						// relocate
						GLfloat penetration = ball->radius - std::abs(diffVec.y);
						if (dir == UP)
							ball->position.y -= penetration;
						else
							ball->position.y += penetration;
					}
				}
			}
		}

		for (PowerUp& powerup : this->powerUps) {
			if (!powerup.destroyed) {
				if (powerup.position.y >= this->height)
					powerup.destroyed = GL_TRUE;
				if (checkCollision(*player, powerup)) {
					// activate a powerup if collided with the player
					activatePowerUp(powerup);
					powerup.destroyed = GL_TRUE;
					powerup.activated = GL_TRUE;
				}
			}
		}
	}

	Collision result = checkCollision(*ball, *player);
	if (!ball->stuck && std::get<0>(result)) {
		// check where it hit the board and change the velocity accordingly
		GLfloat boardCenter = player->position.x + player->size.x / 2;
		GLfloat distance = (ball->position.x + ball->radius) - boardCenter;
		GLfloat percentage = distance / (player->size.x / 2);
		// then move accordingly
		GLfloat strength = 2.f;
		glm::vec2 oldVelocity = ball->velocity;
		ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		ball->velocity.y = -1 * abs(ball->velocity.y);
		ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);

		// if sticky powerup is activated, stick the ball to paddle
		ball->stuck = ball->sticky;

		soundEngine->play2D("C:/dev/Breakout/Breakout/src/audio/bleep.wav", GL_FALSE);
	}
}

void Game::resetLevel()
{
	switch (currentLevel) {
	case 0:
		this->levels[0].load("C:/dev/Breakout/Breakout/src/levels/one.lvl", this->width, this->height * 0.5);
		break;
	case 1:
		this->levels[1].load("C:/dev/Breakout/Breakout/src/levels/two.lvl", this->width, this->height * 0.5);
		break;
	case 2:
		this->levels[2].load("C:/dev/Breakout/Breakout/src/levels/three.lvl", this->width, this->height * 0.5);
		break;
	case 3:
		this->levels[3].load("C:/dev/Breakout/Breakout/src/levels/four.lvl", this->width, this->height * 0.5);
		break;
	}
}

void Game::resetPlayer()
{
	// Reset player/ball stats
	player->size = PLAYER_SIZE;
	player->position = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
	ball->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
	// Also disable all active powerups
	effects->chaos = effects->confuse = GL_FALSE;
	ball->passThrough = ball->sticky = GL_FALSE;
	player->color = glm::vec3(1.0f);
	ball->color = glm::vec3(1.0f);
}


GLboolean shouldSpawn(GLuint chance)
{
	GLuint random = rand() % chance;
	return random == 0;
}


void Game::spawnPowerUps(GameObject & block)
{
	if (shouldSpawn(75)) // 1 in 75 chance
		this->powerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.f), 0.f, block.position, ResourceManager::getTexture("speedPowerup")));
	if (shouldSpawn(75))
		this->powerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.position, ResourceManager::getTexture("stickyPowerup")));
	if (shouldSpawn(75))
		this->powerUps.push_back(PowerUp("passThrough", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.position, ResourceManager::getTexture("passthroughPowerup")));
	if (shouldSpawn(75))
		this->powerUps.push_back(PowerUp("padSizeIncrease", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.position, ResourceManager::getTexture("increasePowerup")));
	if (shouldSpawn(15)) // Negative powerups should spawn more often
		this->powerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.position, ResourceManager::getTexture("confusePowerup")));
	if (shouldSpawn(15))
		this->powerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.position, ResourceManager::getTexture("chaosPowerup")));
}


GLboolean isOtherPowerUpActive(std::vector<PowerUp>& powerups, std::string type) 
{
	for (const PowerUp& powerup : powerups) {
		if (powerup.activated)
			if (powerup.type == type)
				return GL_TRUE;
	}
	return GL_FALSE;
}


// make sure powerups move and are deactivated as soon as their time runs out
void Game::updatePowerUps(GLfloat dt)
{
	for (PowerUp& powerup : this->powerUps) {
		powerup.position += powerup.velocity * dt;
		if (powerup.activated) {
			powerup.duration -= dt;

			if (powerup.duration <= 0.f) {
				//deactivate effects
				powerup.activated = GL_FALSE;

				if (powerup.type == "sticky") {
					if (!isOtherPowerUpActive(this->powerUps, "sticky")) { // only reset if there are no other sticky powerups active
						ball->sticky = GL_FALSE;
						player->color = glm::vec3(1.f);
					}
				}
				else if (powerup.type == "passThrough") {
					if (!isOtherPowerUpActive(this->powerUps, "passThrough")) {
						ball->passThrough = GL_FALSE;
						ball->color = glm::vec3(1.f);
					}
				}
				else if (powerup.type == "confuse") {
					if (!isOtherPowerUpActive(this->powerUps, "confuse")) {
						effects->confuse = GL_FALSE;
					}
				}
				else if (powerup.type == "chaos") {
					if (!isOtherPowerUpActive(this->powerUps, "chaos")) {
						effects->chaos = GL_FALSE;
					}
				}
			}
		}
	}
	this->powerUps.erase(std::remove_if(this->powerUps.begin(), this->powerUps.end(), [](const PowerUp& powerup) {return powerup.destroyed && !powerup.activated; }
	), this->powerUps.end());
}


void activatePowerUp(PowerUp& powerup)
{
	if (powerup.type == "speed") {
		ball->velocity *= 1.2f;
	}
	else if (powerup.type == "sticky") {
		ball->sticky = GL_TRUE;
		player->color = glm::vec3(1.f, 0.5f, 1.f);
	}
	else if (powerup.type == "passThrough") {
		ball->passThrough = GL_TRUE;
		ball->color = glm::vec3(1.f, 0.5f, 0.5f);
	}
	else if (powerup.type == "padSizeIncrease") {
		player->size.x += 50;
	}
	else if (powerup.type == "confuse") {
		if (!effects->chaos)
			effects->confuse = GL_TRUE;
	}
	else if (powerup.type == "chaos") {
		if (!effects->confuse)
			effects->chaos = GL_TRUE;
	}
}

