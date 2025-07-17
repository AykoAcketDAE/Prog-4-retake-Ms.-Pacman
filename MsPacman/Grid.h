#pragma once
#include "BaseComponent.h"
#include "Tile.h"
class Grid : public dae::BaseComponent
{
public:
	Grid(dae::GameObject* owner, std::vector<std::vector<Tile*>> tileArray) :BaseComponent(owner), m_Grid{ tileArray } {};
	std::vector<std::vector<Tile*>> m_Grid{};
	virtual void Update() override {};
	virtual void Render() const override {};
};

