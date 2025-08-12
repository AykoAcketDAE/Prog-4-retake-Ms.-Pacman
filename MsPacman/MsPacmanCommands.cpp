#include "MsPacmanCommands.h"
#include "Player.h"
#include "Score.h"
#include "Ghost.h"
#include "GhostStates.h"
#include "Scene.h"
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

AddPowerPelletScore::AddPowerPelletScore(dae::GameObject* playerGo, dae::GameObject* scoreGo)
	:m_PlayerGo{ playerGo }, m_ScoreGo{ scoreGo }
{
}

void AddPowerPelletScore::Execute()
{
	m_ScoreGo->GetComponent<Score>()->AddScore(m_Score);
}

SetGhostToScatter::SetGhostToScatter(dae::GameObject* Ghost)
	:m_Ghost{ Ghost }
{
}

void SetGhostToScatter::Execute()
{
	if(m_Ghost->GetComponent<Ghost>()->m_CurrentState != m_Ghost->GetComponent<Ghost>()->m_SpawnState.get())
		m_Ghost->GetComponent<Ghost>()->SetState(m_Ghost->GetComponent<Ghost>()->m_ScatterState.get());
}

SetGhostToChase::SetGhostToChase(dae::GameObject* Ghost)
	:m_Ghost{ Ghost }
{
}

void SetGhostToChase::Execute()
{
	m_Ghost->GetComponent<Ghost>()->SetState(m_Ghost->GetComponent<Ghost>()->m_ChaseState.get());
}

SetGhostToSpawn::SetGhostToSpawn(dae::GameObject* Ghost)
	:m_Ghost{ Ghost }
{
}

void SetGhostToSpawn::Execute()
{
	m_Ghost->GetComponent<Ghost>()->SetState(m_Ghost->GetComponent<Ghost>()->m_SpawnState.get());
}

void SkipLevel::Execute()
{
	dae::SceneManager::GetInstance().GetCurrentScene()->FindComponentInScene<Grid>()->SkipLevel();
}
