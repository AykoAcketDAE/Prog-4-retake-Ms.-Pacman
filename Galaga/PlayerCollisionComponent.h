#pragma once
#include "CollisionComponent.h"
#include "GalagaCommands.h"
#include <memory>
#include "HealthComponent.h"

class PlayerCollisionComponent: public dae::CollisionComponent
{
public:
	PlayerCollisionComponent(dae::GameObject* owner, SDL_FRect bounds, dae::GameObject* healthObject);

	void Update()override;
	void Render() const override;

	void OnEnter(dae::GameObject* go)override;

	void WhileColliding(dae::GameObject* go)override;

private:
	int m_Counter{};
	std::unique_ptr<LoseLife>m_LoseLife;
};

