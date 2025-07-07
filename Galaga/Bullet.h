#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"

class Bullet : public dae::BaseComponent
{
public:
	Bullet(dae::GameObject* owner, float offset) : dae::BaseComponent(owner), m_Offset{ offset,0,0 } {};

	void Update()override;
	void Render() const override;

	void SpawnBullet();
	bool GetIsFired() const { return m_IsFired; };

	SDL_FRect GetHitBox();
	void SetIsFiredOff();
	void HasHit() {
		m_Position.y = -800;
		m_IsFired = false;
	}
private:
	
	SDL_FRect m_HitBox{4.f,6.f,3.f,8.f};
	bool m_IsFired{ false };
	glm::vec3 m_Position{};
	float m_FirePos{};
	glm::vec3 m_Offset{};
	
};



class BulletCollisionComponent : public dae::CollisionComponent
{
public:
	BulletCollisionComponent(dae::GameObject* owner, SDL_FRect bounds);

	void Update() override;
	void Render() const override {};

	void OnEnter(dae::GameObject* go)override;
	
	void WhileColliding(dae::GameObject* go)override;
};

