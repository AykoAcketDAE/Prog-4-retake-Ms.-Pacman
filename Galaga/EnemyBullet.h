#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
class EnemyBullet : public dae::BaseComponent
{
public:
	EnemyBullet(dae::GameObject* owner,glm::vec3 pos,glm::vec2 direction)
		: dae::BaseComponent(owner), m_Direction{ direction.x,direction.y }{
		GetOwner()->SetLocalPosition(pos);
		m_HitBox.x = GetOwner()->GetComponent<dae::RenderComponent>()->GetDestRect().x;
		m_HitBox.y = GetOwner()->GetComponent<dae::RenderComponent>()->GetDestRect().y;
		m_HitBox.w = GetOwner()->GetComponent<dae::RenderComponent>()->GetDestRect().w;
		m_HitBox.h = GetOwner()->GetComponent<dae::RenderComponent>()->GetDestRect().h;
	}
	void Update() override;
	void Render() const override;
	SDL_FRect GetHitBox();
private:

	float m_Time{};
	const float m_LIFETIME{ 10 };
	SDL_FRect m_HitBox{ 4.f,6.f,3.f,8.f };
	glm::vec2 m_Direction{};
};