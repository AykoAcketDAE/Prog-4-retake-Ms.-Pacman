#pragma once
#include "Command.h"
#include "GameObject.h"
class SetPlayerDirection final : public dae::Command
{
public:
	SetPlayerDirection(dae::GameObject* go, glm::vec2 direction);
	virtual ~SetPlayerDirection() = default;
	void Execute() override;

private:
	bool m_State{ false };
	dae::GameObject* m_GameObject{ nullptr };
	glm::vec2 m_Direction{};
};

