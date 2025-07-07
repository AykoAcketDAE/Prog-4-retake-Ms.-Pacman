#pragma once
#include "CollisionComponent.h"

class BossCollisionComponent : public dae::CollisionComponent
{
public:
	BossCollisionComponent(dae::GameObject* owner, SDL_FRect bounds);

	void Update()override;
	void Render() const override;

	void OnEnter(dae::GameObject* go)override;
	
	void WhileColliding(dae::GameObject* go)override;

};

