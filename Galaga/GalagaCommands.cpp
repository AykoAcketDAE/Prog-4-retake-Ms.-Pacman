#include "GalagaCommands.h"
#include "Player.h"
#include "Scene.h"
SpawnBullet::SpawnBullet(dae::GameObject* go) :
	m_GameObject{ go }
{
	
}

void SpawnBullet::Execute()
{
	m_GameObject->GetComponent<PlayerComponent>()->SpawnBullet();
}

LoseLife::LoseLife(dae::GameObject* go):
	m_GameObject{go}
{

}

void LoseLife::Execute()
{
	m_GameObject->GetComponent<HealthComponent>()->LostLife();
}

SwitchToScene::SwitchToScene(dae::GameObject* go,const std::string& scene)
	:m_GameObject{go},m_Scene{scene}
{

}

void SwitchToScene::Execute()
{
	dae::SceneManager::GetInstance().SetNextScene(m_Scene);
}
