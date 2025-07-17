#pragma once
#include <BaseComponent.h>
#include <RenderComponent.h>
#include "Grid.h"
#include "Tile.h"

class Player : public dae::BaseComponent
{
public:

	struct PlayerInfo;

	Player(dae::GameObject* owner,dae::RenderComponent* renderComp,PlayerInfo playerInfo,Grid* grid);

	void Update() override;
	void Render() const override;

	struct PlayerInfo
	{
		float time{};
		bool isMoving{};
		glm::vec2 direction{};
		glm::vec2 gridPos{};
	} m_PlayerInfo;
	

private:
	float m_LerpTimer{};
	dae::RenderComponent* m_RenderComp{};
	Grid* m_GridComp{};
};

