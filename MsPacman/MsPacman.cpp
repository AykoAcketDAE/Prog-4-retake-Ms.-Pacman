#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include "MsPacman.h"
#include "Minigin.h"
#include "Tile.h"
#include <SDL_rect.h>
void LoadGrid(dae::Scene& scene) {
	
	for (int row{}; row < 28; ++row)
	{
		for (int col{}; col < 36; ++col)
		{
			auto go = std::make_unique<dae::GameObject>();
			TileInfo info{ row * 24,(col * 24) + 100,24,true };
			go->AddComponent<Tile>(row, col, info);
			scene.Add(std::move(go));
		}
	}
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("scene");
	auto go = std::make_unique<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
	//map
	SDL_FRect mapSrc = {0,0,224,248};
	SDL_FRect mapDst = {0,0,672,744};
	go->SetPosition(0, 100);
	go->AddComponent<dae::RenderComponent>("Map1.png", mapSrc, mapDst);
	scene.Add(std::move(go));
	LoadGrid(scene);
}

int main(int, char* []) {
	std::filesystem::path data_location = "./Data/";
	if (!std::filesystem::exists(data_location))
		data_location = "../Data/";
	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}