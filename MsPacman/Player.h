#pragma once
#include <BaseComponent.h>
#include <RenderComponent.h>

class Player : public dae::BaseComponent
{
public:
	Player(dae::GameObject* owner,dae::RenderComponent* renderComp);

	void Update() override;
	void Render() const override;

private:

	dae::RenderComponent* m_RenderComp;
	glm::vec2 m_Direction;
	float m_Speed;
	bool m_IsMoving;
	glm::vec2 m_GridPos;
};

