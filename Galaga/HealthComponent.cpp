#include "HealthComponent.h"
#include "Timer.h"
HealthComponent::HealthComponent(dae::GameObject* owner, int lives)
	:BaseComponent(owner),m_Lives{lives}
{
	m_Image = GetOwner()->GetComponent<dae::RenderComponent>();
}

void HealthComponent::Update()
{
	if (m_LostLife)
	{
		m_GraceTime += dae::Time::GetInstance().GetDeltaTime();
		if (m_GraceTime >= m_MAXGRACE)m_LostLife = false;
	}
}

void HealthComponent::Render() const
{

}

void HealthComponent::LostLife()
{
	--m_Lives;
	m_LostLife = true;
	if(m_Lives >= 1)
	{
		auto src = m_Image->GetSourceRect();
		src.w = src.w * ((m_Lives - 1.f) / m_Lives);
		m_Image->SetSourceRect(src);

		auto dst = m_Image->GetDestRect();
		dst.w = dst.w * ((m_Lives - 1.f) / m_Lives);
		m_Image->SetDestRect(dst);
	}
}
