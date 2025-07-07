#include <stdexcept>
#include "Renderer.h"
#include "Scene.h"
#include "Texture2D.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();
	
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, float angle, SDL_Point center) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst,angle,&center,SDL_FLIP_NONE);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, float angle, SDL_Point center) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, angle, &center, SDL_FLIP_NONE);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, SDL_FRect sourceRect, SDL_FRect DestRect, float angle, SDL_Point center)
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(DestRect.x);
	dst.y = static_cast<int>(DestRect.y);
	dst.w = static_cast<int>(DestRect.w);
	dst.h = static_cast<int>(DestRect.h);
	SDL_Rect src{};
	src.x = static_cast<int>(sourceRect.x);
	src.y = static_cast<int>(sourceRect.y);
	src.w = static_cast<int>(sourceRect.w);
	src.h = static_cast<int>(sourceRect.h);

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, angle, &center,SDL_FLIP_NONE);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
