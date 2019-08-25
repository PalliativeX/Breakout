#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include "levels/GameLevel.h"

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	// Game state
	GameState  state;
	GLboolean  keys[1024];
	GLuint	   width, height;
	std::vector<GameLevel> levels;
	GLuint currentLevel;
	// Constructor/Destructor
	Game(GLuint w, GLuint h);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void init();
	// GameLoop
	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();
};