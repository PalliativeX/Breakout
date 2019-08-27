#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <irrKlang.h>

#include <tuple>
#include <vector>

#include "levels/GameLevel.h"
#include "PowerUp.h"

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction
{
	UP, 
	RIGHT,
	DOWN,
	LEFT
};

// player paddle
// initial speed and size
const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat   PLAYER_VELOCITY(500.f);

// ball initial params
const glm::vec2 INITIAL_BALL_VELOCITY(100.f, -350.f);
const GLfloat BALL_RADIUS = 12.5f;

// defines a collision typedef that represents collision data
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;


class Game
{
public:
	// Game state
	GameState  state;
	GLboolean  keys[1024];
	GLuint	   width, height;
	std::vector<GameLevel> levels;
	GLuint currentLevel;
	std::vector<PowerUp> powerUps;
	// Constructor/Destructor
	Game(GLuint w, GLuint h);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void init();
	// GameLoop
	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();
	// collisions
	void doCollisions();
	// reset
	void resetLevel();
	void resetPlayer();
	// powerups
	void spawnPowerUps(GameObject& block);
	void updatePowerUps(GLfloat dt);
};
