#pragma once
#include "BaseComponent.h"
#include "Grid.h"

class Pellet : public dae::BaseComponent
{
public:
	Pellet(dae::GameObject* owner,Grid* m_GridComp,int col,int row, bool isPowerPellet);
	void Update() override;
	void Render() const override;
private:
	
	Grid* m_GridComp{};
	bool m_IsPowerPellet{false};
	int m_Col{};
	int m_Row{};
};

