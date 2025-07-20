#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include "MsPacman.h"
#include "Minigin.h"
#include "Tile.h"
#include "Player.h"
#include <SDL_rect.h>
#include <fstream>
#include "Grid.h"
#include "MsPacmanCommands.h"

std::vector<std::vector<Tile*>> LoadGrid(dae::Scene& scene) {
	std::filesystem::path data_location = "../Data/";
	std::filesystem::path path{ data_location/"maps.txt" };
	std::fstream file;
	if (std::filesystem::exists(path))
	{
		file = std::fstream(path);
		std::cout << "found file\n---------------------------------------------------------\n\n";
	}
	else
	{
		std::cout << "File not found\n---------------------------------------------------------\n\n";
	}
	std::vector<std::vector<Tile*>> grid(31);
	std::string line;
	for (int row{}; row < 28; ++row)
	{
		std::getline(file, line);
		for (int col{}; col < 31; ++col)
		{
			auto go = std::make_unique<dae::GameObject>();
			TileInfo info{ row,col,24,static_cast<bool>(line[col] - 48) };
			go->AddComponent<Tile>((row * 24), (col * 24) + 100, info);
			grid[row].push_back(go->GetComponent<Tile>());
			scene.Add(std::move(go));
		}
		
	}
	return grid;
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("scene");
	auto& input = dae::InputManager::GetInstance();
	auto go = std::make_unique<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
	auto grid = std::make_unique<dae::GameObject>();
	
	
	//map
	SDL_FRect mapSrc = {0,0,224,248};
	SDL_FRect mapDst = {0,0,672,744};
	go->SetPosition(0, 100);
	go->AddComponent<dae::RenderComponent>("Map1.png", mapSrc, mapDst);
	scene.Add(std::move(go));
	
	auto GridTemp = LoadGrid(scene);
	grid->AddComponent<Grid>(GridTemp);

	go = std::make_unique<dae::GameObject>();
	SDL_FRect PlayerSrc = { 0,0,16,16 };
	SDL_FRect PlayerDst = { 0,0,24,24};
	Player::PlayerInfo playerInfo{};
	playerInfo.time = 0.1f;
	playerInfo.isMoving = false;
	playerInfo.direction = {0,0};
	playerInfo.gridPos = {1,1};
	go->AddComponent<dae::RenderComponent>("MsPacman.png", PlayerSrc, PlayerDst);
	go->AddComponent<Player>(go->GetComponent<dae::RenderComponent>(), playerInfo,grid->GetComponent<Grid>());
	input.AddKeyboardCommand(SDLK_w, std::make_unique<SetPlayerDirection>(go.get(), glm::vec2{ 0,-1 }));
	input.AddKeyboardCommand(SDLK_a, std::make_unique<SetPlayerDirection>(go.get(), glm::vec2{ -1,0 }));
	input.AddKeyboardCommand(SDLK_s, std::make_unique<SetPlayerDirection>(go.get(), glm::vec2{ 0,1 }));
	input.AddKeyboardCommand(SDLK_d, std::make_unique<SetPlayerDirection>(go.get(), glm::vec2{ 1,0 }));
	
	go->SetParent(grid.get(), false);
	grid->SetPosition(0,100);
	scene.Add(std::move(grid));
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
