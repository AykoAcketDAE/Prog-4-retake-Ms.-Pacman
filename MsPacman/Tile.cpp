#include "Tile.h"
#if _DEBUG
#include "Renderer.h"

#endif

Tile::Tile(dae::GameObject* owner, int row, int col, TileInfo info):
	BaseComponent(owner),m_Row{row},m_Col{col}, m_Info{info}
{
}

void Tile::Update()
{

}

void Tile::Render() const
{
#if _DEBUG
	dae::Renderer::GetInstance().RenderSquare(m_Info.x, m_Info.y, m_Info.width, { 255,255,255 });
#endif
}
