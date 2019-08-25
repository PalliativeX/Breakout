#include "GameObject.h"

GameObject::GameObject() : position(0, 0), size(1, 1), velocity(0.f), color(1.f), rotation(0.f), sprite(), isSolid(false), destroyed(false)
{ }


GameObject::GameObject(glm::vec2 pos, glm::vec2 s, Texture2D spr, glm::vec3 col, glm::vec2 vel)
	: position(pos), size(s), velocity(vel), color(col), rotation(0.f), sprite(spr), isSolid(false), destroyed(false)
{ }


void GameObject::draw(SpriteRenderer & renderer)
{
	renderer.drawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}
