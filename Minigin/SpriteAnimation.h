#pragma once
#include "GameObject.h"
#include "BaseComponent.h"
#include "RenderComponent.h"
namespace dae
{
	class SpriteAnimation :public BaseComponent
	{
	public:
		SpriteAnimation(GameObject* owner,int amountFrames, int timePerFrame,int loops);

		void Update()override;
		void Render() const override;
		void UntilDeath() { m_UntilDeath = true; };
		bool GetIsAnimating() const { 
			m_IsAnimating;
			return m_IsAnimating; 
		}
		void ResetAnimation( int amountFrames, int timePerFrame, int loops) {
			m_AmountLoops = loops;
			m_NrLoops = 0;
			m_SourceRect = m_RenderComp->GetSourceRect();
			m_FrameWidth = m_SourceRect.w;
			m_ElapsedFrames = 0;
			m_AmountFrames = amountFrames;
			m_TimePerFrame = timePerFrame;
			m_IsAnimating = true;
		}
	private:
		SDL_FRect m_SourceRect{};

		int m_AmountFrames{}; // the amount of different sprites in animation
		int m_TimePerFrame{}; // the amount of frames per image on screen
		int m_ElapsedFrames{};
		int m_ElapsedSprites{};
		float m_FrameWidth{};
		int m_AmountLoops{};
		int m_NrLoops{};

		bool m_UntilDeath{false};
		bool m_IsAnimating{true};
		RenderComponent* m_RenderComp{nullptr};

	};
}


