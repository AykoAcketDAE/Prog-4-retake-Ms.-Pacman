#include "Tile.h"
#if _DEBUG
#include "Renderer.h"

#endif

Tile::Tile(dae::GameObject* owner, int row, int col, TileInfo info):
	BaseComponent(owner),x{row},y{col}, m_TileInfo{info}
{
}

void Tile::Update()
{

}

void Tile::Render() const
{
#if _DEBUG
	if(m_TileInfo.isWalkable)
		dae::Renderer::GetInstance().RenderSquare(x, y, m_TileInfo.width, { 255,255,255 },false);
	else if (m_TileInfo.m_Contents[static_cast<int>(TileTypes::Pacman)] == true)
		dae::Renderer::GetInstance().RenderSquare(x, y, m_TileInfo.width, { 255,255,0 },true);
	else
		dae::Renderer::GetInstance().RenderSquare(x, y, m_TileInfo.width, { 255,0,255 },false);

#endif
}
