#include "Player.h"
#include "Timer.h"
#include <cmath>
#include <vector>
#include <iostream>
Player::Player(dae::GameObject* owner, dae::RenderComponent* renderComp, PlayerInfo playerInfo, Grid* grid)
	:BaseComponent(owner),m_RenderComp{renderComp},m_PlayerInfo{playerInfo},m_GridComp{grid}
{
	m_GridComp->m_Grid[static_cast<int>(m_PlayerInfo.gridPos.x)]
		[static_cast<int>(m_PlayerInfo.gridPos.y)]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::Pacman)] = true;
	
}

void Player::Update()
{
	TileInfo nextTile;
	TileInfo currentTile;
	bool transition{false};
	if(m_PlayerInfo.gridPos.x + m_PlayerInfo.direction.x <=-1)
	{
		nextTile = m_GridComp->m_Grid[27][static_cast<int>(m_PlayerInfo.gridPos.y + m_PlayerInfo.direction.y)]->m_TileInfo;
		currentTile = m_GridComp->m_Grid[27][static_cast<int>(m_PlayerInfo.gridPos.y + m_PlayerInfo.direction.y)]->m_TileInfo;
		transition = true;
	}
	else if(m_PlayerInfo.gridPos.x + m_PlayerInfo.direction.x >= 28)
	{
		nextTile = m_GridComp->m_Grid[0][static_cast<int>(m_PlayerInfo.gridPos.y + m_PlayerInfo.direction.y)]->m_TileInfo;
		currentTile = m_GridComp->m_Grid[0][static_cast<int>(m_PlayerInfo.gridPos.y + m_PlayerInfo.direction.y)]->m_TileInfo;
		transition = true;
	}
	else
	{
		nextTile = m_GridComp->m_Grid[static_cast<int>(m_PlayerInfo.gridPos.x + m_PlayerInfo.direction.x)][static_cast<int>(m_PlayerInfo.gridPos.y + m_PlayerInfo.direction.y)]->m_TileInfo;
		currentTile = m_GridComp->m_Grid[static_cast<int>(m_PlayerInfo.gridPos.x + m_PlayerInfo.direction.x)][static_cast<int>(m_PlayerInfo.gridPos.y + m_PlayerInfo.direction.y)]->m_TileInfo;
	}

	
	if(nextTile.isWalkable == true)
	{
		m_LerpTimer += dae::Time::GetInstance().GetDeltaTime();
		if (float t = m_LerpTimer / m_PlayerInfo.time; t < 1)
		{
			if (transition)
			{
				GetOwner()->SetLocalPosition({ (nextTile.row*24),
									 (nextTile.col * 24) + 100 ,0});
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
			currentTile.m_Contents[static_cast<int>(TileTypes::Pacman)] = false;
			//current tile
			nextTile.m_Contents[static_cast<int>(TileTypes::Pacman)] = true;
			if (transition)
			{
				m_PlayerInfo.gridPos = { nextTile.row ,nextTile.col};
				m_LerpTimer = 0;
				GetOwner()->SetLocalPosition({ (nextTile.row*24),
					(nextTile.col*24) + 100 ,0 });
				std::cout << "x: " << m_PlayerInfo.gridPos.x << "  ,y: " << m_PlayerInfo.gridPos.y << std::endl;
			}
			else
			{
				m_PlayerInfo.gridPos = m_PlayerInfo.gridPos + m_PlayerInfo.direction;
				m_LerpTimer = 0;
				GetOwner()->SetLocalPosition({ (m_PlayerInfo.gridPos.x) * 24,
					((m_PlayerInfo.gridPos.y) * 24) + 100 ,0 });
				std::cout << "x: " << m_PlayerInfo.gridPos.x << "  ,y: " << m_PlayerInfo.gridPos.y << std::endl;
			}
			
		}
	}
	else
	{
		m_LerpTimer = 0;
		currentTile.m_Contents[static_cast<int>(TileTypes::Pacman)] = true;
	}
}

void Player::Render() const
{

}
