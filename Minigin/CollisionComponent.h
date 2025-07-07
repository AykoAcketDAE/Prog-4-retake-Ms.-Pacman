#pragma once

#include "BaseComponent.h"
#include "GameObject.h"

namespace dae
{
	class CollisionComponent : public BaseComponent
	{
	public:
		CollisionComponent(GameObject* owner,const SDL_FRect& bounds);

		virtual void Update() =0;
		virtual void Render() const =0;

		virtual void OnEnter(GameObject* go) =0;
		virtual void WhileColliding(GameObject* go) =0;

	protected:
		SDL_FRect m_Hitbox{};
	};
}


