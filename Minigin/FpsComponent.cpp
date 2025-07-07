#include "FpsComponent.h"
#include "Time.h"
#include <iomanip>
#include <sstream>
#include <string>

void FpsComponent::Update()
{
	if (m_TextPtr == nullptr) m_TextPtr = GetOwner()->GetComponent<dae::TextComponent>();

	m_Fps = dae::Time::GetInstance().GetFps();

	std::stringstream ss{};

	ss  << std::setprecision(2) << m_Fps ;

	m_TextPtr->SetText(ss.str() + " fps");
}
