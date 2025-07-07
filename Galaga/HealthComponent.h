#pragma once
#include "GameObject.h"
#include "BaseComponent.h"
#include "RenderComponent.h"
class HealthComponent : public dae::BaseComponent
{
public:
	HealthComponent(dae::GameObject* owner, int lives);

	void Update()override;
	void Render() const override;
	void LostLife();
	
	bool HasGrace()const { return m_LostLife; }
private:
	int m_Lives{};
	float m_HealthWidth{};
	dae::RenderComponent* m_Image;

	bool m_LostLife{ false };
	float m_GraceTime{};
	const float m_MAXGRACE{ 5 };
};

