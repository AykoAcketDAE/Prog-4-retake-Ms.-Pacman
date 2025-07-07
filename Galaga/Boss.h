#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include <memory>
#include "Enemy.h"
#include "RenderComponent.h"
#include "BossCollisionComponent.h"
#include "SpriteAnimation.h"
#include "Timer.h"
class BossState;
class BossIdle;
class BossAttack;
class BossBeam;
class BossAfterBeam;

static const std::unique_ptr<BossIdle> m_IdleStateBoss = std::make_unique<BossIdle>();
static const std::unique_ptr<BossAttack> m_DiveStateBoss = std::make_unique<BossAttack>();
static const std::unique_ptr<BossBeam> m_BeamStateBoss = std::make_unique<BossBeam>();
static const std::unique_ptr<BossAfterBeam> m_AfterBeamStateBoss = std::make_unique<BossAfterBeam>();

class Boss : public Enemy
{
public:
	Boss(dae::GameObject* owner, EnemyStart enemyStart,int speed);

	void Update() override;
	void Render() const override;

	void SetState(BaseEnemyState* state)override;

	void SetupBeam();
	void ResetBeam()
	{
		m_Beam = std::make_unique<dae::GameObject>();
	}
	bool BeamTimer()
	{
		m_BeamTimer += dae::Time::GetInstance().GetDeltaTime();
		if (m_BeamTimer >= m_BEAMMAX)
		{
			m_BeamTimer = 0;
			m_AnimationComp->UntilDeath();
			return true;
		}
		else return false;
	};
	
	bool m_Hit{ false };
	
	
private:
	float m_BeamTimer{};
	const float m_BEAMMAX{ 4 };
	BaseEnemyState* m_BossState;
	SDL_FRect m_HitBox{};
	dae::SpriteAnimation* m_AnimationComp{ nullptr };
	std::unique_ptr<dae::GameObject>m_Beam;
};

class BossState : public BaseEnemyState
{
public:
	virtual BaseEnemyState* Handle(Enemy&, dae::GameObject*) = 0;
	virtual void OnEnter(Enemy&, dae::GameObject*) = 0;
	virtual void OnExit(Enemy&, dae::GameObject*) = 0;
};

class BossIdle: public BossState
{
public:
	BaseEnemyState* Handle(Enemy& go, dae::GameObject* owner) override;
	void OnEnter(Enemy& go, dae::GameObject* owner) override;
	void OnExit(Enemy& go, dae::GameObject* owner) override;
};

class BossAttack : public BossState
{
public:
	BaseEnemyState* Handle(Enemy& go, dae::GameObject* owner) override;
	void OnEnter(Enemy& go, dae::GameObject* owner) override;
	void OnExit(Enemy& go, dae::GameObject* owner) override;
};

class BossBeam : public BossState
{
public:
	BaseEnemyState* Handle(Enemy& go, dae::GameObject* owner) override;
	void OnEnter(Enemy& go, dae::GameObject* owner) override;
	void OnExit(Enemy& go, dae::GameObject* owner) override;
};

class BossAfterBeam : public BossState
{
public:
	BaseEnemyState* Handle(Enemy& go, dae::GameObject* owner) override;
	void OnEnter(Enemy& go, dae::GameObject* owner) override;
	void OnExit(Enemy& go, dae::GameObject* owner) override;
};