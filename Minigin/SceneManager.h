#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void Render();
		void Cleanup();
		Scene* GetCurrentScene() { return m_ActiveScene; }
		void SetNextScene(const std::string& name);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::map<std::string,std::unique_ptr<Scene>> m_scenes;
		Scene* m_ActiveScene{};
		Scene* m_NextScene{};

	};
}
