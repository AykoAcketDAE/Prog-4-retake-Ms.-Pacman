#pragma once

#include <string>
#include "Texture2D.h"
#include "BaseComponent.h"
struct SDL_Texture;


namespace dae {
	
	class BaseComponent;
	class GameObject;
	class RenderComponent final: public BaseComponent
	{
	public:
		RenderComponent(GameObject* owner, const std::string& fullPath);
		RenderComponent(GameObject* owner, const std::string& fullPath, const SDL_FRect& SrcBounds, const SDL_FRect& DestBounds);

		void Update()override;
		void Render()const override;

		void SetTexture(const std::string& fullPath);
		void SetPosition(float x, float y) { xPos = x; yPos = y; };
		void SetSourceRect(SDL_FRect src) { m_SrcBounds = src; };
		SDL_FRect GetSourceRect() const { return m_SrcBounds; };
		void SetDestRect(SDL_FRect dst) { m_DestBounds = dst; };
		SDL_FRect GetDestRect() const { return m_DestBounds; };
		void SetAngle(float angle) { m_Angle = angle; };
		//float GetAngle() const { return m_Angle; };
		void SetCenter(SDL_Point center) { m_Center = center; };
		//SDL_Point GetCenter() const { return m_Center; };
		
	private:

		bool m_HasBounds{false};
		std::string m_Path{};
		float xPos{};
		float yPos{};
		SDL_FRect m_SrcBounds{};
		SDL_FRect m_DestBounds{};
		float m_Angle{};
		SDL_Point m_Center{};
		
		std::shared_ptr<Texture2D> m_Texture{};
	};
}


