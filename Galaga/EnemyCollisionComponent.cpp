#include "EnemyCollisionComponent.h"
#include "RenderComponent.h"
#include "Player.h"
#include "Bullet.h"
#include "SpriteAnimation.h"
#include "Enemy.h"
#include "EnemyBee.h"
#include "EnemyButterfly.h"
#include "SoundSystem.h"
#include "Boss.h"

EnemyCollisionComponent::EnemyCollisionComponent(dae::GameObject* owner, SDL_FRect bounds) :
	CollisionComponent(owner, bounds)
{

}

void EnemyCollisionComponent::Update()
{
	GetOwner()->SetBounds(m_Hitbox);
}

void EnemyCollisionComponent::Render() const
{
	
}

void EnemyCollisionComponent::OnEnter(dae::GameObject* go)
{
	if ((go->HasComponent<PlayerComponent>() or go->HasComponent<Bullet>()) and GetOwner()->HasComponent<Boss>() and !GetOwner()->GetComponent<Boss>()->m_Hit)
	{
		GetOwner()->GetComponent<dae::RenderComponent>()->SetSourceRect({ 0.f,16.f,16.f,16.f });
		GetOwner()->GetComponent<Boss>()->m_Hit = true;
	}
	else if (go->HasComponent<Bullet>() and go->HasComponent<BulletCollisionComponent>())
	{
		if(go->GetComponent<Bullet>()->GetIsFired())
		{
			if (GetOwner()->HasComponent<EnemyBee>())
			{
				if (dae::servicelocator::GetSoundSystem().IsLoaded(dae::make_sdbm_hash("BeeDead")))
					dae::servicelocator::GetSoundSystem().Play(dae::make_sdbm_hash("BeeDead"), 25, 0);
			}
			if (GetOwner()->HasComponent<EnemyButterfly>())
			{
				if(dae::servicelocator::GetSoundSystem().IsLoaded(dae::make_sdbm_hash("ButterflyDead")))
					dae::servicelocator::GetSoundSystem().Play(dae::make_sdbm_hash("ButterflyDead"), 25, 0);
			}

			GetOwner()->GetComponent<Enemy>()->m_Alive = false;
			GetOwner()->GetComponent<dae::RenderComponent>()->SetTexture("Explosion.png");
			GetOwner()->GetComponent<dae::RenderComponent>()->SetSourceRect({ 0.f,0.f,32.f,32.f });
			GetOwner()->GetComponent<dae::RenderComponent>()->SetDestRect({ 0.f,0.f,32.f,32.f });
			GetOwner()->GetComponent<dae::SpriteAnimation>()->ResetAnimation(6, 5, 1);
			GetOwner()->GetComponent<dae::SpriteAnimation>()->UntilDeath();
			go->GetComponent<Bullet>()->SetIsFiredOff();
			GetOwner()->RemoveComponent<EnemyCollisionComponent>();
		}

	}
	if (go->HasComponent<PlayerComponent>())
	{
		//GetOwner()->GetComponent<dae::RenderComponent>()->SetSourceRect({ 0.f,0.f,0.f,0.f });
		//go->MarkForDestruction();
	}
}


void EnemyCollisionComponent::WhileColliding(dae::GameObject* go)
{
	
	if (go->HasComponent<Bullet>())
	{
		//GetOwner()->GetComponent<dae::RenderComponent>()->SetSourceRect({ 0.f,0.f,0.f,0.f });
	}

}