#pragma once
#include "BaseComponent.h"
class Pellet : public dae::BaseComponent
{
public:
	Pellet(dae::GameObject* owner, bool isPowerPellet);
	void Update() override;
	void Render() const override;
private:

};

