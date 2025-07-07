#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"


dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (const auto& component : m_ComponentPtrList) {
		if (component == nullptr) continue;
		component->Update();
	}
	m_Bounds.x = m_WorldPosition.GetPosition().x;
	m_Bounds.y = m_WorldPosition.GetPosition().y;
}

void dae::GameObject::Render() const
{
	if (&m_ComponentPtrList == nullptr) return;
	for (const auto& component : m_ComponentPtrList) {
		if (component == nullptr) continue;
		component->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_LocalPosition.SetPosition(x, y, 0.0f);
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_Parent == parent)
		return;
	if (parent == nullptr)
		SetLocalPosition(GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		SetPositionDirty();
	}
	if (m_Parent) m_Parent->RemoveChild(this);
	m_Parent = parent;
	if (m_Parent) m_Parent->AddChild(this);
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalPosition.SetPosition(pos);
	SetPositionDirty();
}

void dae::GameObject::SetWorldPosition(const glm::vec3& pos)
{
	m_WorldPosition.SetPosition(pos);
	SetPositionDirty();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_Parent == nullptr)
			m_WorldPosition.SetPosition(m_LocalPosition.GetPosition());
		else
			m_WorldPosition.SetPosition(m_Parent->GetWorldPosition() + m_LocalPosition.GetPosition());
	}
	m_PositionIsDirty = false;
}

void dae::GameObject::SetPositionDirty()
{
	m_PositionIsDirty = true;
	for (int index{}; index < m_Children.size(); ++index)
	{
		m_Children[index]->SetPositionDirty();
	}
}

void dae::GameObject::MarkForDestruction()
{ 
	m_MarkedForDestruction = true; 
	for (auto child : m_Children)
	{
		child->MarkForDestruction();
	}
}


void dae::GameObject::RemoveChild(GameObject* child)
{
	if (child) {
		child->SetParent(nullptr, true);
		child->UpdateWorldPosition();
		m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child));
	}
}

void dae::GameObject::AddChild(GameObject* child)
{
	if (child and m_Parent != child)
	{
		m_Children.emplace_back(child);
	}
}

bool dae::GameObject::IsChild(GameObject* child)
{
	if (child && m_Parent != child)
	{
		return std::find(child->m_Children.begin(), child->m_Children.end(), this) != child->m_Children.end();
	}
	else return false;
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldPosition.GetPosition();
}

