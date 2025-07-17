#include "MsPacmanCommands.h"
#include "Player.h"
SetPlayerDirection::SetPlayerDirection(dae::GameObject* go, glm::vec2 direction)
	:dae::Command(), m_GameObject{ go }, m_Direction{ direction }
{
}

void SetPlayerDirection::Execute()
{
	m_GameObject->GetComponent<Player>()->m_PlayerInfo.direction = { 0 ,0};
	m_GameObject->GetComponent<Player>()->m_PlayerInfo.direction = m_Direction;
}
