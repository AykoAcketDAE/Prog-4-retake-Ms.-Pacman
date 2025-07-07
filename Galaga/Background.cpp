#include "Background.h"
#include "Timer.h"

Background::Background(dae::GameObject* owner):
	BaseComponent(owner)
{
	
}

void Background::Update()
{
	m_Height = m_Height - dae::Time::GetInstance().GetDeltaTime() * 200;
	if (m_Height <= -720.f) m_Height = 0.f;
	GetOwner()->SetLocalPosition({ 0.f,m_Height,0.f });
}

void Background::Render() const
{

}
