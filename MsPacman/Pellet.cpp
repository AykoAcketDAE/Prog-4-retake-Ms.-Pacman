#include "Pellet.h"
#include "Renderer.h"
#include <RenderComponent.h>
#include "Grid.h"
#include "Tile.h"
Pellet::Pellet(dae::GameObject* owner, Grid* gridComp, int col, int row,  bool isPowerPellet):
	BaseComponent(owner), m_GridComp{ gridComp }, m_IsPowerPellet{ isPowerPellet }, m_Row{ row }, m_Col{col}
{
	m_GridComp->m_Grid[col][row]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::Pellet)] = true;
}

void Pellet::Update()
{
	
}

void Pellet::Render() const
{
	
	if(m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::Pellet)] == true)
	{
		dae::Renderer::GetInstance().RenderSquare((m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.row * 24) + 9, (m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.col * 24) + 9+100, 6, { 255,255,0 }, true);
	}
	else if (m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::PowerPellet)] == true)
	{
		dae::Renderer::GetInstance().RenderSquare((m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.row * 24) + 6, (m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.col * 24) + 100, 24, { 255,255,0 }, true);
		dae::Renderer::GetInstance().RenderSquare((m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.row * 24), (m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.col * 24) + 6 + 100, 24, { 255,255,0 }, true);
		dae::Renderer::GetInstance().RenderSquare((m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.row * 24)+3, (m_GridComp->m_Grid[m_Col][m_Row]->m_TileInfo.col * 24) + 3 + 100, 18, { 255,255,0 }, true);
	}

}
