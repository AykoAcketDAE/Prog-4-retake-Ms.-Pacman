#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "RenderComponent.h"
namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, float angle, SDL_Point center) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, float angle, SDL_Point center) const;
		void RenderTexture(const Texture2D& texture, SDL_FRect sourceRect, SDL_FRect DestRect,float angle,SDL_Point center);
		void RenderSquare(int x, int y, int width, SDL_Color color);
		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

