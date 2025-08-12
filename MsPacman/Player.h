#pragma once
#include <BaseComponent.h>
#include <RenderComponent.h>
#include "Observer.h"
#include "MsPacmanCommands.h"

class Grid;
class Tile;

struct PlayerCommands
{
	std::unique_ptr<AddPelletScore> scorePellet;
	std::unique_ptr<AddPowerPelletScore> scorePowerPellet;
	std::vector<std::unique_ptr<SetGhostToSpawn>> killGhost;
	std::vector<std::unique_ptr<SetGhostToScatter>> scatterGhost;

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
	void SetCanEatGhostTrue();

private:
	
	void UpdatePlayerLocation();
	PlayerCommands m_PlayerCommands{};
	float m_LerpTimer{};
	dae::RenderComponent* m_RenderComp{};
	Grid* m_GridComp{};
	bool m_CanEatGhost{ false };
	float m_GhostTimer{};
	float m_EatenGhost{};
};

