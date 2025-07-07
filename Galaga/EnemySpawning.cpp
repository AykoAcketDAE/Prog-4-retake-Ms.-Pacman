#include "EnemySpawning.h"
#include "EnemyCollisionComponent.h"
#include "EnemyMath.h"
#include "RenderComponent.h"
#include "SpriteAnimation.h"
#include "Boss.h"


EnemySpawning::EnemySpawning(dae::GameObject* owner, dae::Scene& scene)
	:dae::BaseComponent(owner)
{
	SDL_FRect src = { 0.f,0.f,16.f,16.f };
	SDL_FRect dst = { -500.f, -500.f, 48.f,48.f };
	
	ReadBezier readFile;
	if(!readFile.m_Info.read)
	{
		readFile.GetBezierDataFromFile("Beziers.txt");
		readFile.m_Info.read = true;
	}
	
	for (int row{}; row < readFile.m_Info.gridPos.size(); ++row)
	{
		for (int col{}; col < readFile.m_Info.gridPos[row].size(); ++col)
		{
			auto enemy = std::make_unique<dae::GameObject>();
			if (row == 0)
			{
				Enemy::EnemyStart start{ readFile.m_Info.gridPos[row][col], Enemy::AnimationPath::leftTop, Enemy::AnimationPath::BossLeft };
				enemy->AddComponent<Boss>(start,50);
				src = { 0.f,0.f,16.f,16.f };
				dst = { -500.f, -500.f, 48.f,48.f };
				enemy->AddComponent<dae::RenderComponent>("Boss.png", src, dst);
			}
			if (row == 1)
			{
				Enemy::EnemyStart start{ readFile.m_Info.gridPos[row][col], Enemy::AnimationPath::rightTop, Enemy::AnimationPath::bomingRunLeft };
				enemy->AddComponent<EnemyButterfly>(start,30);
				enemy->AddComponent<dae::RenderComponent>("Butterflies.png", src, dst);
			}
			if (row == 2)
			{
				Enemy::EnemyStart start{ readFile.m_Info.gridPos[row][col], Enemy::AnimationPath::leftBottom, Enemy::AnimationPath::bomingRunLeft };
				enemy->AddComponent<EnemyButterfly>(start,30);
				enemy->AddComponent<dae::RenderComponent>("Butterflies.png", src, dst);
			}
			if (row == 3)
			{
				Enemy::EnemyStart start{ readFile.m_Info.gridPos[row][col], Enemy::AnimationPath::rightBottom, Enemy::AnimationPath::diveLeft };
				enemy->AddComponent<EnemyBee>(start,50);
				enemy->AddComponent<dae::RenderComponent>("Bees.png", src, dst);
			}
			if (row == 4)
			{
				Enemy::EnemyStart start{ readFile.m_Info.gridPos[row][col], Enemy::AnimationPath::leftBottom, Enemy::AnimationPath::diveRight };
				enemy->AddComponent<EnemyBee>(start,50);
				enemy->AddComponent<dae::RenderComponent>("Bees.png", src, dst);
			}
			
			enemy->AddComponent<dae::SpriteAnimation>(2, 30, 0);
			enemy->AddComponent<EnemyCollisionComponent>(dst);
			m_EnemyComps.emplace_back(enemy->GetComponent<Enemy>());
			scene.AddCollisionObject(enemy.get());
			m_EnemyGos.emplace_back(std::move(enemy));
		}
	}
}
void EnemySpawning::Update()
{
	for (int index{}; index < m_EnemyGos.size(); ++index)
	{
		if (m_EnemyGos[index] == nullptr)continue;
		if (index == 0)
		{
			m_EnemyGos[index]->Update();
			continue;
		}
		if(m_EnemyComps[index-1]->m_Frame >= 7)
			m_EnemyGos[index]->Update();
		if (rand() % 5000 == 1)
		{
			if(m_EnemyGos[index]->HasComponent<EnemyBee>())
				m_EnemyComps[index]->SetState(m_DiveStateBee.get());
			if (m_EnemyGos[index]->HasComponent<EnemyButterfly>())
				m_EnemyComps[index]->SetState(m_DiveStateButterfly.get());
		}
	}
}

void EnemySpawning::Render() const
{
	for (auto& enemy : m_EnemyGos)
	{
		enemy->Render();
	}
}