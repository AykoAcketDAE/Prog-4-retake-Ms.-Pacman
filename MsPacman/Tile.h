#pragma once
#include <BaseComponent.h>
enum class TileType
{
	Pacman,blinky,inky,pinky,clyde,pellet,powerPellet,Powerup
};
struct TileInfo
{
	int x{};
	int y{};
	int width{};
	bool isWalkable{true};
	std::vector<TileType> TypesInTile{};
};
class Tile : public dae::BaseComponent
{
public:
	Tile(dae::GameObject* owner,int row,int col, TileInfo info);

	void Update() override;
	void Render() const override;

private:
	int m_Row{};
	int m_Col{};
	TileInfo m_Info;
};

