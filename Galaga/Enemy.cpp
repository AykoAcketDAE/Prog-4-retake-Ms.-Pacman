#include "Enemy.h"
#include "RenderComponent.h"
#include <algorithm>
#include "Timer.h"
#include "EnemyBee.h"
#include "EnemyButterfly.h"
#include <glm.hpp>
#include "Boss.h"


float Enemy::m_Movement{ 0.5f };
bool  Enemy::m_MovementInvert{ false };
const float Enemy::m_FLYINTIME{ 1.5f };
Enemy::Enemy(dae::GameObject* owner, glm::vec2 Gridpos, AnimationPath spawnAnimation, AnimationPath attackAnimation ,int speed)
	:dae::BaseComponent(owner), m_StartingVars{ Gridpos ,spawnAnimation ,attackAnimation }
{
	std::vector<BezierPath> bezierPath(2);
	ReadBezier readFile;
	
	bezierPath[0].AddCurve(readFile.m_Info.curves[static_cast<int>(m_StartingVars.spawnAnimation)], 5);
	bezierPath[1].AddCurve(readFile.m_Info.curves[static_cast<int>(m_StartingVars.attackAnimation)], speed);
	
	m_PointPath.emplace_back(bezierPath[0].Sample());
	m_PointPath.emplace_back(bezierPath[1].Sample());
}

void Enemy::Update()
{
   
}

void Enemy::Render() const
{
   
}

void Enemy::UpdatePosition()
{
    auto offset = std::lerp(-m_OFFSETPOS, m_OFFSETPOS, m_Movement) + 50;
    if (m_MovementInvert)
        m_Movement -= 0.0001f; 
    else
        m_Movement += 0.0001f;

    if (m_Movement >= 0.9f)
    {
        m_MovementInvert = true;
        m_Movement = 0.899f;
    }
    else if (m_Movement <= 0.1f)
    {
        m_MovementInvert = false;
        m_Movement = 0.111f;
    }
    if ((m_AnimationState == AnimationState::beam or m_AnimationState == AnimationState::attacking) and GetOwner()->HasComponent<Boss>()) {
        offset = 0;
    }
    if (m_AnimationState == AnimationState::returning)
    {
       
        glm::vec3 currentPosition = GetOwner()->GetPosition();
        glm::vec3 targetPosition = { m_StartingVars.Gridpos.x + offset, m_StartingVars.Gridpos.y - 75, 0.f };
        float interpolationFactor = 0.1f;
        glm::vec3 newPosition = Lerp(currentPosition, targetPosition, interpolationFactor);
        if (currentPosition.y == targetPosition.y) 
        {
            AnimationState::idle; 
            //GetOwner()->GetComponent<dae::RenderComponent>()->SetAngle(0);
            return;
        }
        GetOwner()->SetLocalPosition(newPosition);
        //BezierPath angleCalc;
        //GetOwner()->GetComponent<dae::RenderComponent>()->SetAngle(angleCalc.GetAngle(currentPosition, targetPosition));
        return;
    }
    if (m_AnimationState == AnimationState::beam)
    {
        GetOwner()->GetComponent<dae::RenderComponent>()->SetAngle(180);
        GetOwner()->SetLocalPosition({
            m_PointPath[static_cast<int>(AnimationState::attacking)][m_Frame].first.x + m_StartingVars.Gridpos.x,
            m_PointPath[static_cast<int>(AnimationState::attacking)][m_Frame].first.y + m_StartingVars.Gridpos.y - 75, 0.f });
        return;
    }
    else if (m_Frame == static_cast<int>(m_PointPath[static_cast<int>(m_AnimationState)].size() / 1.5) and m_AnimationState == AnimationState::attacking
        and GetOwner()->HasComponent<Boss>())
    {
        SetState(m_BeamStateBoss.get());
    }
    if (m_AnimationState == AnimationState::beam) return;
    
    
    else if (m_Frame < m_PointPath[static_cast<int>(m_AnimationState)].size())
    {
        if (m_AnimationState == AnimationState::idle and m_Frame < m_PointPath[static_cast<int>(m_AnimationState)].size())
        {
            GetOwner()->SetLocalPosition({ m_PointPath[static_cast<int>(m_AnimationState)][m_Frame].first.x, m_PointPath[static_cast<int>(m_AnimationState)][m_Frame].first.y - 75, 0.f });
        }
        if (m_AnimationState == AnimationState::attacking)
        {
            GetOwner()->SetLocalPosition({
                m_PointPath[static_cast<int>(m_AnimationState)][m_Frame].first.x + m_StartingVars.Gridpos.x + offset,
                m_PointPath[static_cast<int>(m_AnimationState)][m_Frame].first.y + m_StartingVars.Gridpos.y - 75, 0.f });
        }
        if (m_Frame < m_PointPath[static_cast<int>(m_AnimationState)].size())
        {
            if(GetOwner()->HasComponent<dae::RenderComponent>())
                GetOwner()->GetComponent<dae::RenderComponent>()->SetAngle(m_PointPath[static_cast<int>(m_AnimationState)][m_Frame].second);
        }
        ++m_Frame;
    }
    else if (m_Frame >= m_PointPath[static_cast<int>(m_AnimationState)].size() and m_AnimationState == AnimationState::attacking)
    {
        if (GetOwner()->HasComponent<EnemyBee>())
            SetState(m_DiveStateBee.get());
        if (GetOwner()->HasComponent<EnemyButterfly>())
            SetState(m_DiveStateButterfly.get());
    }
    if (m_Frame == m_PointPath[static_cast<int>(m_AnimationState)].size() and m_AnimationState == AnimationState::attacking)
    {
        m_AnimationState = AnimationState::returning;
        GetOwner()->SetLocalPosition({ 300,-100,0 });
    }
    else if(m_AnimationState == AnimationState::idle)
    {
        glm::vec3 currentPosition = GetOwner()->GetPosition();
        glm::vec3 targetPosition = { m_StartingVars.Gridpos.x + offset, m_StartingVars.Gridpos.y - 75, 0.f };
        float interpolationFactor = 0.1f; 
        glm::vec3 newPosition = Lerp(currentPosition, targetPosition, interpolationFactor);
        GetOwner()->SetLocalPosition(newPosition);
        

    }
}

