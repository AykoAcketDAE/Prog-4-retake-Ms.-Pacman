#pragma once
#include "BaseComponent.h"
#include <utility>
#include "MsPacman.h"
class Tile;
struct Node
{
	glm::vec2 pos;
	std::vector<std::pair<Node*, int>> neighbours;
};

class Grid : public dae::BaseComponent
{
public:
	Grid(dae::GameObject* owner, std::vector<std::vector<Tile*>> tileArray,const std::string& NextMap);
	std::vector<std::vector<Tile*>> m_Grid{};
	void Update() override;
	void Render() const override;

	void SkipLevel();
	
	std::vector<std::pair<Node, glm::vec2>> m_Vertices;
	std::string m_NextMap;
private:

	void FindVertices();
	void FindNeighbours();
	
	void FindNorthNeighbour(int index);
	void FindSouthNeighbour(int index);
	void FindEastNeighbour(int index);
	void FindWestNeighbour(int index);
	int m_AmountPelletsLeft{std::numeric_limits<int>::max()};
};

