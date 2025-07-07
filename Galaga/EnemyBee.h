#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "EnemyMath.h"
#include <memory>
#include <iostream>
#include "Enemy.h"


class BeeState;
class BeeIdle;
class BeeDive;
class Enemy;

static const std::unique_ptr<BeeIdle> m_IdleStateBee = std::make_unique<BeeIdle>();
static const std::unique_ptr<BeeDive> m_DiveStateBee = std::make_unique<BeeDive>();

class EnemyBee: public Enemy
{

public:
	EnemyBee(dae::GameObject* owner, EnemyStart enemyStart, int speed);
	virtual void Update() override;
	
	virtual void SetState(BaseEnemyState* state)override;



private:
	BaseEnemyState* m_State{nullptr};
};

class BeeState:public BaseEnemyState
{
public:
	virtual BaseEnemyState* Handle(Enemy&, dae::GameObject* ) = 0;
	virtual void OnEnter(Enemy&, dae::GameObject* ) = 0;
	virtual void OnExit(Enemy&, dae::GameObject* ) = 0;
};

class BeeIdle : public BeeState
{
public:
	BaseEnemyState* Handle(Enemy& bee, dae::GameObject* owner) override;
	void OnEnter(Enemy& bee, dae::GameObject* owner) override;
	void OnExit(Enemy& bee, dae::GameObject* owner) override;
};

class BeeDive: public BeeState
{
public:
	
	BaseEnemyState* Handle(Enemy& bee, dae::GameObject* owner) override;
	void OnEnter(Enemy& bee, dae::GameObject* owner) override;
	void OnExit(Enemy& bee, dae::GameObject* owner) override;
};