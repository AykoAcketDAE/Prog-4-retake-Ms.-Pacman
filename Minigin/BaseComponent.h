#pragma once
#include "GameObject.h"


namespace dae {

	
	class GameObject;
	
	class BaseComponent
	{
	public:
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void Update() = 0;
		virtual void Render() const = 0;

	protected:
		BaseComponent(GameObject* owner) : m_Owner{ owner } {};
		GameObject* GetOwner() const { return m_Owner; };

	private:
		GameObject* m_Owner{ nullptr };

	};

	
};

