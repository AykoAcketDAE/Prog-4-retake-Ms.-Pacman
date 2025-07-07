#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "EnemyMath.h"
#include <memory>
#include <iostream>
#include "Enemy.h"


class ButterflyState;
class ButterflyIdle;
class ButterflyDive;
class Enemy;

static const std::unique_ptr<ButterflyIdle> m_IdleStateButterfly = std::make_unique<ButterflyIdle>();
static const std::unique_ptr<ButterflyDive> m_DiveStateButterfly = std::make_unique<ButterflyDive>();

class EnemyButterfly : public Enemy
{

public:
	EnemyButterfly(dae::GameObject* owner, EnemyStart enemyStart, int speed);
	virtual void Update() override;
	virtual void Render() const override;
	virtual void SetState(BaseEnemyState* state)override;

private:
	BaseEnemyState* m_State{ nullptr };
};

class ButterflyState :public BaseEnemyState
{
public:
	virtual BaseEnemyState* Handle(Enemy&, dae::GameObject*) = 0;
	virtual void OnEnter(Enemy&, dae::GameObject*) = 0;
	virtual void OnExit(Enemy&, dae::GameObject*) = 0;
};

class ButterflyIdle : public ButterflyState
{
public:
	BaseEnemyState* Handle(Enemy& butterfly, dae::GameObject* owner) override;
	void OnEnter(Enemy& butterfly, dae::GameObject* owner) override;
	void OnExit(Enemy& butterfly, dae::GameObject* owner) override;
};

class ButterflyDive : public ButterflyState
{
public:

	BaseEnemyState* Handle(Enemy& butterfly, dae::GameObject* owner) override;
	void OnEnter(Enemy& butterfly, dae::GameObject* owner) override;
	void OnExit(Enemy& butterfly, dae::GameObject* owner) override;
};