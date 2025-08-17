#pragma once
#include <BaseComponent.h>
#include <RenderComponent.h>
#include "Observer.h"
#include "MsPacmanCommands.h"
#include "Tile.h"
class Grid;

struct PlayerCommands
{
	std::unique_ptr<AddPelletScore> scorePellet;
	std::unique_ptr<AddPowerPelletScore> scorePowerPellet;

	std::unique_ptr<AddOneGhost> oneGhost;
	std::unique_ptr<AddTwoGhost> twoGhost;
	std::unique_ptr<AddThreeGhost> threeGhost;
	std::unique_ptr<AddFourGhost> fourGhost;

	std::vector<std::unique_ptr<SetGhostToSpawn>> killGhost;
	std::vector<std::unique_ptr<SetGhostToScatter>> scatterGhost;
	std::unique_ptr<PlayerDied> playerDied;
};

class Player : public dae::BaseComponent
{
public:

	struct PlayerInfo;

	Player(dae::GameObject* owner,dae::RenderComponent* renderComp,PlayerInfo playerInfo,Grid* grid, PlayerCommands playerCommands, const TileTypes& tileType);

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
	int m_EatenGhost{};
	TileTypes m_TileType{};
	
};

