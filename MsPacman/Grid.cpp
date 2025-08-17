#include "Grid.h"
#include <algorithm>
#include <glm.hpp>
#if _DEBUG
#include "Renderer.h"
#include <iostream>
#endif
#include "Scene.h"

#include "Tile.h"
Grid::Grid(dae::GameObject* owner, std::vector<std::vector<Tile*>> tileArray, const std::string& NextMap)
	:BaseComponent(owner), m_Grid{ tileArray } ,m_NextMap{NextMap}
{
	FindVertices();
	FindNeighbours();
}

void Grid::Update()
{
	if (m_AmountPelletsLeft == 0)
		dae::SceneManager::GetInstance().SetNextScene(m_NextMap);
	int pellets{};
	for (int row{ 1 }; row < m_Grid.size() - 1; ++row)
	{
		for (int col{ 1 }; col < m_Grid[row].size() - 1; ++col)
		{
			if (m_Grid[row][col]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::Pellet)] == true)
			{
				++pellets;
			}
			if (m_Grid[row][col]->m_TileInfo.m_Contents[static_cast<int>(TileTypes::PowerPellet)] == true)
			{
				++pellets;
			}
		}
	}
	m_AmountPelletsLeft = pellets;
	
}

void Grid::Render() const
{
#if _DEBUG
	for(auto node :m_Vertices)
		dae::Renderer::GetInstance().RenderSquare(static_cast<int>(node.first.pos.x * 24), static_cast<int>(node.first.pos.y * 24) + 100, 24, { 255,255,255 }, false);
#endif

}

void Grid::SkipLevel()
{
	if (m_NextMap == "Map2")
	{
		LoadMap2();
	}
	if (m_NextMap == "Map3")
	{
		LoadMap3();
	}
	if (m_NextMap == "HighScore")
	{
		LoadHighScore();
	}
	dae::SceneManager::GetInstance().SetNextScene(m_NextMap);

}

void Grid::FindVertices()
{
	for (int row{ 1 }; row < m_Grid.size() - 1; ++row)
	{
		for (int col{ 1 }; col < m_Grid[row].size() - 1; ++col)
		{
			if(m_Grid[row][col]->m_TileInfo.isWalkable)
			{
				glm::vec2 neighbours{3,3};
				if (m_Grid[row + 1][col]->m_TileInfo.isWalkable and m_Grid[row][col + 1]->m_TileInfo.isWalkable)
				{
					if (neighbours == static_cast<glm::vec2>(3.f, 3.f)) neighbours = { 0,0 };
					neighbours.x += 1;
					neighbours.y += 1;
					
				}
				if (m_Grid[row - 1][col]->m_TileInfo.isWalkable and m_Grid[row][col + 1]->m_TileInfo.isWalkable)
				{
					if (neighbours == static_cast<glm::vec2>(3.f, 3.f)) neighbours = { 0,0 };
					neighbours.x = neighbours.x - 1;
					neighbours.y = neighbours.y + 1;

				}
				if (m_Grid[row + 1][col]->m_TileInfo.isWalkable and m_Grid[row][col - 1]->m_TileInfo.isWalkable)
				{
					if (neighbours == static_cast<glm::vec2>(3.f, 3.f)) neighbours = { 0,0 };
					neighbours.x  = neighbours.x + 1;
					neighbours.y = neighbours.y - 1;
				}
				if (m_Grid[row - 1][col]->m_TileInfo.isWalkable and m_Grid[row][col - 1]->m_TileInfo.isWalkable)
				{
					if (neighbours == static_cast<glm::vec2>(3.f, 3.f)) neighbours = { 0,0 };
					neighbours.x = neighbours.x - 1;
					neighbours.y = neighbours.y - 1;
				}
				if (neighbours == static_cast<glm::vec2>(3.f, 3.f)) continue;
				m_Vertices.push_back(std::pair<Node, glm::vec2>({ {row,col} }, neighbours));
			}
		}
	}
}

void Grid::FindNeighbours()
{
	for (int index{}; index < m_Vertices.size(); ++index)
	{
		
		if (m_Vertices[index].second.y >= 1)
		{
			FindSouthNeighbour(index);
		}
		if (m_Vertices[index].second.y <= -1)
		{
			FindNorthNeighbour(index);
		}
		if (m_Vertices[index].second.y == 0)
		{
			FindSouthNeighbour(index);
			FindNorthNeighbour(index);
		}
		if (m_Vertices[index].second.x >= 1)
		{
			FindEastNeighbour(index);
		}
		if (m_Vertices[index].second.x <= -1)
		{
			FindWestNeighbour(index);
		}
		if (m_Vertices[index].second.x == 0)
		{
			FindWestNeighbour(index);
			FindEastNeighbour(index);
		}
	}
}

void Grid::FindNorthNeighbour(int index)
{
	//north
	auto it = m_Vertices.rend() - index;
	        

	auto neighbour = std::find_if(it, m_Vertices.rend(), [&](std::pair<Node, glm::vec2> node) {
		if (node.first.pos.x == m_Vertices[index].first.pos.x and node.first.pos != m_Vertices[index].first.pos)
		{
			if (node.second.y + m_Vertices[index].second.y == 0) {
#if _DEBUG
				std::cout << "--------------------------------\n";
				std::cout << "North\n";
				std::cout << "Start node: " << m_Vertices[index].first.pos.x << ", " << m_Vertices[index].first.pos.y << "\n";
				std::cout << "found node: " << node.first.pos.x << ", " << node.first.pos.y << "\n";
#endif
				return true;
			}
			if (node.second.y + m_Vertices[index].second.y >= 2) {
#if _DEBUG
				std::cout << "--------------------------------\n";
				std::cout << "North\n";
				std::cout << "Start node: " << m_Vertices[index].first.pos.x << ", " << m_Vertices[index].first.pos.y << "\n";
				std::cout << "found node: " << node.first.pos.x << ", " << node.first.pos.y << "\n";
#endif
				return true;
			}
			else if (m_Vertices[index].second.y == 0)
			{
#if _DEBUG
				std::cout << "--------------------------------\n";
				std::cout << "North\n";
				std::cout << "Start node: " << m_Vertices[index].first.pos.x << ", " << m_Vertices[index].first.pos.y << "\n";
				std::cout << "found node: " << node.first.pos.x << ", " << node.first.pos.y << "\n";
#endif
				return true;
			}
			
			else return false;
		}
		else return false;
		});
	if (neighbour == m_Vertices.rend()) return;
	float distance = std::abs(m_Vertices[index].first.pos.y - neighbour->first.pos.y);
#if _DEBUG
	std::cout << "distance to Node: " << distance << "\n";
#endif
	m_Vertices[index].first.neighbours.push_back({ &neighbour->first, static_cast<int>(distance) });
}

void Grid::FindSouthNeighbour(int index)
{
	//south
	auto it = m_Vertices.begin() + index;

	auto neighbour = std::find_if(it, m_Vertices.end(), [&](std::pair<Node, glm::vec2> node) {
		if (node.first.pos.x == m_Vertices[index].first.pos.x and node.first.pos != m_Vertices[index].first.pos)
		{
			if (node.second.y + m_Vertices[index].second.y == 0 )
			{
#if _DEBUG
				std::cout << "--------------------------------\n";
				std::cout << "South\n";
				std::cout << "Start node: " << m_Vertices[index].first.pos.x << ", " << m_Vertices[index].first.pos.y << "\n";
				std::cout << "found node: " << node.first.pos.x << ", " << node.first.pos.y << "\n";
#endif
				return true;
			}
			if (node.second.y + m_Vertices[index].second.y >= 2)
			{
#if _DEBUG
				std::cout << "--------------------------------\n";
				std::cout << "South\n";
				std::cout << "Start node: " << m_Vertices[index].first.pos.x << ", " << m_Vertices[index].first.pos.y << "\n";
				std::cout << "found node: " << node.first.pos.x << ", " << node.first.pos.y << "\n";
#endif
				return true;
			}
			else if (m_Vertices[index].second.y == 0)
			{
#if _DEBUG
				std::cout << "--------------------------------\n";
				std::cout << "South\n";
				std::cout << "Start node: " << m_Vertices[index].first.pos.x << ", " << m_Vertices[index].first.pos.y << "\n";
				std::cout << "found node: " << node.first.pos.x << ", " << node.first.pos.y << "\n";
#endif
				return true;
			}
			
			else return false;
		}
		else return false;
		});
	if (neighbour == m_Vertices.end()) return;
	float distance = std::abs(m_Vertices[index].first.pos.y - neighbour->first.pos.y);
#if _DEBUG
	std::cout << "distance to Node: " << distance << "\n";
#endif
	m_Vertices[index].first.neighbours.push_back({ &neighbour->first, static_cast<int>(distance) });
}

void Grid::FindEastNeighbour(int index)
{
	//east
	auto it = m_Vertices.begin() + index;

	auto neighbour = std::find_if(it, m_Vertices.end(), [&](std::pair<Node, glm::vec2> node) {
		if (node.first.pos.y == m_Vertices[index].first.pos.y and node.first.pos != m_Vertices[index].first.pos)
		{
			if (node.second.x + m_Vertices[index].second.x == 0 )
			{
#if _DEBUG
				std::cout << "--------------------------------\n";
				std::cout << "East\n";
				std::cout << "Start node: " << m_Vertices[index].first.pos.x << ", " << m_Vertices[index].first.pos.y << "\n";
				std::cout << "found node: " << node.first.pos.x << ", " << node.first.pos.y << "\n";
#endif
				return true;
			}
			else if (m_Vertices[index].second.x == 0)
			{
#if _DEBUG
				std::cout << "--------------------------------\n";
				std::cout << "East\n";
				std::cout << "Start node: " << m_Vertices[index].first.pos.x << ", " << m_Vertices[index].first.pos.y << "\n";
				std::cout << "found node: " << node.first.pos.x << ", " << node.first.pos.y << "\n";
#endif
				return true;
			}
			else return false;
		}
		else return false;
		});
	if (neighbour == m_Vertices.end()) return;
	float distance = std::abs(m_Vertices[index].first.pos.x - neighbour->first.pos.x);
#if _DEBUG
	std::cout << "distance to Node: " << distance << "\n";
#endif
	m_Vertices[index].first.neighbours.push_back({ &neighbour->first, static_cast<int>(distance) });
}

void Grid::FindWestNeighbour(int index)
{
	//west
	auto it = m_Vertices.rbegin() + (m_Vertices.size() - index);

	auto neighbour = std::find_if(it, m_Vertices.rend(), [&](std::pair<Node, glm::vec2> node) {
		if (node.first.pos.y == m_Vertices[index].first.pos.y and node.first.pos != m_Vertices[index].first.pos)
		{
			if (node.second.x + m_Vertices[index].second.x == 0  )
			{
#if _DEBUG
				std::cout << "--------------------------------\n";
				std::cout << "West\n";
				std::cout << "Start node: " << m_Vertices[index].first.pos.x << ", " << m_Vertices[index].first.pos.y << "\n";
				std::cout << "found node: " << node.first.pos.x << ", " << node.first.pos.y << "\n";
#endif
				return true;
			}
			else if (m_Vertices[index].second.x == 0)
			{
#if _DEBUG
				std::cout << "--------------------------------\n";
				std::cout << "West\n";
				std::cout << "Start node: " << m_Vertices[index].first.pos.x << ", " << m_Vertices[index].first.pos.y << "\n";
				std::cout << "found node: " << node.first.pos.x << ", " << node.first.pos.y << "\n";
#endif
				return true;
			}
			else return false;
		}
		else return false;
		});
	if (neighbour == m_Vertices.rend()) return;
	float distance =std::abs(m_Vertices[index].first.pos.x - neighbour->first.pos.x);
#if _DEBUG
	std::cout << "distance to Node: " << distance << "\n";
#endif
	m_Vertices[index].first.neighbours.push_back({ &neighbour->first, static_cast<int>(distance) });
}
