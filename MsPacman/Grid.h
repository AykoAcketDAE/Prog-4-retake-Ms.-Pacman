#pragma once
#include "BaseComponent.h"
#include "Tile.h"
#include <utility>
#include "Player.h"
class Player;

struct Node
{
	glm::vec2 pos;
	std::vector<std::pair<Node*, int>> neighbours;
};

class Grid : public dae::BaseComponent
{
public:
	Grid(dae::GameObject* owner, std::vector<std::vector<Tile*>> tileArray);
	std::vector<std::vector<Tile*>> m_Grid{};
	void Update() override;
	void Render() const override;

	
	Player::PlayerInfo* m_MsPacman;
	std::vector<std::pair<Node, glm::vec2>> m_Vertices;
private:

	void FindVertices();
	void FindNeighbours();
	
	void FindNorthNeighbour(int index);
	void FindSouthNeighbour(int index);
	void FindEastNeighbour(int index);
	void FindWestNeighbour(int index);
};

