#include "Pellet.h"
#include "Renderer.h"
#include <RenderComponent.h>
#include "Grid.h"
#include "Tile.h"
Pellet::Pellet(dae::GameObject* owner, Grid* gridComp, int col, int row,  bool isPowerPellet):
	BaseComponent(owner), m_GridComp{ gridComp }, m_IsPowerPellet{ isPowerPellet }, m_Row{ row }, m_Col{col}
{
	m_GridComp->m_Grid[col][row]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::pellet)] = true;
}

void Pellet::Update()
{
	//if (m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::Pacman)] == true)
	//	m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::pellet)] = false;
}

void Pellet::Render() const
{
	
	if(m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::pellet)] == true)
	{
		dae::Renderer::GetInstance().RenderSquare((m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.row * 24) + 9, (m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.col * 24) + 9+100, 6, { 255,255,0 }, true);
	}
}
