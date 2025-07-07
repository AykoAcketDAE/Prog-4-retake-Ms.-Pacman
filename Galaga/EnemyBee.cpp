#include "EnemyBee.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EnemyBullet.h"
#include "Bullet.h"
EnemyBee::EnemyBee(dae::GameObject* owner, Enemy::EnemyStart enemyStart, int speed)
	:Enemy(owner,enemyStart.Gridpos,enemyStart.spawnAnimation,enemyStart.attackAnimation,speed)
{
	SetState(m_IdleStateBee.get());
}

void EnemyBee::Update()
{
	if (m_State)
	{
		m_State = m_State->Handle(*this,GetOwner());
	}
}



void EnemyBee::SetState(BaseEnemyState* state)
{
	if(m_State)m_State->OnExit(*this, GetOwner());
	m_State = state;
	if (m_State)m_State->OnEnter(*this, GetOwner());
}

BaseEnemyState* BeeIdle::Handle(Enemy& bee,dae::GameObject*)
{
	bee.UpdatePosition();
	bee.SetAnimationState(Enemy::AnimationState::idle);
	return m_IdleStateBee.get();
}

void BeeIdle::OnEnter(Enemy& bee, dae::GameObject* )
{
	bee.SetAnimationState(Enemy::AnimationState::idle);
}

void BeeIdle::OnExit(Enemy& , dae::GameObject* )
{
	
}

BaseEnemyState* BeeDive::Handle(Enemy& bee, dae::GameObject* owner)
{
	bee.UpdatePosition();
	if (rand() % 300 == 1 and bee.m_Alive)
	{
		auto scene = dae::SceneManager::GetInstance().GetCurrentScene();
		std::unique_ptr<dae::GameObject> bullet1 = std::make_unique<dae::GameObject>();
		std::unique_ptr<dae::GameObject> bullet2 = std::make_unique<dae::GameObject>();
		std::unique_ptr<dae::GameObject> bullet3 = std::make_unique<dae::GameObject>();
		
		
		SDL_FRect src = { 0.f,12.f,3.f,12.f };
		SDL_FRect dst = { 24.f, 24.f, 9.f,24.f };
		bullet1->AddComponent<dae::RenderComponent>("Bullets.png", src, dst);
		bullet2->AddComponent<dae::RenderComponent>("Bullets.png", src, dst);
		bullet3->AddComponent<dae::RenderComponent>("Bullets.png", src, dst);
		glm::vec2 dir1{ 10.f, 200.f };
		glm::vec2 dir2{ 0.f, 200.f };
		glm::vec2 dir3{ -10.f, 200.f };
		glm::vec3 pos = { owner->GetPosition().x,owner->GetPosition().y - 75,0 };
		bullet1->AddComponent<EnemyBullet>(pos, dir1);
		bullet2->AddComponent<EnemyBullet>(pos, dir2);
		bullet3->AddComponent<EnemyBullet>(pos, dir3);
		SDL_FRect HitBox{ 0.f,0.f,9.f,24.f };
		bullet1->AddComponent<BulletCollisionComponent>(HitBox);
		bullet2->AddComponent<BulletCollisionComponent>(HitBox);
		bullet3->AddComponent<BulletCollisionComponent>(HitBox);
		scene->Add(std::move(bullet1));
		scene->Add(std::move(bullet2));
		scene->Add(std::move(bullet3));
	}
	//bee.SetAnimationState(Enemy::AnimationState::attacking);
	return m_DiveStateBee.get();
}

void BeeDive::OnEnter(Enemy& bee, dae::GameObject* )
{
	bee.SetAnimationState(Enemy::AnimationState::attacking);
	bee.m_Frame = 0;
}

void BeeDive::OnExit(Enemy& bee, dae::GameObject* owner)
{
	bee.SetAnimationState(Enemy::AnimationState::idle);
	owner->SetLocalPosition({ 300,-200,0.f });
	bee.UpdatePosition();
}