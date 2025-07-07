#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
class PlayerComponent :public dae::BaseComponent
{
public:
	PlayerComponent(dae::GameObject* owner);

	void Update()override;
	void Render() const override;

	void SpawnBullet();
	void SetStuckPos(glm::vec3 pos)
	{
		m_StuckPos = pos;
	};
	bool m_Stuck{ false };
private:

	float m_Cooldown{};
	float m_MaxCoolDown{ 0.2f };
	int m_BulletCount{};
	std::vector<dae::GameObject> m_BulletObjects{};
	glm::vec3 m_StuckPos{};
	SDL_FRect m_HitBox{};
};

