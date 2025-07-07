#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "EnemyMath.h"
#include <memory>
#include <iostream>
class BaseEnemyState;
class EnemyBee;
class BeeState;
class BeeIdle;
class BeeDive;

class Enemy: public dae::BaseComponent
{
public:
	enum class AnimationState
	{
		idle,attacking,returning,beam
	};
	enum class AnimationPath
	{
		leftTop, rightTop, leftBottom, rightBottom,diveLeft, diveRight,bomingRunLeft, bomingRunRight,BossLeft,BossRight
	};

	struct EnemyStart {
		glm::vec2 Gridpos;
		AnimationPath spawnAnimation;
		AnimationPath attackAnimation;
	};

	Enemy(dae::GameObject* owner, glm::vec2 Gridpos, AnimationPath spawnAnimation, AnimationPath attackAnimation, int speed);

	virtual void Update();
	virtual void Render() const;

	virtual void SetState(BaseEnemyState*) {};

	void UpdatePosition();
	void SetAnimationState(AnimationState state) { 
		m_AnimationState = state; 
	}

	bool m_Alive{ true };
	int m_Frame{};
protected:

	glm::fvec3 Lerp(glm::vec2 startPos, glm::vec2 endPos, float t)
	{
		return { std::lerp(startPos.x,endPos.x,t), std::lerp(startPos.y, endPos.y,t),0.f };
	};
	EnemyStart m_StartingVars;
	std::vector<std::vector<std::pair<glm::fvec2,float>>> m_PointPath;
	AnimationState m_AnimationState{AnimationState::idle};
	
private:
	static const float m_FLYINTIME;
	static float m_Movement;
	const float m_OFFSETPOS{125.f};
	static bool m_MovementInvert;
};

class BaseEnemyState
{
public:
	virtual BaseEnemyState* Handle(Enemy&, dae::GameObject* ) =0;
	virtual ~BaseEnemyState() = default;

	virtual void OnEnter(Enemy&, dae::GameObject* ) = 0;
	virtual void OnExit(Enemy&, dae::GameObject* ) = 0;
};