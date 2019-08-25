#pragma once

#include <vector>
#include "C:/dev/Breakout/Breakout/src/GameObject.h"
#include "C:/dev/Breakout/Breakout/src/ResourceManager.h"

class GameLevel
{
public:
	std::vector<GameObject> bricks;

	GameLevel();
	// loads level from file
	void load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
	// render level
	void draw(SpriteRenderer& renderer);
	// check if the level is completed
	GLboolean isCompleted();
	
private:
	// initialize level from tile data
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};