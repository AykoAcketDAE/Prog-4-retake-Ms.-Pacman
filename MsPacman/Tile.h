#pragma once
#include <BaseComponent.h>

enum class TileTypes{
	Blinky,
	Inky,
	Pinky,
	Clyde,
	Pacman,
	Pellet,
	PowerPellet,
	PowerUp,
};
struct TileInfo
{
	int row{};
	int col{};
	int width{};
	bool isWalkable{true};
	std::vector<bool> m_Contents{
	false,//bool Pacman;
	false,//bool blinky;
	false,//bool inky;
	false,//bool pinky;
	false,//bool clyde;
	false,//bool pellet;
	false,//bool powerPellet;
	false //bool Powerup;
	};
};

class Tile : public dae::BaseComponent
{
public:
	Tile(dae::GameObject* owner,int row,int col, TileInfo info);

	void Update() override;
	void Render() const override;

	TileInfo m_TileInfo;
private:
	
	int x{};
	int y{};
};

