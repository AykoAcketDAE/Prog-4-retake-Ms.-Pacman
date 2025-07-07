#pragma once
#include <memory>
#include "Transform.h"
#include <string>
#include <vector>
#include "BaseComponent.h"
#include <SDL_rect.h>
#include <concepts>

namespace dae
{
	

	class BaseComponent;

	class GameObject final
	{
	public:
		virtual void Update();
		virtual void Render() const;

		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<typename Comp, typename... Args>
		void AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<BaseComponent, Comp>::value, "ComponentType must be derived from Component");

			auto component = std::make_unique<Comp>(this, std::forward<Args>(args)...);

			m_ComponentPtrList.push_back(std::move(component));
		};
		template<typename Comp>
		void RemoveComponent()
		{
			static_assert(std::is_base_of<BaseComponent, Comp>::value, "ComponentType must be derived from Component");

			auto it = std::remove_if(m_ComponentPtrList.begin(), m_ComponentPtrList.end(),
				[](const std::unique_ptr<BaseComponent>& component)
				{
					return dynamic_cast<Comp*>(component.get()) != nullptr;
				});

			if (it != m_ComponentPtrList.end())
			{
				m_ComponentPtrList.erase( it, m_ComponentPtrList.end());
			}
		
			
			

		};
		template<typename Comp>
		Comp* GetComponent()
		{
			static_assert(std::is_base_of<BaseComponent, Comp>::value, "ComponentType must be derived from Component");

			for (const auto& component : m_ComponentPtrList)
			{
				if (auto castedComp = dynamic_cast<Comp*>(component.get()))
				{
					return castedComp;
				}
			}
			return nullptr;
		};
		template<typename Comp>
		bool HasComponent()
		{
			static_assert(std::is_base_of<BaseComponent, Comp>::value, "ComponentType must be derived from Component");
			for (const auto& component : m_ComponentPtrList)
			{
				if (component == nullptr) continue;
				if (dynamic_cast<Comp*>(component.get()))
				{
					return true;
				}
			}
			return false;
		};

		void SetParent(GameObject* parent, bool keepWorldPosition);
		void SetLocalPosition(const glm::vec3& pos);
		void SetWorldPosition(const glm::vec3& pos);
		void UpdateWorldPosition();
		void SetPositionDirty();

		Transform GetLocalTransform() const { return m_LocalPosition; };
		Transform GetWorldTransform() const { return m_WorldPosition; };

		const glm::vec3& GetWorldPosition();
		glm::vec3 GetPosition() const { return m_LocalPosition.GetPosition(); };

		GameObject* GetParent()const { return m_Parent; };
		std::vector<GameObject*> GetChildren() const { return m_Children; };

		void SetBounds(SDL_FRect bounds) { 
			m_Bounds = bounds; 
		};
		const SDL_FRect* GetBounds() {
			return 	&m_Bounds;
		}
		void MarkForDestruction();
		bool GetIsMarkedForDestruction() const { return m_MarkedForDestruction; };

	private:

		void RemoveChild(GameObject* child);
		void AddChild(GameObject* child);
		bool IsChild(GameObject* child);

		std::vector<std::unique_ptr<BaseComponent>> m_ComponentPtrList{};

		SDL_FRect m_Bounds{};
		std::vector<GameObject*> m_Children	{};
		GameObject* m_Parent				{ nullptr };
		Transform m_LocalPosition			{};
		Transform m_WorldPosition			{};
		bool m_PositionIsDirty				{ false };
		bool m_MarkedForDestruction			{ false };
	};
}
