#include "HealthComponent.h"
#include "Scene.h"
#include "Timer.h"
#include "MsPacman.h"
int HealthComponent::m_Lives = 3;

HealthComponent::HealthComponent(dae::GameObject* owner, int )
	:BaseComponent(owner)
{
	m_Image = GetOwner()->GetComponent<dae::RenderComponent>();
	auto src = m_Image->GetSourceRect();
	src.w = src.w * ((m_Lives ) / m_Lives);
	m_Image->SetSourceRect(src);

	auto dst = m_Image->GetDestRect();
	dst.w = dst.w * ((m_Lives ) / m_Lives);
	m_Image->SetDestRect(dst);
}

void HealthComponent::Update()
{
	if (m_LostLife)
	{
		m_GraceTime += dae::Time::GetInstance().GetDeltaTime();
		if (m_GraceTime >= m_MAXGRACE)
		{
			m_LostLife = false;
			m_GraceTime = 0;
		}
	}
}

void HealthComponent::Render() const
{

}

void HealthComponent::LostLife()
{
	if (m_GraceTime != 0) return;
	
	m_LostLife = true;
	if (m_Lives >= 1)
	{
		auto src = m_Image->GetSourceRect();
		src.w = src.w * ((m_Lives - 1.f) / m_Lives);
		m_Image->SetSourceRect(src);

		auto dst = m_Image->GetDestRect();
		dst.w = dst.w * ((m_Lives - 1.f) / m_Lives);
		m_Image->SetDestRect(dst);
	}
	else if (m_Lives == 0)
	{
		LoadHighScore();
		dae::SceneManager::GetInstance().SetNextScene("HighScore");
	}

	--m_Lives;
}