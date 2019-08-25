#include "GameLevel.h"

#include <fstream>
#include <sstream>

GameLevel::GameLevel()
{
}

// loads all numbers in a 2D vector
void GameLevel::load(const GLchar * file, GLuint levelWidth, GLuint levelHeight)
{
	// clear old data
	this->bricks.clear();
	// load from file
	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;
	if (fstream) {
		while (std::getline(fstream, line)) { // read each line from level file
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode) // read each word separated by spaces
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
}

// iterates through each loaded number and adds a GameObject to bricks vector
void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	// calculate dimensions
	GLuint height = tileData.size();
	GLuint width  = tileData[0].size();
	GLfloat unitWidth = levelWidth / static_cast<GLfloat>(width);
	GLfloat unitHeight = levelHeight / height;
	// initialize level tiles based on tileData
	for (GLuint y = 0; y < height; y++) {
		for (GLuint x = 0; x < width; x++) {
			// check block type from level data
			if (tileData[y][x] == 1) { // solid
				glm::vec2 pos(unitWidth * x, unitHeight * y);
				glm::vec2 size(unitWidth, unitHeight);
				GameObject obj(pos, size, ResourceManager::getTexture("blockSolid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.isSolid = GL_TRUE;
				this->bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) {
				glm::vec3 color = glm::vec3(1.f);
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.f, 0.7f, 0.f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.f, .5f, 0.0f);

				glm::vec2 pos(unitWidth * x, unitHeight * y);
				glm::vec2 size(unitWidth, unitHeight);
				this->bricks.push_back(GameObject(pos, size, ResourceManager::getTexture("block"), color));
			}
		}
	}
}


void GameLevel::draw(SpriteRenderer& renderer)
{
	for (GameObject& tile : this->bricks)
		if (!tile.destroyed)
			tile.draw(renderer);
}


// checking if all non-solid bricks are destroyed
GLboolean GameLevel::isCompleted()
{
	for (GameObject& tile : this->bricks)
		if (!tile.isSolid && !tile.destroyed)
			return GL_FALSE;
	return GL_TRUE;
}
