#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include "MsPacman.h"
#include "Minigin.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("scene");
	auto go = std::make_unique<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
	go->AddComponent<dae::TextComponent>("I'm blue daba dee daba die",font);
	go->GetComponent<dae::TextComponent>()->SetColor(0, 0, 255, 0);
	go->SetLocalPosition({ 200,300,0 });
	scene.Add(std::move(go));
}

int main(int, char* []) {
	std::filesystem::path data_location = "./Data/";
	if (!std::filesystem::exists(data_location))
		data_location = "../Data/";
	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}