#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
dae::RenderComponent::RenderComponent(GameObject* owner, const std::string& fullPath)
	:BaseComponent(owner),m_Path{ fullPath }
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(fullPath);
}

dae::RenderComponent::RenderComponent(GameObject* owner, const std::string& fullPath, const SDL_FRect& SrcBounds, const SDL_FRect& DestBounds)
	:BaseComponent(owner), m_Path{ fullPath }, m_SrcBounds{SrcBounds}, m_DestBounds{DestBounds}, m_HasBounds{true}
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(fullPath);
}

void dae::RenderComponent::Update()
{
	m_DestBounds.x = GetOwner()->GetWorldPosition().x;
	m_DestBounds.y = GetOwner()->GetWorldPosition().y;
}
void dae::RenderComponent::Render() const
{
	if(!m_HasBounds)
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, GetOwner()->GetPosition().x, GetOwner()->GetPosition().y,m_Angle,m_Center);
	else
		dae::Renderer::GetInstance().RenderTexture(*m_Texture,m_SrcBounds,m_DestBounds,m_Angle,m_Center);
}

void dae::RenderComponent::SetTexture(const std::string& fullPath)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(fullPath);
	m_Path = fullPath;
}

