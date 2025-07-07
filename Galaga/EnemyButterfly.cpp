#include "EnemyButterfly.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EnemyBullet.h"
#include "Bullet.h"
EnemyButterfly::EnemyButterfly(dae::GameObject* owner, EnemyStart enemyStart, int speed)
	:Enemy(owner, enemyStart.Gridpos, enemyStart.spawnAnimation, enemyStart.attackAnimation,speed)
{
	SetState(m_IdleStateButterfly.get());
}

void EnemyButterfly::Update()
{
	if (m_State)
	{
		m_State = m_State->Handle(*this, GetOwner());
	}
}

void EnemyButterfly::Render() const
{

}

void EnemyButterfly::SetState(BaseEnemyState* state)
{
	if (m_State)m_State->OnExit(*this, GetOwner());
	m_State = state;
	if (m_State)m_State->OnEnter(*this, GetOwner());
}

BaseEnemyState* ButterflyIdle::Handle(Enemy& butterfly, dae::GameObject*)
{
	butterfly.UpdatePosition();
	butterfly.SetAnimationState(Enemy::AnimationState::idle);
	return m_IdleStateButterfly.get();
}

void ButterflyIdle::OnEnter(Enemy& butterfly, dae::GameObject*)
{
	butterfly.SetAnimationState(Enemy::AnimationState::idle);
	butterfly.m_Frame = 0;
}

void ButterflyIdle::OnExit(Enemy&, dae::GameObject*)
{

}

BaseEnemyState* ButterflyDive::Handle(Enemy& butterfly, dae::GameObject* owner)
{
	butterfly.UpdatePosition();
	if (rand() % 300 == 1 and butterfly.m_Alive)
	{
		auto scene = dae::SceneManager::GetInstance().GetCurrentScene();
		std::unique_ptr<dae::GameObject> bullet1 = std::make_unique<dae::GameObject>();
		std::unique_ptr<dae::GameObject> bullet2 = std::make_unique<dae::GameObject>();
		std::unique_ptr<dae::GameObject> bullet3 = std::make_unique<dae::GameObject>();
		
		
		SDL_FRect src = { 0.f,12.f,3.f,12.f };
		SDL_FRect dst = { 12.f, 12.f, 9.f,24.f };
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
		bullet1->AddComponent<BulletCollisionComponent>(bullet1->GetComponent<EnemyBullet>()->GetHitBox());
		bullet2->AddComponent<BulletCollisionComponent>(bullet2->GetComponent<EnemyBullet>()->GetHitBox());
		bullet3->AddComponent<BulletCollisionComponent>(bullet3->GetComponent<EnemyBullet>()->GetHitBox());
		scene->Add(std::move(bullet1));
		scene->Add(std::move(bullet2));
		scene->Add(std::move(bullet3));
	}
	//bee.SetAnimationState(Enemy::AnimationState::attacking);
	return m_DiveStateButterfly.get();
}

void ButterflyDive::OnEnter(Enemy& butterfly, dae::GameObject*)
{
	butterfly.SetAnimationState(Enemy::AnimationState::attacking);
	butterfly.m_Frame =0;
}

void ButterflyDive::OnExit(Enemy& butterfly, dae::GameObject* owner)
{
	butterfly.SetAnimationState(Enemy::AnimationState::idle);
	owner->SetLocalPosition({ 300,-200,0.f });
	butterfly.UpdatePosition();
}