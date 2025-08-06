#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include "MsPacman.h"
#include "Minigin.h"
#include "Tile.h"
#include "Grid.h"
#include "Player.h"
#include <SDL_rect.h>
#include <fstream>
#include "MsPacmanCommands.h"
#include "Pellet.h"
#include "Score.h"

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
	std::vector<std::vector<Tile*>> grid(28);
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

	auto score = std::make_unique<dae::GameObject>();
	score->AddComponent<dae::TextComponent>("0", font);
	score->GetComponent<dae::TextComponent>()->SetPosition(40, 60);
	score->AddComponent<Score>();

	auto player = std::make_unique<dae::GameObject>();
	SDL_FRect PlayerSrc = { 0,0,16,16 };
	SDL_FRect PlayerDst = { 0,0,24,24};
	Player::PlayerInfo playerInfo{};
	playerInfo.time = 0.1f;
	playerInfo.isMoving = false;
	playerInfo.direction = {0,0};
	playerInfo.gridPos = {1,1};
	PlayerCommands playerCommands;
	playerCommands.m_ScorePellet = std::make_unique<AddPelletScore>(player.get(), score.get());
	player->AddComponent<dae::RenderComponent>("MsPacman.png", PlayerSrc, PlayerDst);
	auto gridcomp = grid->GetComponent<Grid>();
	player->AddComponent<Player>(player->GetComponent<dae::RenderComponent>(), playerInfo,grid->GetComponent<Grid>(),std::move(playerCommands));

	for (int row{1}; row < gridcomp->m_Grid.size()-1; ++row)
	{
		for (int col{1}; col < gridcomp->m_Grid[row].size()-1; ++col)
		{
			if (gridcomp->m_Grid[row][col]->m_TileInfo.isWalkable)
			{
				auto pellet = std::make_unique<dae::GameObject>();
				pellet->AddComponent<Pellet>(gridcomp,row,col, false);
				scene.Add(std::move(pellet));
			}
		}
	}
	input.AddKeyboardCommand(SDLK_w, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,-1 }));
	input.AddKeyboardCommand(SDLK_a, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ -1,0 }));
	input.AddKeyboardCommand(SDLK_s, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,1 }));
	input.AddKeyboardCommand(SDLK_d, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 1,0 }));

	player->SetParent(grid.get(), false);
	grid->SetPosition(0,100);
	scene.Add(std::move(grid));
	scene.Add(std::move(score));
	scene.Add(std::move(player));
}

int main(int, char* []) {
	std::filesystem::path data_location = "./Data/";
	if (!std::filesystem::exists(data_location))
		data_location = "../Data/";
	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}
