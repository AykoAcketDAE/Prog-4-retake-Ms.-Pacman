#include "MsPacmanCommands.h"
#include "Player.h"
#include "Score.h"
SetPlayerDirection::SetPlayerDirection(dae::GameObject* go, glm::vec2 direction)
	:dae::Command(), m_GameObject{ go }, m_Direction{ direction }
{
}

void SetPlayerDirection::Execute()
{
	//m_GameObject->GetComponent<Player>()->m_PlayerInfo.direction = { 0 ,0};
	if(m_GameObject->GetComponent<Player>()->CheckDirection(m_Direction))
		m_GameObject->GetComponent<Player>()->m_PlayerInfo.direction = m_Direction;

}


AddPelletScore::AddPelletScore(dae::GameObject* playerGo, dae::GameObject* scoreGo)
	:m_PlayerGo{playerGo},m_ScoreGo{scoreGo}
{
}

void AddPelletScore::Execute()
{
	m_ScoreGo->GetComponent<Score>()->AddScore(m_Score);
}
