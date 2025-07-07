#include "EnemyBullet.h"
#include "Timer.h"
#include "Renderer.h"
#include "SDL.h"
#include <iostream>
void EnemyBullet::Update()
{
	//if(GetOwner()->GetPosition()) 
	m_Time += dae::Time::GetInstance().GetDeltaTime();
	if (m_Time >= m_LIFETIME) GetOwner()->MarkForDestruction();
	GetOwner()->SetLocalPosition({ 
		GetOwner()->GetPosition().x + m_Direction.x * dae::Time::GetInstance().GetDeltaTime(),
		GetOwner()->GetPosition().y + m_Direction.y * dae::Time::GetInstance().GetDeltaTime(),
		0.f});

	GetOwner()->SetBounds(GetHitBox());
	
}

void EnemyBullet::Render() const
{
	//SDL_FRect temp = { GetOwner()->GetPosition().x , m_HitBox.y - GetOwner()->GetPosition().y , m_HitBox.w, m_HitBox.h };
	//SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
	//SDL_RenderDrawRectF(dae::Renderer::GetInstance().GetSDLRenderer(), &m_HitBox);
}

SDL_FRect EnemyBullet::GetHitBox()
{
	return static_cast<SDL_FRect>(m_HitBox.x + GetOwner()->GetPosition().x, m_HitBox.y + GetOwner()->GetPosition().y, m_HitBox.w, m_HitBox.h);
}