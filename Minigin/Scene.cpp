#include "Scene.h"
#include "GameObject.h"
#include "SDL.h"
#include <algorithm>
#include "CollisionComponent.h"
#include "SceneManager.h"
using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {
	
}

void dae::Scene::CollisionCheck()
{
	for (auto& object1 : m_CollisionObjects)
	{
		if (object1 == nullptr) continue;
		for (auto& object2 : m_CollisionObjects)
		{
			if (object2 == nullptr) continue;
			if (object1 == object2) continue;
			if (SDL_HasIntersectionF(object2->GetBounds(), object1->GetBounds()) and
				std::find(m_CurrentCollisions.begin(),m_CurrentCollisions.end(),object2) == m_CurrentCollisions.end())
			{
				if(object2->HasComponent<CollisionComponent>())
				{
					object2->GetComponent<CollisionComponent>()->OnEnter(object1);
					m_CurrentCollisions.emplace_back(object2);
				}
			}
			else if(SDL_HasIntersectionF(object2->GetBounds(), object1->GetBounds()) and
					std::find(m_CurrentCollisions.begin(), m_CurrentCollisions.end(), object2) != m_CurrentCollisions.end()){
				if (object2->HasComponent<CollisionComponent>())
				{
					object2->GetComponent<CollisionComponent>()->WhileColliding(object1);
				}
			}
			else {
				m_CurrentCollisions.erase(std::remove(m_CurrentCollisions.begin(), m_CurrentCollisions.end(), object2), m_CurrentCollisions.end());
			}
		}
		
	}
}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	if (object->HasComponent<CollisionComponent>())
		m_CollisionObjects.push_back(object.get());
	m_objects.push_back(std::move(object));
}

void dae::Scene::AddCollisionObject(GameObject* go)
{
	m_CollisionObjects.emplace_back(go);
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}


void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	for (int index{}; index < m_objects.size();++index)
	{
		if(m_objects[index])
			m_objects[index]->Update();
	}
	CollisionCheck();
}

void Scene::Render() const
{
	for (int index{}; index < m_objects.size(); ++index)
	{
		if (m_objects[index])
			m_objects[index]->Render();
	}
}

void dae::Scene::Cleanup()
{
	for (auto& object : m_objects)
	{
		if (object == nullptr) Remove(std::move(object));
		else if (object->GetIsMarkedForDestruction())
		{
			Remove(std::move(object));
		}
	}
}
