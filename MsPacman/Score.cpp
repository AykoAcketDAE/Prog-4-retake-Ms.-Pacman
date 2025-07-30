#include "Score.h"

Score::Score(dae::GameObject* owner)
	:BaseComponent(owner)
{
	m_TextComp = GetOwner()->GetComponent<dae::TextComponent>();
}

void Score::Update()
{
	m_TextComp->SetText(std::to_string(m_Score));
}

void Score::Render() const
{

}
