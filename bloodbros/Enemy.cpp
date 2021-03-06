#include "Application.h"
#include "Enemy.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"

Enemy::Enemy(int x, int y) : position(x, y), collider(nullptr)
{}

Enemy::~Enemy()
{
	if (collider != nullptr)
		App->collision->EraseCollider(collider);
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);
	if (this != nullptr){
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
		
	}
	
}
void Enemy::OnCollision(Collider* c1, Collider* c2){

}
	
