#include "CollisionComponent.h"

dae::CollisionComponent::CollisionComponent(GameObject* owner, const SDL_FRect& bounds)
	:BaseComponent(owner),m_Hitbox{bounds}
{
	GetOwner()->SetBounds(m_Hitbox);
}


