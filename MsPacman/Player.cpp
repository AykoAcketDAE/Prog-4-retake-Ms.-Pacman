#include "Player.h"
#include "Timer.h"
#include <cmath>
#include <vector>
#include <iostream>
#include "Grid.h"
Player::Player(dae::GameObject* owner, dae::RenderComponent* renderComp, PlayerInfo playerInfo, Grid* grid, PlayerCommands playerCommands)
	:BaseComponent(owner), m_RenderComp{ renderComp }, m_PlayerInfo{ playerInfo }, m_GridComp{ grid }
{
	m_PlayerCommands = std::move(playerCommands);
	m_GridComp->m_Grid[static_cast<int>(m_PlayerInfo.gridPos.x)]
		[static_cast<int>(m_PlayerInfo.gridPos.y)]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::Pacman)] = true;
	
}

void Player::Update()
{
	UpdatePlayerLocation();
	if (m_CanEatGhost and m_GhostTimer <= 7.f)
	{
		m_GhostTimer += dae::Time::GetInstance().GetDeltaTime(); 
		TileInfo* currentTile{ &m_GridComp->m_Grid[static_cast<int>(m_PlayerInfo.gridPos.x)][static_cast<int>(m_PlayerInfo.gridPos.y)]->m_TileInfo };

		for (int index{}; index < 4; ++index)
		{
			if (currentTile->m_Contents[index] == true)
			{
				m_PlayerCommands.killGhost[index]->Execute();
			}
		}  
		
		
	}
	else
	{
		m_CanEatGhost = false;
		m_GhostTimer = 0;
	}
}

void Player::Render() const
{

}

bool Player::CheckDirection(glm::vec2 direction)
{
	TileInfo* nextTile;
	if (m_PlayerInfo.gridPos.x + direction.x <= -1) return false;
	if (m_PlayerInfo.gridPos.x + direction.x >= 28) return false;
	nextTile = &m_GridComp->m_Grid[static_cast<int>(m_PlayerInfo.gridPos.x +direction.x)][static_cast<int>(m_PlayerInfo.gridPos.y + direction.y)]->m_TileInfo;

	if (nextTile->isWalkable) return true;
	return false;
}

void Player::SetCanEatGhostTrue()
{
	m_CanEatGhost = true;
	for (auto& ghost : m_PlayerCommands.scatterGhost)
	{
		ghost->Execute();
	}
}


void Player::UpdatePlayerLocation()
{
	TileInfo* nextTile;
	TileInfo* currentTile;
	bool transition{ false };

	if (m_PlayerInfo.gridPos.x + m_PlayerInfo.direction.x <= -1)// When player teleports to other side of map
	{
		nextTile = &m_GridComp->m_Grid[27][static_cast<int>(m_PlayerInfo.gridPos.y + m_PlayerInfo.direction.y)]->m_TileInfo;
		currentTile = &m_GridComp->m_Grid[27][static_cast<int>(m_PlayerInfo.gridPos.y)]->m_TileInfo;
		transition = true;
	}
	else if (m_PlayerInfo.gridPos.x + m_PlayerInfo.direction.x >= 28)// When player teleports to other side of map
	{
		nextTile = &m_GridComp->m_Grid[0][static_cast<int>(m_PlayerInfo.gridPos.y + m_PlayerInfo.direction.y)]->m_TileInfo;
		currentTile = &m_GridComp->m_Grid[0][static_cast<int>(m_PlayerInfo.gridPos.y)]->m_TileInfo;
		transition = true;
	}
	else
	{
		nextTile = &m_GridComp->m_Grid[static_cast<int>(m_PlayerInfo.gridPos.x + m_PlayerInfo.direction.x)][static_cast<int>(m_PlayerInfo.gridPos.y + m_PlayerInfo.direction.y)]->m_TileInfo;
		currentTile = &m_GridComp->m_Grid[static_cast<int>(m_PlayerInfo.gridPos.x)][static_cast<int>(m_PlayerInfo.gridPos.y)]->m_TileInfo;
	}

	currentTile->m_Contents[static_cast<int>(TileTypes::Pacman)] = true;
	if (currentTile->m_Contents[static_cast<int>(TileTypes::Pellet)] == true)
	{
		currentTile->m_Contents[static_cast<int>(TileTypes::Pellet)] = false;
		m_PlayerCommands.scorePellet->Execute();
	}
	if (currentTile->m_Contents[static_cast<int>(TileTypes::PowerPellet)] == true)
	{
		currentTile->m_Contents[static_cast<int>(TileTypes::PowerPellet)] = false;
		m_PlayerCommands.scorePowerPellet->Execute();
		SetCanEatGhostTrue();
		
	}
	
	

	if (nextTile->isWalkable == true)
	{
		m_LerpTimer += dae::Time::GetInstance().GetDeltaTime();
		if (float t = m_LerpTimer / m_PlayerInfo.time; t < 1)
		{
			if (transition)
			{
				GetOwner()->SetLocalPosition({ (nextTile->row * 24),
									 (nextTile->col * 24) + 100 ,0 });
			}
			else
			{
				GetOwner()->SetLocalPosition({ std::lerp((m_PlayerInfo.gridPos.x) * 24,(m_PlayerInfo.gridPos.x + m_PlayerInfo.direction.x) * 24,t),
									std::lerp(((m_PlayerInfo.gridPos.y) * 24) + 100,((m_PlayerInfo.gridPos.y + m_PlayerInfo.direction.y) * 24) + 100,t),0 });
			}
		}
		else
		{
			// previous tile
			currentTile->m_Contents[static_cast<int>(TileTypes::Pacman)] = false;
			// current tile
			nextTile->m_Contents[static_cast<int>(TileTypes::Pacman)] = true;
			if (transition)
			{
				m_PlayerInfo.gridPos = { nextTile->row ,nextTile->col };
				m_LerpTimer = 0;
				GetOwner()->SetLocalPosition({ (nextTile->row * 24),
					(nextTile->col * 24) + 100 ,0 });
				//std::cout << "x: " << m_PlayerInfo.gridPos.x << "  ,y: " << m_PlayerInfo.gridPos.y << std::endl;
			}
			else
			{
				m_PlayerInfo.gridPos = m_PlayerInfo.gridPos + m_PlayerInfo.direction;
				m_LerpTimer = 0;
				GetOwner()->SetLocalPosition({ (m_PlayerInfo.gridPos.x) * 24,
					((m_PlayerInfo.gridPos.y) * 24) + 100 ,0 });
				//std::cout << "x: " << m_PlayerInfo.gridPos.x << "  ,y: " << m_PlayerInfo.gridPos.y << std::endl;
			}
		}
	}
	else
	{
		m_LerpTimer = 0;
		currentTile->m_Contents[static_cast<int>(TileTypes::Pacman)] = true;
	}
}
