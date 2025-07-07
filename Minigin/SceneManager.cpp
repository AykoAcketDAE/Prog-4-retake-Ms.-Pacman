#include "SceneManager.h"
#include "Scene.h"


dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::unique_ptr<Scene>(new Scene(name));
	Scene* scenePtr = scene.get();
	m_ActiveScene = scene.get();
	m_scenes[name] = std::move(scene);
	return *scenePtr;
}

void dae::SceneManager::Update()
{
	if (m_NextScene != nullptr)
	{
		std::swap(m_ActiveScene, m_NextScene);
		m_NextScene = nullptr;
	}
	if (m_ActiveScene!= nullptr)
		m_ActiveScene->Update();
}

void dae::SceneManager::Render()
{
	if (m_ActiveScene)
		m_ActiveScene->Render();
}

void dae::SceneManager::Cleanup()
{
	if (m_ActiveScene)
		m_ActiveScene->Cleanup();
}

void dae::SceneManager::SetNextScene(const std::string& name)
{
	m_NextScene = m_scenes[name].get();
}


