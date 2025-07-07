#include "Boss.h"
#include <memory>

#include "SceneManager.h"
#include "Scene.h"
Boss::Boss(dae::GameObject* owner, EnemyStart enemyStart, int speed)
	:Enemy(owner, enemyStart.Gridpos, enemyStart.spawnAnimation, enemyStart.attackAnimation,speed)
{
	SetState(m_IdleStateBoss.get());
	m_Beam = std::make_unique<dae::GameObject>();
}

void Boss::Update()
{
	if (!GetOwner()->HasComponent<dae::RenderComponent>()) GetOwner()->MarkForDestruction();
	if (m_BossState)
	{
		m_BossState->Handle(*this,GetOwner());
	}
}

void Boss::Render() const
{

}

void Boss::SetState(BaseEnemyState* state)
{
	if (m_BossState == state) return;
	if (m_BossState)m_BossState->OnExit(*this, GetOwner());
	m_BossState = state;
	if (m_BossState)m_BossState->OnEnter(*this, GetOwner());
}

void Boss::SetupBeam()
{
	
	if(m_Beam.get() !=nullptr)
	{
		SDL_FRect src{ 0,0,48,80 };
		SDL_FRect dst{ -60,0,120,240 };
		SDL_FRect bounds{ - 60,0,120,-800 };
		m_Beam->AddComponent<dae::RenderComponent>("Beam.png", src, dst);
		m_Beam->AddComponent<dae::SpriteAnimation>(3, 5, 300);
		m_Beam->AddComponent<BossCollisionComponent>(bounds);
		m_AnimationComp = m_Beam->GetComponent<dae::SpriteAnimation>();
		m_Beam->SetParent(GetOwner(), false);
		m_Beam->SetLocalPosition({ dst.x -24,dst.y,0.f });
		dae::SceneManager::GetInstance().GetCurrentScene()->AddCollisionObject(m_Beam.get());
		dae::SceneManager::GetInstance().GetCurrentScene()->Add(std::move(m_Beam));
	}
}

BaseEnemyState* BossIdle::Handle(Enemy& boss, dae::GameObject* )
{
	boss.UpdatePosition();
	if (rand() % 6000 == 1 and boss.m_Alive == true)
	{
		boss.SetState(m_DiveStateBoss.get());
		return m_DiveStateBoss.get();
	}
	return m_IdleStateBoss.get();
}

void BossIdle::OnEnter(Enemy& boss, dae::GameObject* )
{
	boss.UpdatePosition();
	boss.SetAnimationState(Enemy::AnimationState::idle);
}

void BossIdle::OnExit(Enemy& , dae::GameObject* )
{

}

BaseEnemyState* BossAttack::Handle(Enemy& boss, dae::GameObject* )
{
	//std::cout << "Before: " << owner->GetPosition().x << ", " << owner->GetPosition().y << std::endl;
	boss.UpdatePosition();
	//std::cout << "After: " << owner->GetPosition().x << ", " << owner->GetPosition().y << std::endl;
	return m_DiveStateBoss.get();
}

void BossAttack::OnEnter(Enemy& boss, dae::GameObject* )
{
	boss.SetAnimationState(Enemy::AnimationState::attacking);
	boss.m_Frame = 0;
}

void BossAttack::OnExit(Enemy& , dae::GameObject* )
{
	
}

BaseEnemyState* BossBeam::Handle(Enemy& boss, dae::GameObject* owner)
{
	
	if (owner->GetComponent<Boss>()->BeamTimer())
	{
		boss.m_Frame += 1;
		boss.SetAnimationState(Enemy::AnimationState::attacking);
		boss.SetState(m_AfterBeamStateBoss.get());
		return m_AfterBeamStateBoss.get();
	}
	return m_BeamStateBoss.get();
}

void BossBeam::OnEnter(Enemy& boss, dae::GameObject* owner)
{
	owner->GetComponent<Boss>()->SetupBeam();
	boss.SetAnimationState(Enemy::AnimationState::beam);
}

void BossBeam::OnExit(Enemy& boss, dae::GameObject* owner)
{
	if(!owner->GetChildren().empty())
		owner->GetChildren()[0]->MarkForDestruction();
	owner->GetComponent<Boss>()->ResetBeam();
	boss.SetAnimationState(Enemy::AnimationState::attacking);
}

BaseEnemyState* BossAfterBeam::Handle(Enemy& boss, dae::GameObject* owner)
{
	boss.UpdatePosition();
	if(owner->GetPosition().y <= -800)
		boss.SetState(m_IdleStateBoss.get());
	return m_AfterBeamStateBoss.get();
}

void BossAfterBeam::OnEnter(Enemy& boss, dae::GameObject* )
{
	boss.SetAnimationState(Enemy::AnimationState::attacking);
}

void BossAfterBeam::OnExit(Enemy& boss, dae::GameObject* )
{
	boss.SetAnimationState(Enemy::AnimationState::returning);
	
}
