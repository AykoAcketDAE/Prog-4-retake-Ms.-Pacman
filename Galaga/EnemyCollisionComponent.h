#pragma once
#include "CollisionComponent.h"

class EnemyCollisionComponent : public dae::CollisionComponent
{
public:
	EnemyCollisionComponent(dae::GameObject* owner, SDL_FRect bounds);

	void Update()override;
	void Render() const override;

	void OnEnter(dae::GameObject* go)override;

	void WhileColliding(dae::GameObject* go)override;
private:
	
};
