#pragma once
#include <vector>
#include <string>
#include <memory>
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class GameObject;
	
	class Scene final
	{
		friend Scene& dae::SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::unique_ptr<GameObject> object);
		void AddCollisionObject(GameObject* go);
		void Remove(std::unique_ptr<GameObject> object);
		void RemoveAll();

		void Update();
		void Render() const;
		void Cleanup();
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 

		explicit Scene(const std::string& name);
		
		void CollisionCheck();

		std::string m_name;
		std::vector < std::unique_ptr<GameObject>> m_objects{};
		std::vector <GameObject*> m_CollisionObjects{};
		std::vector <GameObject*> m_CurrentCollisions{};
		static unsigned int m_idCounter;
	};

}
