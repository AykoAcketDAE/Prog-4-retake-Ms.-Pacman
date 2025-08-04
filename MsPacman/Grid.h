#pragma once
#include "BaseComponent.h"
#include "Tile.h"
#include <utility>

class Grid : public dae::BaseComponent
{
public:
	Grid(dae::GameObject* owner, std::vector<std::vector<Tile*>> tileArray);
	std::vector<std::vector<Tile*>> m_Grid{};
	void Update() override;
	void Render() const override;

	struct Node
	{
		glm::vec2 pos;
		std::vector<std::pair<Node*,int>> neighbours;
	};

	std::vector<std::pair<Node, glm::vec2>> m_Vertices;
private:
	void FindVertices();
	void FindNeighbours();
	
	void FindNorthNeighbour(int index);
	void FindSouthNeighbour(int index);
	void FindEastNeighbour(int index);
	void FindWestNeighbour(int index);
};

