#include "Score.h"

int Score::m_Score = 0;

Score::Score(dae::GameObject* owner,int )
	:BaseComponent(owner)
{
	m_TextComp = GetOwner()->GetComponent<dae::TextComponent>();
	m_Score = m_Score;
	//m_TextComp->SetText(std::to_string(m_Score));
}

void Score::Update()
{
	if (m_TextComp)
		m_TextComp->SetText(std::to_string(m_Score));
}

void Score::Render() const
{

}
