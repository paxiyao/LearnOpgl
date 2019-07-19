#include "game_object.h"

GameObject::GameObject()
	: Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(GL_FALSE), Destroyed(GL_FALSE) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
	:Position(pos), Size(size), Sprite(sprite), Color(color), Velocity(velocity), Rotation(0.0f), IsSolid(GL_FALSE), Destroyed(GL_FALSE) { }
	
// Draw sprite
void GameObject::Draw(SpriteRenderer& render)
{
	if (!this->Destroyed)
		render.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}