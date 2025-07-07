#include "Player.h"
#include <iostream>
#include "Bullet.h"
#include "Timer.h"
#include "RenderComponent.h"
#include <SDL.h>
#include "Renderer.h"
PlayerComponent::PlayerComponent(dae::GameObject* owner)
	:BaseComponent(owner)
{
    m_HitBox.x = GetOwner()->GetComponent<dae::RenderComponent>()->GetDestRect().x-24;
    m_HitBox.y = GetOwner()->GetComponent<dae::RenderComponent>()->GetDestRect().y+24;
    m_HitBox.w = GetOwner()->GetComponent<dae::RenderComponent>()->GetDestRect().w;
    m_HitBox.h = GetOwner()->GetComponent<dae::RenderComponent>()->GetDestRect().h;
}

void PlayerComponent::Update()
{
    if (m_Stuck) 
    {
        GetOwner()->SetLocalPosition(m_StuckPos);
        return;
    }
	m_Cooldown += dae::Time::GetInstance().GetDeltaTime();
	
	if (GetOwner()->GetPosition().x > 550) GetOwner()->SetLocalPosition({ 550.f ,GetOwner()->GetPosition().y ,0.f });
	if (GetOwner()->GetPosition().x < 20) GetOwner()->SetLocalPosition({ 20.f ,GetOwner()->GetPosition().y ,0.f });
    //GetOwner()->SetBounds(static_cast<SDL_FRect>(m_HitBox.x + GetOwner()->GetPosition().x+12, m_HitBox.y + GetOwner()->GetPosition().y+24, m_HitBox.w-12, m_HitBox.h-24));
    
}

void PlayerComponent::Render() const
{
    //SDL_FRect temp = { m_HitBox.x + GetOwner()->GetPosition().x + 12, m_HitBox.y + GetOwner()->GetPosition().y + 24, m_HitBox.w - 12, m_HitBox.h - 24 };
    //SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
    //SDL_RenderDrawRectF(dae::Renderer::GetInstance().GetSDLRenderer(), &temp);
}

void PlayerComponent::SpawnBullet()
{
   
    if (m_Cooldown >= m_MaxCoolDown)
    {
        m_Cooldown = 0;
        
        int activeBullets = 0;
        for (dae::GameObject* go : GetOwner()->GetChildren())
        {
            if (go->GetComponent<Bullet>()->GetIsFired())
            {
                activeBullets++;
            }
        }
        
        if (activeBullets < 2)
        {
            for (dae::GameObject* go : GetOwner()->GetChildren())
            {
                if (!go->GetComponent<Bullet>()->GetIsFired())
                {
                    go->GetComponent<Bullet>()->SpawnBullet();
                    break;
                }
            }
        }
    }
}
