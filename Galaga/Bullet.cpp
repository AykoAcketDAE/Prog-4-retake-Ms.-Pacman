#include "Bullet.h"
#include "Timer.h"
#include "SoundSystem.h"
#include "Player.h"
#include "EnemyCollisionComponent.h"
#include "EnemyBullet.h"
#include "EnemyBee.h"
#include "EnemyButterfly.h"

void Bullet::Update()
{
	m_Offset = { GetOwner()->GetPosition().x - m_FirePos,0.f,0.f};
	if (m_IsFired)
	{
		m_Position.y -= 500 * dae::Time::GetInstance().GetDeltaTime();
		//glm::vec3 temp = GetOwner()->GetLocalTransform().Translate(GetOwner()->GetPosition(), m_Offset);
		GetOwner()->SetLocalPosition({ m_Position.x - GetOwner()->GetParent()->GetPosition().x,m_Position.y-GetOwner()->GetParent()->GetPosition().y,0});
	
		GetOwner()->SetBounds(GetHitBox());
		//m_IsFired = true;
	}
	else {
		GetOwner()->SetLocalPosition({ 20,0,0 });
	}
	if (GetOwner()->GetPosition().y <= -800)
	{
		SetIsFiredOff();
	}
}

void Bullet::Render() const
{
}

void Bullet::SpawnBullet()
{
	//GetOwner()->SetPosition(GetOwner()->GetParent()->GetPosition().x, GetOwner()->GetParent()->GetPosition().y);

	m_FirePos = GetOwner()->GetParent()->GetPosition().x;
	m_Position.x = GetOwner()->GetParent()->GetPosition().x+20;
	m_Position.y = GetOwner()->GetParent()->GetPosition().y;
	m_IsFired = true;
	
	dae::servicelocator::GetSoundSystem().Play(dae::make_sdbm_hash("PlayerShoot"), 50, 0);
}

SDL_FRect Bullet::GetHitBox()
{
	return static_cast<SDL_FRect>(m_HitBox.x + m_Position.x,m_HitBox.y + m_Position.y,m_HitBox.w, m_HitBox.h);
}

void Bullet::SetIsFiredOff()
{
	m_IsFired = false;
	m_Position.y = 0;
	GetOwner()->SetLocalPosition(GetOwner()->GetLocalTransform().Translate(m_Position, m_Offset));
}

BulletCollisionComponent::BulletCollisionComponent(dae::GameObject* owner, SDL_FRect bounds)
	:dae::CollisionComponent(owner,bounds)
{

}

void BulletCollisionComponent::Update()
{
	GetOwner()->SetBounds(m_Hitbox);
}

void BulletCollisionComponent::OnEnter(dae::GameObject* go)
{
	if (go->HasComponent<PlayerComponent>() and GetOwner()->HasComponent<EnemyBullet>())
	{
		GetOwner()->RemoveComponent<EnemyBullet>();
		GetOwner()->RemoveComponent<BulletCollisionComponent>();
	}
	if (go->HasComponent<EnemyBullet>())
	{
		//std::cout << "hit\n";
	}
	
	
}

void BulletCollisionComponent::WhileColliding(dae::GameObject* )
{
	//if (go->HasComponent<EnemyCollisionComponent>())
	//{
	//	GetOwner()->GetComponent<Bullet>()->SetIsFiredOff();
	//}
}
