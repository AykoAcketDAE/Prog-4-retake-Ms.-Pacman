#pragma once
#include <BaseComponent.h>
#include <RenderComponent.h>
#include "Grid.h"
#include "Tile.h"
#include "Observer.h"
#include "MsPacmanCommands.h"
struct PlayerCommands
{
	std::unique_ptr<AddPelletScore> m_ScorePellet;
};

class Player : public dae::BaseComponent
{
public:

	struct PlayerInfo;

	Player(dae::GameObject* owner,dae::RenderComponent* renderComp,PlayerInfo playerInfo,Grid* grid, PlayerCommands playerCommands);

	void Update() override;
	void Render() const override;

	struct PlayerInfo
	{
		float time{};
		bool isMoving{};
		glm::vec2 direction{};
		glm::vec2 gridPos{};
	} m_PlayerInfo;
	
	bool CheckDirection(glm::vec2 direction);

private:
	void UpdatePlayerLocation();
	PlayerCommands m_PlayerCommands{};
	float m_LerpTimer{};
	dae::RenderComponent* m_RenderComp{};
	Grid* m_GridComp{};

};

