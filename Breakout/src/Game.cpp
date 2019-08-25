#include "Game.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "BallObject.h"

SpriteRenderer* renderer;

Game::Game(GLuint w, GLuint h)
	: state(GAME_ACTIVE), keys(), width(w), height(h)
{ }


Game::~Game()
{

}


// player paddle
// initial speed and size
const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat   PLAYER_VELOCITY(500.f);
GameObject* player;

// ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.f, -350.f);
const GLfloat BALL_RADIUS = 12.5f;
BallObject* ball;

void Game::init()
{
	// load shaders
	ResourceManager::loadShader("C:/dev/Breakout/Breakout/src/shaders/sprite.vert", "C:/dev/Breakout/Breakout/src/shaders/sprite.frag", nullptr, "sprite");
	// configure shaders
	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::ortho(0.f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.f, -1.f, 1.f);
	ResourceManager::getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getShader("sprite").setMatrix4("projection", projection);
	// set render-specific controls
	renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

	// load textures
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/background.jpg", GL_FALSE, "background");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/block.png", GL_FALSE, "block");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/blockSolid.png", GL_FALSE, "blockSolid");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/paddle.png", GL_TRUE, "paddle");
	ResourceManager::loadTexture("C:/dev/Breakout/Breakout/src/textures/ball.png", GL_TRUE, "ball");

	// load levels
	GameLevel one;   one.load  ("C:/dev/Breakout/Breakout/src/levels/one.lvl",   this->width, this->height * 0.5);
	GameLevel two;   two.load  ("C:/dev/Breakout/Breakout/src/levels/two.lvl",   this->width, this->height * 0.5);
	GameLevel three; three.load("C:/dev/Breakout/Breakout/src/levels/three.lvl", this->width, this->height * 0.5);
	GameLevel four;  four.load ("C:/dev/Breakout/Breakout/src/levels/four.lvl",  this->width, this->height * 0.5);
	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);
	this->currentLevel = 1;

	// initializing player
	glm::vec2 playerPos = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
	player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));

	// initializing ball
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("ball"));
}


void Game::update(GLfloat dt)
{
	ball->move(dt, this->width);
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
		// draw background
		renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0, 0), glm::vec2(this->width, this->height), 0.f);
		// draw level
		this->levels[this->currentLevel].draw(*renderer);

		player->draw(*renderer);
		ball->draw(*renderer);
	}
}