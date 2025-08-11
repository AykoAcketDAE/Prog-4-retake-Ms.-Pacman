#include "GhostStates.h"
#include "Ghost.h"
#include <iostream>
void GhostScatter::Update(Ghost& ghost)
{
	m_Time += dae::Time::GetInstance().GetDeltaTime();
	if (m_MaxTime < m_Time)
	{
		ghost.SetState(ghost.m_ChaseState.get());
		return;
	}
	ghost.SetTargetLocation(ghost.m_GhostInfo.CornerPos);
	ghost.Movement();

}

void GhostScatter::OnEnter(Ghost& ghost)
{
	m_Time = 0;
	ghost.SetTargetLocation(ghost.m_GhostInfo.CornerPos);
	ghost.FindClosestNode(ghost.m_GhostInfo.pos, false);

	std::cout << "scatter state\n";

}

void GhostScatter::OnExit(Ghost& )
{
	//ghost.FindClosestNode(ghost.m_GhostInfo.pos, false);
}

void GhostChase::Update(Ghost& ghost)
{
	m_Time += dae::Time::GetInstance().GetDeltaTime();
	if (m_MaxTime < m_Time)
	{
		ghost.SetState(ghost.m_ScatterState.get());
		return;
	}
	ghost.SetTargetLocation(ghost.m_MsPacman->gridPos);
	ghost.Movement();
}

void GhostChase::OnEnter(Ghost& ghost)
{
	ghost.SetTargetLocation(ghost.m_MsPacman->gridPos);
	m_Time = 0;
	std::cout << "chase state\n";

}

void GhostChase::OnExit(Ghost& )
{
	//ghost.FindClosestNode(ghost.m_GhostInfo.pos, false);
}

void GhostSpawn::Update(Ghost& ghost)
{
	m_Time += dae::Time::GetInstance().GetDeltaTime();
	if (m_MaxTime < m_Time)
	{
		ghost.SetState(ghost.m_ChaseState.get());
		return;
	}
}

void GhostSpawn::OnEnter(Ghost& ghost)
{
	if (m_StartPos == glm::vec2{ 0,0 })
		m_StartPos = ghost.m_GhostInfo.pos;
	ghost.SetGhostLocation(m_StartPos);
	m_Time = 0;
	std::cout << "spawn state\n";
}

void GhostSpawn::OnExit(Ghost& ghost)
{
	ghost.SetGhostLocation(m_EndPos);
	ghost.FindClosestNode(ghost.m_GhostInfo.pos, true);

}
