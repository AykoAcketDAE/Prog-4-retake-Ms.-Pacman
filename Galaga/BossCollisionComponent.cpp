#include "BossCollisionComponent.h"
#include "Bullet.h"
#include "RenderComponent.h"
#include "Player.h"
#include <iostream>
BossCollisionComponent::BossCollisionComponent(dae::GameObject* owner, SDL_FRect bounds):
	CollisionComponent(owner,bounds)
{

}

void BossCollisionComponent::Update()
{
	GetOwner()->SetBounds(m_Hitbox);
	std::cout << "pos: " << GetOwner()->GetPosition().x << ", " << GetOwner()->GetPosition().y << std::endl;
	std::cout << "Hitbox: x " << GetOwner()->GetBounds()->x << ", " << GetOwner()->GetBounds()->y << std::endl;
	std::cout << "Hitbox: w " << GetOwner()->GetBounds()->w << ", " << GetOwner()->GetBounds()->h << std::endl;
}

void BossCollisionComponent::Render() const
{

}

void BossCollisionComponent::OnEnter(dae::GameObject* go)
{
	if (go->HasComponent<PlayerComponent>())
		std::cout << "hallo111111111111111\n";
}


void BossCollisionComponent::WhileColliding(dae::GameObject* go )
{
	if (go->HasComponent<PlayerComponent>())
		std::cout<<"hallo       222222222222  2\n";
}
