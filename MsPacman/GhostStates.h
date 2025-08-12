#pragma once
#include "Timer.h"
#include "Ghost.h"
class Ghost;
class GhostState
{
public:
	virtual ~GhostState() = default;
	virtual void Update(Ghost& ) = 0;
	virtual void OnEnter(Ghost& ) = 0;
	virtual void OnExit(Ghost& ) = 0;
};

class GhostScatter : public GhostState
{
public:
	void Update(Ghost& ghost) override;
	void OnEnter(Ghost& ghost) override;
	void OnExit(Ghost& ghost) override;
private:
	const float m_MaxTime{ 7.f };
	float m_Time{};
};
class GhostChase : public GhostState
{
public:
	void Update(Ghost& ghost) override;
	void OnEnter(Ghost& ghost) override;
	void OnExit(Ghost& ghost) override;
private:
	const float m_MaxTime{ 20.f };
	float m_Time{};
};
class GhostSpawn : public GhostState
{
public:
	void Update(Ghost& ghost) override;
	void OnEnter(Ghost& ghost) override;
	void OnExit(Ghost& ghost) override;
private:
	const float m_MaxTime{ 5.f };
	float m_Time{};
	glm::vec2 m_StartPos{};
	glm::vec2 m_EndPos{(rand()%5) + 10,11};
};

