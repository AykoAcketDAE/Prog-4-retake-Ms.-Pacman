#include "MsPacmanCommands.h"
#include "Player.h"
#include "Score.h"
#include "Ghost.h"
#include "GhostStates.h"
#include "Scene.h"
#include "HealthComponent.h"
#include "MsPacman.h"

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
	{
		m_Ghost->GetComponent<Ghost>()->SetState(m_Ghost->GetComponent<Ghost>()->m_ScatterState.get());
		m_Ghost->GetComponent<Ghost>()->m_RenderComp->SetTexture("ScaredGhost.png");
	}
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

PlayerDied::PlayerDied(dae::GameObject* go)
	:m_GameObject{ go }
{

}

void PlayerDied::Execute()
{
	m_GameObject->GetComponent<HealthComponent>()->LostLife();
}

void GotoStartScreen::Execute()
{
	LoadStartScreen();
	dae::SceneManager::GetInstance().SetNextScene("StartScreen");
}

void SetSolo::Execute()
{
	SetBoolSolo();
	LoadMap3();
	LoadMap2();
	LoadMap1();
	
	
}

void SetCoop::Execute()
{
	
	SetBoolCoop();
	
	LoadMap3();
	LoadMap2();
	LoadMap1();
}

void SetVersus::Execute()
{
	SetBoolVersus();
	LoadMap3();
	LoadMap2();
	LoadMap1();
}

AddOneGhost::AddOneGhost(dae::GameObject* playerGo, dae::GameObject* scoreGo)
	:m_PlayerGo{ playerGo }, m_ScoreGo{ scoreGo }
{
}

void AddOneGhost::Execute()
{
	m_ScoreGo->GetComponent<Score>()->AddScore(m_Score);
}

AddTwoGhost::AddTwoGhost(dae::GameObject* playerGo, dae::GameObject* scoreGo)
	:m_PlayerGo{ playerGo }, m_ScoreGo{ scoreGo }
{
}

void AddTwoGhost::Execute()
{
	m_ScoreGo->GetComponent<Score>()->AddScore(m_Score);
}

AddThreeGhost::AddThreeGhost(dae::GameObject* playerGo, dae::GameObject* scoreGo)
	:m_PlayerGo{ playerGo }, m_ScoreGo{ scoreGo }
{
}

void AddThreeGhost::Execute()
{
	m_ScoreGo->GetComponent<Score>()->AddScore(m_Score);
}

AddFourGhost::AddFourGhost(dae::GameObject* playerGo, dae::GameObject* scoreGo)
	:m_PlayerGo{ playerGo }, m_ScoreGo{ scoreGo }
{
}

void AddFourGhost::Execute()
{
	m_ScoreGo->GetComponent<Score>()->AddScore(m_Score);
}

void MuteMusic::Execute()
{
	dae::servicelocator::GetSoundSystem().Mute();
}
