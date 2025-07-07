#include "PlayerCollisionComponent.h"
#include "EnemyCollisionComponent.h"
#include "SpriteAnimation.h"
#include "Player.h"
#include "RenderComponent.h"
#include "EnemyBullet.h"
#include "iostream"
#include "BossCollisionComponent.h"
#include "Boss.h"
PlayerCollisionComponent::PlayerCollisionComponent(dae::GameObject* owner, SDL_FRect bounds, dae::GameObject* healthObject)
	:CollisionComponent(owner, bounds)
{
	m_LoseLife = std::make_unique<LoseLife>(healthObject);
}

void PlayerCollisionComponent::Update()
{
	GetOwner()->SetBounds(m_Hitbox);
	
}

void PlayerCollisionComponent::Render() const
{

}

void PlayerCollisionComponent::OnEnter(dae::GameObject* go)
{
	if (go->HasComponent<EnemyCollisionComponent>())
	{
		if (go->HasComponent<Boss>())
		{
			go->GetComponent<Boss>()->m_Hit = true;
		}
		else {
			go->GetComponent<dae::RenderComponent>()->SetTexture("Explosion.png");
			go->GetComponent<dae::RenderComponent>()->SetSourceRect({ 0.f,0.f,32.f,32.f });
			go->GetComponent<dae::RenderComponent>()->SetDestRect({ 0.f,0.f,32.f,32.f });
			go->GetComponent<dae::SpriteAnimation>()->ResetAnimation(6, 5, 1);
			//go->MarkForDestruction();
			go->RemoveComponent<EnemyCollisionComponent>();
		}
		

		m_LoseLife->Execute();
	}
	if (go->HasComponent<EnemyBullet>())
	{
		m_LoseLife->Execute();
		go->GetComponent<dae::RenderComponent>()->SetTexture("Explosion.png");
		go->GetComponent<dae::RenderComponent>()->SetSourceRect({ 0.f,0.f,0.f,0.f });
		go->GetComponent<dae::RenderComponent>()->SetDestRect({ 0.f,0.f,0.f,0.f });
		go->GetComponent<dae::SpriteAnimation>()->ResetAnimation(6, 5, 1);
		go->RemoveComponent<EnemyBullet>();

		m_LoseLife->Execute();
	}
	if (go->HasComponent<BossCollisionComponent>())
	{
		m_Counter = 0;
	}
}

void PlayerCollisionComponent::WhileColliding(dae::GameObject* go)
{
	if (go->HasComponent<BossCollisionComponent>())
	{
		m_Counter++;
		std::cout << "Collided with Boss nr: " << m_Counter << std::endl;
		if (m_Counter >= 100)
		{
			m_LoseLife->Execute();
			
		}
	}
}
