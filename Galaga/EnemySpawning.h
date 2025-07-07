#pragma once
#include "GameObject.h"
#include "BaseComponent.h"
#include <memory>
#include <vector>
#include "Enemy.h"
#include "EnemyBee.h"
#include "EnemyButterfly.h"
#include "Scene.h"

class EnemySpawning: public dae::BaseComponent
{
public:
	EnemySpawning(dae::GameObject* owner,dae::Scene& scene);

	void Update()override;
	void Render() const override;

private:
	
	std::vector<std::unique_ptr<dae::GameObject>> m_EnemyGos;
	std::vector<Enemy*> m_EnemyComps;
	std::vector < std::vector<glm::vec2>> m_GridLocations{};
};