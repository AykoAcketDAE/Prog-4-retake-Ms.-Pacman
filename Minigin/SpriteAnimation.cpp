#include "SpriteAnimation.h"

dae::SpriteAnimation::SpriteAnimation(GameObject* owner, int amountFrames, int timePerFrame, int loops )
	:BaseComponent(owner),
	m_AmountFrames{ amountFrames },
	m_TimePerFrame{ timePerFrame },
	m_AmountLoops{ loops }
	
{
	m_RenderComp = GetOwner()->GetComponent<RenderComponent>();
	m_SourceRect = m_RenderComp->GetSourceRect();
	m_FrameWidth = m_SourceRect.w;
}

void dae::SpriteAnimation::Update()
{
	++m_ElapsedFrames;
	if (m_ElapsedFrames == m_TimePerFrame and m_IsAnimating) 
	{
		m_ElapsedFrames = 0;
		m_SourceRect.y = m_RenderComp->GetSourceRect().y;
		m_FrameWidth = m_SourceRect.w;

		SDL_FRect temp = { m_SourceRect.x + m_ElapsedSprites* m_FrameWidth,m_SourceRect.y ,m_SourceRect.w ,m_SourceRect.h };
		m_RenderComp->SetSourceRect(temp);

		++m_ElapsedSprites;
		if (m_ElapsedSprites == m_AmountFrames) 
		{
			m_ElapsedSprites = 0;
			if (m_NrLoops++ != m_AmountLoops and m_AmountLoops != 0) m_IsAnimating = false;
		}
		
	}
	else if (m_UntilDeath)
	{
		GetOwner()->MarkForDestruction();
	}

}

void dae::SpriteAnimation::Render() const
{

}
