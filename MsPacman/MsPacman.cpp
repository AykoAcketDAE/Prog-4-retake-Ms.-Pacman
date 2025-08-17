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
#include "Ghost.h"
#include "HealthComponent.h"

bool solo{};
bool coop{};
bool versus{};

void SetBoolCoop()
{
	solo = false;
	coop = true;
	versus = false;
}
void SetBoolSolo()
{
	solo = true;
	coop = false;
	versus = false;
}
void SetBoolVersus()
{
	solo = false;
	coop = false;
	versus = true;
}
std::vector<std::vector<Tile*>> LoadGrid(dae::Scene& scene,const std::string& string) {
	std::filesystem::path data_location = "../Data/";
	std::filesystem::path path{ data_location/string };
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

void LoadMap1()
{
	dae::servicelocator::GetSoundSystem().Play(dae::make_sdbm_hash("Ambience"), 100, 999);
	if(solo)
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Map1");
		auto& input = dae::InputManager::GetInstance();
		auto go = std::make_unique<dae::GameObject>();
		auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
		auto grid = std::make_unique<dae::GameObject>();
		input.AddKeyboardCommand(SDLK_F1, std::make_unique<SkipLevel>());
		input.AddKeyboardCommand(SDLK_F2, std::make_unique<MuteMusic>());
		//map
		SDL_FRect mapSrc = { 0,0,224,248 };
		SDL_FRect mapDst = { 0,0,672,744 };
		go->SetPosition(0, 100);
		go->AddComponent<dae::RenderComponent>("Map1.png", mapSrc, mapDst);
		scene.Add(std::move(go));

		auto GridTemp = LoadGrid(scene, "Map1.txt");

		grid->AddComponent<Grid>(GridTemp, "Map2");

		auto score = std::make_unique<dae::GameObject>();
		score->AddComponent<dae::TextComponent>("0", font);
		score->GetComponent<dae::TextComponent>()->SetPosition(40, 60);
		score->AddComponent<Score>(0);
		score->GetComponent<Score>()->m_Score = 0;

		auto health = std::make_unique<dae::GameObject>();
		SDL_FRect healthSrc = { 0,0,48,16 };
		SDL_FRect healthDst = { 0,0,96,32 };
		health->SetLocalPosition(glm::vec3{ 10,850,0 });
		health->AddComponent<dae::RenderComponent>("Health.png", healthSrc, healthDst);
		health->AddComponent<HealthComponent>(0);

		auto blinky = std::make_unique<dae::GameObject>();
		auto inky = std::make_unique<dae::GameObject>();
		auto pinky = std::make_unique<dae::GameObject>();
		auto sue = std::make_unique<dae::GameObject>();

		auto player = std::make_unique<dae::GameObject>();
		SDL_FRect PlayerSrc = { 0,0,16,16 };
		SDL_FRect PlayerDst = { 0,0,24,24 };
		Player::PlayerInfo playerInfo{};
		playerInfo.time = 0.2f;
		playerInfo.isMoving = false;
		playerInfo.direction = { 0,0 };
		playerInfo.gridPos = { 13,11 };
		PlayerCommands playerCommands;
		playerCommands.scorePellet = std::make_unique<AddPelletScore>(player.get(), score.get());
		playerCommands.scorePowerPellet = std::make_unique<AddPowerPelletScore>(player.get(), score.get());
		playerCommands.oneGhost = std::make_unique<AddOneGhost>(player.get(), score.get());
		playerCommands.twoGhost = std::make_unique<AddTwoGhost>(player.get(), score.get());
		playerCommands.threeGhost = std::make_unique<AddThreeGhost>(player.get(), score.get());
		playerCommands.fourGhost = std::make_unique<AddFourGhost>(player.get(), score.get());
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(blinky.get()));
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(inky.get()));
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(pinky.get()));
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(sue.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(blinky.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(inky.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(pinky.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(sue.get()));
		playerCommands.playerDied = std::make_unique<PlayerDied>(health.get());


		player->AddComponent<dae::RenderComponent>("MsPacman.png", PlayerSrc, PlayerDst);
		auto gridcomp = grid->GetComponent<Grid>();
		player->AddComponent<Player>(player->GetComponent<dae::RenderComponent>(), playerInfo, grid->GetComponent<Grid>(), std::move(playerCommands), TileTypes::Pacman);


		for (int row{ 1 }; row < gridcomp->m_Grid.size() - 1; ++row)
		{
			for (int col{ 1 }; col < gridcomp->m_Grid[row].size() - 1; ++col)
			{
				if (gridcomp->m_Grid[row][col]->m_TileInfo.isWalkable)
				{
					auto pellet = std::make_unique<dae::GameObject>();
					if (row == 1 and col == 2)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 2)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 1 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else
						pellet->AddComponent<Pellet>(gridcomp, row, col, false);
					scene.Add(std::move(pellet));
				}
			}
		}
		input.AddKeyboardCommand(SDLK_w, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,-1 }));
		input.AddKeyboardCommand(SDLK_a, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ -1,0 }));
		input.AddKeyboardCommand(SDLK_s, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,1 }));
		input.AddKeyboardCommand(SDLK_d, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 1,0 }));

		input.AddGamepad(std::make_unique<GamepadInput>(0));
		input.AddGamepadCommand(0, dae::GamepadData::DPadUp, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,-1 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadLeft, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ -1,0 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadDown, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,1 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadRight, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 1,0 }));

		player->SetParent(grid.get(), false);
		grid->SetPosition(0, 100);


		blinky->AddComponent<dae::RenderComponent>("Blinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		blinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,13 }, player->GetComponent<Player>(), glm::vec2{ 1,1 }, TileTypes::Blinky);


		inky->AddComponent<dae::RenderComponent>("Inky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		inky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,15 }, player->GetComponent<Player>(), glm::vec2{ 1,29 }, TileTypes::Inky);


		pinky->AddComponent<dae::RenderComponent>("Pinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		pinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,13 }, player->GetComponent<Player>(), glm::vec2{ 26,1 }, TileTypes::Pinky);


		sue->AddComponent<dae::RenderComponent>("Sue.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		sue->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,15 }, player->GetComponent<Player>(), glm::vec2{ 26,29 }, TileTypes::Sue);


		scene.Add(std::move(grid));
		scene.Add(std::move(health));
		scene.Add(std::move(score));
		scene.Add(std::move(player));
		scene.Add(std::move(blinky));
		scene.Add(std::move(inky));
		scene.Add(std::move(pinky));
		scene.Add(std::move(sue));
	}
	else if (coop)
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Map1");
		auto& input = dae::InputManager::GetInstance();
		auto go = std::make_unique<dae::GameObject>();
		auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
		auto grid = std::make_unique<dae::GameObject>();
		input.AddKeyboardCommand(SDLK_F1, std::make_unique<SkipLevel>());
		input.AddKeyboardCommand(SDLK_F2, std::make_unique<MuteMusic>());

		//map
		SDL_FRect mapSrc = { 0,0,224,248 };
		SDL_FRect mapDst = { 0,0,672,744 };
		go->SetPosition(0, 100);
		go->AddComponent<dae::RenderComponent>("Map1.png", mapSrc, mapDst);
		scene.Add(std::move(go));

		auto GridTemp = LoadGrid(scene, "Map1.txt");

		grid->AddComponent<Grid>(GridTemp, "Map2");

		auto score = std::make_unique<dae::GameObject>();
		score->AddComponent<dae::TextComponent>("0", font);
		score->GetComponent<dae::TextComponent>()->SetPosition(40, 60);
		score->AddComponent<Score>(0);
		score->GetComponent<Score>()->m_Score = 0;

		auto health = std::make_unique<dae::GameObject>();
		SDL_FRect healthSrc = { 0,0,48,16 };
		SDL_FRect healthDst = { 0,0,96,32 };
		health->SetLocalPosition(glm::vec3{ 10,850,0 });
		health->AddComponent<dae::RenderComponent>("Health.png", healthSrc, healthDst);
		health->AddComponent<HealthComponent>(0);


		auto blinky = std::make_unique<dae::GameObject>();
		auto inky = std::make_unique<dae::GameObject>();
		auto pinky = std::make_unique<dae::GameObject>();
		auto sue = std::make_unique<dae::GameObject>();

		auto player1 = std::make_unique<dae::GameObject>();
		auto player2 = std::make_unique<dae::GameObject>();
		SDL_FRect PlayerSrc = { 0,0,16,16 };
		SDL_FRect PlayerDst = { 0,0,24,24 };
		Player::PlayerInfo playerInfo{};
		playerInfo.time = 0.2f;
		playerInfo.isMoving = false;
		playerInfo.direction = { 0,0 };
		playerInfo.gridPos = { 13,11 };
		PlayerCommands player1Commands;
		


		player1->AddComponent<dae::RenderComponent>("MsPacman.png", PlayerSrc, PlayerDst);
		player2->AddComponent<dae::RenderComponent>("Player2.png", PlayerSrc, PlayerDst);
		auto gridcomp = grid->GetComponent<Grid>();

		player1Commands.scorePellet = std::make_unique<AddPelletScore>(player1.get(), score.get());
		player1Commands.scorePowerPellet = std::make_unique<AddPowerPelletScore>(player1.get(), score.get());
		player1Commands.oneGhost = std::make_unique<AddOneGhost>(player1.get(), score.get());
		player1Commands.twoGhost = std::make_unique<AddTwoGhost>(player1.get(), score.get());
		player1Commands.threeGhost = std::make_unique<AddThreeGhost>(player1.get(), score.get());
		player1Commands.fourGhost = std::make_unique<AddFourGhost>(player1.get(), score.get());
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(blinky.get()));
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(inky.get()));
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(pinky.get()));
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(sue.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(blinky.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(inky.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(pinky.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(sue.get()));
		player1Commands.playerDied = std::make_unique<PlayerDied>(health.get());

		player1->AddComponent<Player>(player1->GetComponent<dae::RenderComponent>(), playerInfo, grid->GetComponent<Grid>(), std::move(player1Commands), TileTypes::Pacman);
		PlayerCommands player2Commands;
		player2Commands.scorePellet = std::make_unique<AddPelletScore>(player2.get(), score.get());
		player2Commands.scorePowerPellet = std::make_unique<AddPowerPelletScore>(player2.get(), score.get());
		player2Commands.oneGhost = std::make_unique<AddOneGhost>(player2.get(), score.get());
		player2Commands.twoGhost = std::make_unique<AddTwoGhost>(player2.get(), score.get());
		player2Commands.threeGhost = std::make_unique<AddThreeGhost>(player2.get(), score.get());
		player2Commands.fourGhost = std::make_unique<AddFourGhost>(player2.get(), score.get());
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(blinky.get()));
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(inky.get()));
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(pinky.get()));
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(sue.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(blinky.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(inky.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(pinky.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(sue.get()));
		player2Commands.playerDied = std::make_unique<PlayerDied>(health.get());

		player2->AddComponent<Player>(player2->GetComponent<dae::RenderComponent>(), playerInfo, grid->GetComponent<Grid>(), std::move(player2Commands), TileTypes::Pacman);


		for (int row{ 1 }; row < gridcomp->m_Grid.size() - 1; ++row)
		{
			for (int col{ 1 }; col < gridcomp->m_Grid[row].size() - 1; ++col)
			{
				if (gridcomp->m_Grid[row][col]->m_TileInfo.isWalkable)
				{
					auto pellet = std::make_unique<dae::GameObject>();
					if (row == 1 and col == 2)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 2)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 1 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else
						pellet->AddComponent<Pellet>(gridcomp, row, col, false);
					scene.Add(std::move(pellet));
				}
			}
		}
		input.AddKeyboardCommand(SDLK_w, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 0,-1 }));
		input.AddKeyboardCommand(SDLK_a, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ -1,0 }));
		input.AddKeyboardCommand(SDLK_s, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 0,1 }));
		input.AddKeyboardCommand(SDLK_d, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 1,0 }));

		input.AddGamepad(std::make_unique<GamepadInput>(0));
		input.AddGamepadCommand(0,dae::GamepadData::DPadUp, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 0,-1 }));
		input.AddGamepadCommand(0,dae::GamepadData::DPadLeft, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ -1,0 }));
		input.AddGamepadCommand(0,dae::GamepadData::DPadDown, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 0,1 }));
		input.AddGamepadCommand(0,dae::GamepadData::DPadRight, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 1,0 }));

		input.AddGamepad(std::make_unique<GamepadInput>(1));
		input.AddGamepadCommand(1, dae::GamepadData::DPadUp, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 0,-1 }));
		input.AddGamepadCommand(1, dae::GamepadData::DPadLeft, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ -1,0 }));
		input.AddGamepadCommand(1, dae::GamepadData::DPadDown, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 0,1 }));
		input.AddGamepadCommand(1, dae::GamepadData::DPadRight, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 1,0 }));

		player1->SetParent(grid.get(), false);
		grid->SetPosition(0, 100);

		blinky->AddComponent<dae::RenderComponent>("Blinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		blinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,13 }, player1->GetComponent<Player>(), glm::vec2{ 1,1 }, TileTypes::Blinky);

		inky->AddComponent<dae::RenderComponent>("Inky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		inky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,15 }, player1->GetComponent<Player>(), glm::vec2{ 1,29 }, TileTypes::Inky);

		pinky->AddComponent<dae::RenderComponent>("Pinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		pinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,13 }, player1->GetComponent<Player>(), glm::vec2{ 26,1 }, TileTypes::Pinky);

		sue->AddComponent<dae::RenderComponent>("Sue.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		sue->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,15 }, player1->GetComponent<Player>(), glm::vec2{ 26,29 }, TileTypes::Sue);

		scene.Add(std::move(grid));
		scene.Add(std::move(health));
		scene.Add(std::move(score));
		scene.Add(std::move(player1));
		scene.Add(std::move(player2));
		scene.Add(std::move(blinky));
		scene.Add(std::move(inky));
		scene.Add(std::move(pinky));
		scene.Add(std::move(sue));
	}
}
void LoadMap2()
{
	if (solo)
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Map2");
		auto& input = dae::InputManager::GetInstance();
		auto go = std::make_unique<dae::GameObject>();
		auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
		auto grid = std::make_unique<dae::GameObject>();
		input.AddKeyboardCommand(SDLK_F1, std::make_unique<SkipLevel>());
		input.AddKeyboardCommand(SDLK_F2, std::make_unique<MuteMusic>());

		//map
		SDL_FRect mapSrc = { 0,0,224,248 };
		SDL_FRect mapDst = { 0,0,672,744 };
		go->SetPosition(0, 100);
		go->AddComponent<dae::RenderComponent>("Map2.png", mapSrc, mapDst);
		scene.Add(std::move(go));

		auto GridTemp = LoadGrid(scene, "Map2.txt");

		grid->AddComponent<Grid>(GridTemp, "Map3");

		auto score = std::make_unique<dae::GameObject>();
		score->AddComponent<dae::TextComponent>("0", font);
		score->GetComponent<dae::TextComponent>()->SetPosition(40, 60);
		score->AddComponent<Score>(0);


		auto health = std::make_unique<dae::GameObject>();
		SDL_FRect healthSrc = { 0,0,48,16 };
		SDL_FRect healthDst = { 0,0,96,32 };
		health->SetLocalPosition(glm::vec3{ 10,850,0 });
		health->AddComponent<dae::RenderComponent>("Health.png", healthSrc, healthDst);
		health->AddComponent<HealthComponent>(0);

		auto blinky = std::make_unique<dae::GameObject>();
		auto inky = std::make_unique<dae::GameObject>();
		auto pinky = std::make_unique<dae::GameObject>();
		auto sue = std::make_unique<dae::GameObject>();

		auto player = std::make_unique<dae::GameObject>();
		SDL_FRect PlayerSrc = { 0,0,16,16 };
		SDL_FRect PlayerDst = { 0,0,24,24 };
		Player::PlayerInfo playerInfo{};
		playerInfo.time = 0.2f;
		playerInfo.isMoving = false;
		playerInfo.direction = { 0,0 };
		playerInfo.gridPos = { 13,11 };
		PlayerCommands playerCommands;
		playerCommands.scorePellet = std::make_unique<AddPelletScore>(player.get(), score.get());
		playerCommands.scorePowerPellet = std::make_unique<AddPowerPelletScore>(player.get(), score.get());
		playerCommands.oneGhost = std::make_unique<AddOneGhost>(player.get(), score.get());
		playerCommands.twoGhost = std::make_unique<AddTwoGhost>(player.get(), score.get());
		playerCommands.threeGhost = std::make_unique<AddThreeGhost>(player.get(), score.get());
		playerCommands.fourGhost = std::make_unique<AddFourGhost>(player.get(), score.get());
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(blinky.get()));
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(inky.get()));
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(pinky.get()));
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(sue.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(blinky.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(inky.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(pinky.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(sue.get()));
		playerCommands.playerDied = std::make_unique<PlayerDied>(health.get());


		player->AddComponent<dae::RenderComponent>("MsPacman.png", PlayerSrc, PlayerDst);
		auto gridcomp = grid->GetComponent<Grid>();
		player->AddComponent<Player>(player->GetComponent<dae::RenderComponent>(), playerInfo, grid->GetComponent<Grid>(), std::move(playerCommands), TileTypes::Pacman);

		for (int row{ 1 }; row < gridcomp->m_Grid.size() - 1; ++row)
		{
			for (int col{ 1 }; col < gridcomp->m_Grid[row].size() - 1; ++col)
			{
				if (gridcomp->m_Grid[row][col]->m_TileInfo.isWalkable)
				{
					auto pellet = std::make_unique<dae::GameObject>();
					if (row == 1 and col == 4)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 4)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 1 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else
						pellet->AddComponent<Pellet>(gridcomp, row, col, false);
					scene.Add(std::move(pellet));
				}
			}
		}
		input.AddKeyboardCommand(SDLK_w, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,-1 }));
		input.AddKeyboardCommand(SDLK_a, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ -1,0 }));
		input.AddKeyboardCommand(SDLK_s, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,1 }));
		input.AddKeyboardCommand(SDLK_d, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 1,0 }));
		input.AddGamepad(std::make_unique<GamepadInput>(0));
		input.AddGamepadCommand(0, dae::GamepadData::DPadUp, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,-1 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadLeft, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ -1,0 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadDown, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,1 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadRight, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 1,0 }));

		player->SetParent(grid.get(), false);
		grid->SetPosition(0, 100);


		blinky->AddComponent<dae::RenderComponent>("Blinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		blinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,13 }, player->GetComponent<Player>(), glm::vec2{ 1,1 }, TileTypes::Blinky);


		inky->AddComponent<dae::RenderComponent>("Inky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		inky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,15 }, player->GetComponent<Player>(), glm::vec2{ 1,29 }, TileTypes::Inky);


		pinky->AddComponent<dae::RenderComponent>("Pinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		pinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,13 }, player->GetComponent<Player>(), glm::vec2{ 26,1 }, TileTypes::Pinky);


		sue->AddComponent<dae::RenderComponent>("Sue.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		sue->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,15 }, player->GetComponent<Player>(), glm::vec2{ 26,29 }, TileTypes::Sue);


		scene.Add(std::move(grid));
		scene.Add(std::move(health));
		scene.Add(std::move(score));
		scene.Add(std::move(player));
		scene.Add(std::move(blinky));
		scene.Add(std::move(inky));
		scene.Add(std::move(pinky));
		scene.Add(std::move(sue));
	}
	else if (coop)
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Map2");
		auto& input = dae::InputManager::GetInstance();
		auto go = std::make_unique<dae::GameObject>();
		auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
		auto grid = std::make_unique<dae::GameObject>();
		input.AddKeyboardCommand(SDLK_F1, std::make_unique<SkipLevel>());
		input.AddKeyboardCommand(SDLK_F2, std::make_unique<MuteMusic>());

		//map
		SDL_FRect mapSrc = { 0,0,224,248 };
		SDL_FRect mapDst = { 0,0,672,744 };
		go->SetPosition(0, 100);
		go->AddComponent<dae::RenderComponent>("Map2.png", mapSrc, mapDst);
		scene.Add(std::move(go));

		auto GridTemp = LoadGrid(scene, "Map2.txt");

		grid->AddComponent<Grid>(GridTemp, "Map3");

		auto score = std::make_unique<dae::GameObject>();
		score->AddComponent<dae::TextComponent>("0", font);
		score->GetComponent<dae::TextComponent>()->SetPosition(40, 60);
		score->AddComponent<Score>(0);


		auto health = std::make_unique<dae::GameObject>();
		SDL_FRect healthSrc = { 0,0,48,16 };
		SDL_FRect healthDst = { 0,0,96,32 };
		health->SetLocalPosition(glm::vec3{ 10,850,0 });
		health->AddComponent<dae::RenderComponent>("Health.png", healthSrc, healthDst);
		health->AddComponent<HealthComponent>(0);

		auto blinky = std::make_unique<dae::GameObject>();
		auto inky = std::make_unique<dae::GameObject>();
		auto pinky = std::make_unique<dae::GameObject>();
		auto sue = std::make_unique<dae::GameObject>();

		auto player1 = std::make_unique<dae::GameObject>();
		auto player2 = std::make_unique<dae::GameObject>();
		SDL_FRect PlayerSrc = { 0,0,16,16 };
		SDL_FRect PlayerDst = { 0,0,24,24 };
		Player::PlayerInfo playerInfo{};
		playerInfo.time = 0.2f;
		playerInfo.isMoving = false;
		playerInfo.direction = { 0,0 };
		playerInfo.gridPos = { 13,11 };
		PlayerCommands player1Commands;

		player1->AddComponent<dae::RenderComponent>("MsPacman.png", PlayerSrc, PlayerDst);
		player2->AddComponent<dae::RenderComponent>("Player2.png", PlayerSrc, PlayerDst);
		auto gridcomp = grid->GetComponent<Grid>();

		player1Commands.scorePellet = std::make_unique<AddPelletScore>(player1.get(), score.get());
		player1Commands.scorePowerPellet = std::make_unique<AddPowerPelletScore>(player1.get(), score.get());
		player1Commands.oneGhost = std::make_unique<AddOneGhost>(player1.get(), score.get());
		player1Commands.twoGhost = std::make_unique<AddTwoGhost>(player1.get(), score.get());
		player1Commands.threeGhost = std::make_unique<AddThreeGhost>(player1.get(), score.get());
		player1Commands.fourGhost = std::make_unique<AddFourGhost>(player1.get(), score.get());
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(blinky.get()));
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(inky.get()));
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(pinky.get()));
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(sue.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(blinky.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(inky.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(pinky.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(sue.get()));
		player1Commands.playerDied = std::make_unique<PlayerDied>(health.get());

		player1->AddComponent<Player>(player1->GetComponent<dae::RenderComponent>(), playerInfo, grid->GetComponent<Grid>(), std::move(player1Commands), TileTypes::Pacman);
		PlayerCommands player2Commands;
		player2Commands.scorePellet = std::make_unique<AddPelletScore>(player2.get(), score.get());
		player2Commands.scorePowerPellet = std::make_unique<AddPowerPelletScore>(player2.get(), score.get());
		player2Commands.oneGhost = std::make_unique<AddOneGhost>(player2.get(), score.get());
		player2Commands.twoGhost = std::make_unique<AddTwoGhost>(player2.get(), score.get());
		player2Commands.threeGhost = std::make_unique<AddThreeGhost>(player2.get(), score.get());
		player2Commands.fourGhost = std::make_unique<AddFourGhost>(player2.get(), score.get());
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(blinky.get()));
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(inky.get()));
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(pinky.get()));
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(sue.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(blinky.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(inky.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(pinky.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(sue.get()));
		player2Commands.playerDied = std::make_unique<PlayerDied>(health.get());

		player2->AddComponent<Player>(player2->GetComponent<dae::RenderComponent>(), playerInfo, grid->GetComponent<Grid>(), std::move(player2Commands), TileTypes::Pacman);


		for (int row{ 1 }; row < gridcomp->m_Grid.size() - 1; ++row)
		{
			for (int col{ 1 }; col < gridcomp->m_Grid[row].size() - 1; ++col)
			{
				if (gridcomp->m_Grid[row][col]->m_TileInfo.isWalkable)
				{
					auto pellet = std::make_unique<dae::GameObject>();
					if (row == 1 and col == 4)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 4)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 1 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else
						pellet->AddComponent<Pellet>(gridcomp, row, col, false);
					scene.Add(std::move(pellet));
				}
			}
		}
		input.AddKeyboardCommand(SDLK_w, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 0,-1 }));
		input.AddKeyboardCommand(SDLK_a, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ -1,0 }));
		input.AddKeyboardCommand(SDLK_s, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 0,1 }));
		input.AddKeyboardCommand(SDLK_d, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 1,0 }));

		input.AddGamepad(std::make_unique<GamepadInput>(0));
		input.AddGamepadCommand(0, dae::GamepadData::DPadUp, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 0,-1 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadLeft, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ -1,0 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadDown, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 0,1 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadRight, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 1,0 }));

		input.AddGamepad(std::make_unique<GamepadInput>(1));
		input.AddGamepadCommand(1, dae::GamepadData::DPadUp, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 0,-1 }));
		input.AddGamepadCommand(1, dae::GamepadData::DPadLeft, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ -1,0 }));
		input.AddGamepadCommand(1, dae::GamepadData::DPadDown, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 0,1 }));
		input.AddGamepadCommand(1, dae::GamepadData::DPadRight, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 1,0 }));

		player1->SetParent(grid.get(), false);
		grid->SetPosition(0, 100);

		blinky->AddComponent<dae::RenderComponent>("Blinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		blinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,13 }, player1->GetComponent<Player>(), glm::vec2{ 1,1 }, TileTypes::Blinky);

		inky->AddComponent<dae::RenderComponent>("Inky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		inky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,15 }, player1->GetComponent<Player>(), glm::vec2{ 1,29 }, TileTypes::Inky);

		pinky->AddComponent<dae::RenderComponent>("Pinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		pinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,13 }, player1->GetComponent<Player>(), glm::vec2{ 26,1 }, TileTypes::Pinky);

		sue->AddComponent<dae::RenderComponent>("Sue.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		sue->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,15 }, player1->GetComponent<Player>(), glm::vec2{ 26,29 }, TileTypes::Sue);

		scene.Add(std::move(grid));
		scene.Add(std::move(health));
		scene.Add(std::move(score));
		scene.Add(std::move(player1));
		scene.Add(std::move(player2));
		scene.Add(std::move(blinky));
		scene.Add(std::move(inky));
		scene.Add(std::move(pinky));
		scene.Add(std::move(sue));
	}
}
void LoadMap3()
{
	if (solo)
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Map3");
		auto& input = dae::InputManager::GetInstance();
		auto go = std::make_unique<dae::GameObject>();
		auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
		auto grid = std::make_unique<dae::GameObject>();
		input.AddKeyboardCommand(SDLK_F1, std::make_unique<SkipLevel>());
		input.AddKeyboardCommand(SDLK_F2, std::make_unique<MuteMusic>());

		//map
		SDL_FRect mapSrc = { 0,0,224,248 };
		SDL_FRect mapDst = { 0,0,672,744 };
		go->SetPosition(0, 100);
		go->AddComponent<dae::RenderComponent>("Map3.png", mapSrc, mapDst);
		scene.Add(std::move(go));

		auto GridTemp = LoadGrid(scene, "Map3.txt");

		grid->AddComponent<Grid>(GridTemp, "HighScore");

		auto score = std::make_unique<dae::GameObject>();
		score->AddComponent<dae::TextComponent>("0", font);
		score->GetComponent<dae::TextComponent>()->SetPosition(40, 60);
		score->AddComponent<Score>(0);


		auto health = std::make_unique<dae::GameObject>();
		SDL_FRect healthSrc = { 0,0,48,16 };
		SDL_FRect healthDst = { 0,0,96,32 };
		health->SetLocalPosition(glm::vec3{ 10,850,0 });
		health->AddComponent<dae::RenderComponent>("Health.png", healthSrc, healthDst);
		health->AddComponent<HealthComponent>(0);

		auto blinky = std::make_unique<dae::GameObject>();
		auto inky = std::make_unique<dae::GameObject>();
		auto pinky = std::make_unique<dae::GameObject>();
		auto sue = std::make_unique<dae::GameObject>();

		auto player = std::make_unique<dae::GameObject>();
		SDL_FRect PlayerSrc = { 0,0,16,16 };
		SDL_FRect PlayerDst = { 0,0,24,24 };
		Player::PlayerInfo playerInfo{};
		playerInfo.time = 0.2f;
		playerInfo.isMoving = false;
		playerInfo.direction = { 0,0 };
		playerInfo.gridPos = { 13,11 };
		PlayerCommands playerCommands;
		playerCommands.scorePellet = std::make_unique<AddPelletScore>(player.get(), score.get());
		playerCommands.scorePowerPellet = std::make_unique<AddPowerPelletScore>(player.get(), score.get());
		playerCommands.oneGhost = std::make_unique<AddOneGhost>(player.get(), score.get());
		playerCommands.twoGhost = std::make_unique<AddTwoGhost>(player.get(), score.get());
		playerCommands.threeGhost = std::make_unique<AddThreeGhost>(player.get(), score.get());
		playerCommands.fourGhost = std::make_unique<AddFourGhost>(player.get(), score.get());
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(blinky.get()));
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(inky.get()));
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(pinky.get()));
		playerCommands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(sue.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(blinky.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(inky.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(pinky.get()));
		playerCommands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(sue.get()));
		playerCommands.playerDied = std::make_unique<PlayerDied>(health.get());


		player->AddComponent<dae::RenderComponent>("MsPacman.png", PlayerSrc, PlayerDst);
		auto gridcomp = grid->GetComponent<Grid>();
		player->AddComponent<Player>(player->GetComponent<dae::RenderComponent>(), playerInfo, grid->GetComponent<Grid>(), std::move(playerCommands), TileTypes::Pacman);

		for (int row{ 1 }; row < gridcomp->m_Grid.size() - 1; ++row)
		{
			for (int col{ 1 }; col < gridcomp->m_Grid[row].size() - 1; ++col)
			{
				if (gridcomp->m_Grid[row][col]->m_TileInfo.isWalkable)
				{
					auto pellet = std::make_unique<dae::GameObject>();
					if (row == 1 and col == 2)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 2)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 1 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else
						pellet->AddComponent<Pellet>(gridcomp, row, col, false);
					scene.Add(std::move(pellet));
				}
			}
		}
		input.AddKeyboardCommand(SDLK_w, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,-1 }));
		input.AddKeyboardCommand(SDLK_a, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ -1,0 }));
		input.AddKeyboardCommand(SDLK_s, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,1 }));
		input.AddKeyboardCommand(SDLK_d, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 1,0 }));
		input.AddGamepad(std::make_unique<GamepadInput>(0));
		input.AddGamepadCommand(0, dae::GamepadData::DPadUp, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,-1 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadLeft, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ -1,0 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadDown, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 0,1 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadRight, std::make_unique<SetPlayerDirection>(player.get(), glm::vec2{ 1,0 }));

		player->SetParent(grid.get(), false);
		grid->SetPosition(0, 100);


		blinky->AddComponent<dae::RenderComponent>("Blinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		blinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,13 }, player->GetComponent<Player>(), glm::vec2{ 1,1 }, TileTypes::Blinky);


		inky->AddComponent<dae::RenderComponent>("Inky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		inky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,15 }, player->GetComponent<Player>(), glm::vec2{ 1,29 }, TileTypes::Inky);


		pinky->AddComponent<dae::RenderComponent>("Pinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		pinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,13 }, player->GetComponent<Player>(), glm::vec2{ 26,1 }, TileTypes::Pinky);


		sue->AddComponent<dae::RenderComponent>("Sue.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		sue->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,15 }, player->GetComponent<Player>(), glm::vec2{ 26,29 }, TileTypes::Sue);


		scene.Add(std::move(grid));
		scene.Add(std::move(health));
		scene.Add(std::move(score));
		scene.Add(std::move(player));
		scene.Add(std::move(blinky));
		scene.Add(std::move(inky));
		scene.Add(std::move(pinky));
		scene.Add(std::move(sue));
	}
	else if (coop)
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Map3");
		auto& input = dae::InputManager::GetInstance();
		auto go = std::make_unique<dae::GameObject>();
		auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
		auto grid = std::make_unique<dae::GameObject>();
		input.AddKeyboardCommand(SDLK_F1, std::make_unique<SkipLevel>());
		input.AddKeyboardCommand(SDLK_F2, std::make_unique<MuteMusic>());

		//map
		SDL_FRect mapSrc = { 0,0,224,248 };
		SDL_FRect mapDst = { 0,0,672,744 };
		go->SetPosition(0, 100);
		go->AddComponent<dae::RenderComponent>("Map3.png", mapSrc, mapDst);
		scene.Add(std::move(go));

		auto GridTemp = LoadGrid(scene, "Map3.txt");

		grid->AddComponent<Grid>(GridTemp, "HighScore");

		auto score = std::make_unique<dae::GameObject>();
		score->AddComponent<dae::TextComponent>("0", font);
		score->GetComponent<dae::TextComponent>()->SetPosition(40, 60);
		score->AddComponent<Score>(0);


		auto health = std::make_unique<dae::GameObject>();
		SDL_FRect healthSrc = { 0,0,48,16 };
		SDL_FRect healthDst = { 0,0,96,32 };
		health->SetLocalPosition(glm::vec3{ 10,850,0 });
		health->AddComponent<dae::RenderComponent>("Health.png", healthSrc, healthDst);
		health->AddComponent<HealthComponent>(0);

		auto blinky = std::make_unique<dae::GameObject>();
		auto inky = std::make_unique<dae::GameObject>();
		auto pinky = std::make_unique<dae::GameObject>();
		auto sue = std::make_unique<dae::GameObject>();

		auto player1 = std::make_unique<dae::GameObject>();
		auto player2 = std::make_unique<dae::GameObject>();
		SDL_FRect PlayerSrc = { 0,0,16,16 };
		SDL_FRect PlayerDst = { 0,0,24,24 };
		Player::PlayerInfo playerInfo{};
		playerInfo.time = 0.2f;
		playerInfo.isMoving = false;
		playerInfo.direction = { 0,0 };
		playerInfo.gridPos = { 13,11 };
		PlayerCommands player1Commands;

		player1->AddComponent<dae::RenderComponent>("MsPacman.png", PlayerSrc, PlayerDst);
		player2->AddComponent<dae::RenderComponent>("Player2.png", PlayerSrc, PlayerDst);
		auto gridcomp = grid->GetComponent<Grid>();

		player1Commands.scorePellet = std::make_unique<AddPelletScore>(player1.get(), score.get());
		player1Commands.scorePowerPellet = std::make_unique<AddPowerPelletScore>(player1.get(), score.get());
		player1Commands.oneGhost = std::make_unique<AddOneGhost>(player1.get(), score.get());
		player1Commands.twoGhost = std::make_unique<AddTwoGhost>(player1.get(), score.get());
		player1Commands.threeGhost = std::make_unique<AddThreeGhost>(player1.get(), score.get());
		player1Commands.fourGhost = std::make_unique<AddFourGhost>(player1.get(), score.get());
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(blinky.get()));
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(inky.get()));
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(pinky.get()));
		player1Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(sue.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(blinky.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(inky.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(pinky.get()));
		player1Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(sue.get()));
		player1Commands.playerDied = std::make_unique<PlayerDied>(health.get());

		player1->AddComponent<Player>(player1->GetComponent<dae::RenderComponent>(), playerInfo, grid->GetComponent<Grid>(), std::move(player1Commands), TileTypes::Pacman);
		PlayerCommands player2Commands;
		player2Commands.scorePellet = std::make_unique<AddPelletScore>(player2.get(), score.get());
		player2Commands.scorePowerPellet = std::make_unique<AddPowerPelletScore>(player2.get(), score.get());
		player2Commands.oneGhost = std::make_unique<AddOneGhost>(player2.get(), score.get());
		player2Commands.twoGhost = std::make_unique<AddTwoGhost>(player2.get(), score.get());
		player2Commands.threeGhost = std::make_unique<AddThreeGhost>(player2.get(), score.get());
		player2Commands.fourGhost = std::make_unique<AddFourGhost>(player2.get(), score.get());
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(blinky.get()));
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(inky.get()));
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(pinky.get()));
		player2Commands.killGhost.push_back(std::make_unique<SetGhostToSpawn>(sue.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(blinky.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(inky.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(pinky.get()));
		player2Commands.scatterGhost.push_back(std::make_unique<SetGhostToScatter>(sue.get()));
		player2Commands.playerDied = std::make_unique<PlayerDied>(health.get());

		player2->AddComponent<Player>(player2->GetComponent<dae::RenderComponent>(), playerInfo, grid->GetComponent<Grid>(), std::move(player2Commands), TileTypes::Pacman);


		for (int row{ 1 }; row < gridcomp->m_Grid.size() - 1; ++row)
		{
			for (int col{ 1 }; col < gridcomp->m_Grid[row].size() - 1; ++col)
			{
				if (gridcomp->m_Grid[row][col]->m_TileInfo.isWalkable)
				{
					auto pellet = std::make_unique<dae::GameObject>();
					if (row == 1 and col == 2)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 2)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 1 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else if (row == 26 and col == 26)
						pellet->AddComponent<Pellet>(gridcomp, row, col, true);
					else
						pellet->AddComponent<Pellet>(gridcomp, row, col, false);
					scene.Add(std::move(pellet));
				}
			}
		}
		input.AddKeyboardCommand(SDLK_w, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 0,-1 }));
		input.AddKeyboardCommand(SDLK_a, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ -1,0 }));
		input.AddKeyboardCommand(SDLK_s, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 0,1 }));
		input.AddKeyboardCommand(SDLK_d, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 1,0 }));

		input.AddGamepad(std::make_unique<GamepadInput>(0));
		input.AddGamepadCommand(0, dae::GamepadData::DPadUp, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 0,-1 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadLeft, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ -1,0 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadDown, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 0,1 }));
		input.AddGamepadCommand(0, dae::GamepadData::DPadRight, std::make_unique<SetPlayerDirection>(player1.get(), glm::vec2{ 1,0 }));

		input.AddGamepad(std::make_unique<GamepadInput>(1));
		input.AddGamepadCommand(1, dae::GamepadData::DPadUp, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 0,-1 }));
		input.AddGamepadCommand(1, dae::GamepadData::DPadLeft, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ -1,0 }));
		input.AddGamepadCommand(1, dae::GamepadData::DPadDown, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 0,1 }));
		input.AddGamepadCommand(1, dae::GamepadData::DPadRight, std::make_unique<SetPlayerDirection>(player2.get(), glm::vec2{ 1,0 }));

		player1->SetParent(grid.get(), false);
		grid->SetPosition(0, 100);

		blinky->AddComponent<dae::RenderComponent>("Blinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		blinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,13 }, player1->GetComponent<Player>(), glm::vec2{ 1,1 }, TileTypes::Blinky);

		inky->AddComponent<dae::RenderComponent>("Inky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		inky->AddComponent<Ghost>(gridcomp, glm::vec2{ 12,15 }, player1->GetComponent<Player>(), glm::vec2{ 1,29 }, TileTypes::Inky);

		pinky->AddComponent<dae::RenderComponent>("Pinky.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		pinky->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,13 }, player1->GetComponent<Player>(), glm::vec2{ 26,1 }, TileTypes::Pinky);

		sue->AddComponent<dae::RenderComponent>("Sue.png", SDL_FRect{ 0,0,16,16 }, SDL_FRect{ 0.f,0,24,24 });
		sue->AddComponent<Ghost>(gridcomp, glm::vec2{ 15,15 }, player1->GetComponent<Player>(), glm::vec2{ 26,29 }, TileTypes::Sue);

		scene.Add(std::move(grid));
		scene.Add(std::move(health));
		scene.Add(std::move(score));
		scene.Add(std::move(player1));
		scene.Add(std::move(player2));
		scene.Add(std::move(blinky));
		scene.Add(std::move(inky));
		scene.Add(std::move(pinky));
		scene.Add(std::move(sue));
	}
}

void LoadHighScore()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("HighScore");
	auto& input = dae::InputManager::GetInstance();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
	auto score = std::make_unique<dae::GameObject>();
	input.AddKeyboardCommand(SDLK_F2, std::make_unique<MuteMusic>());
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<Score>(0);
	go->AddComponent<dae::TextComponent>("Your Score :" + std::to_string(score->GetComponent<Score>()->m_Score), font);
	
	go->GetComponent<dae::TextComponent>()->SetPosition(100, 400);
	dae::servicelocator::GetSoundSystem().Stop();
	dae::servicelocator::GetSoundSystem().Play(dae::make_sdbm_hash("TheEnd"), 100, 0);

	input.AddKeyboardCommand(SDLK_RETURN,std::make_unique<GotoStartScreen>());
	input.AddGamepad(std::make_unique<GamepadInput>(0));
	input.AddGamepadCommand(0,dae::GamepadData::AButton, std::make_unique<GotoStartScreen>());

	scene.Add(std::move(go));
}

void LoadStartScreen()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("StartScreen");
	auto& input = dae::InputManager::GetInstance();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 24);
	auto TitleFont = dae::ResourceManager::GetInstance().LoadFont("Font.ttf", 40);
	input.AddKeyboardCommand(SDLK_F2, std::make_unique<MuteMusic>());
	auto Title = std::make_unique<dae::GameObject>();
	Title->AddComponent<dae::TextComponent>("MS. PACMAN", TitleFont);
	Title->GetComponent<dae::TextComponent>()->SetPosition(120, 200);
	auto GoSolo = std::make_unique<dae::GameObject>();
	GoSolo->AddComponent<dae::TextComponent>(	"- Solo		 Press A", font);
	GoSolo->GetComponent<dae::TextComponent>()->SetPosition(120, 350);
	auto GoCoop = std::make_unique<dae::GameObject>();
	GoCoop->AddComponent<dae::TextComponent>(	"- Coop		 Press B", font);
	GoCoop->GetComponent<dae::TextComponent>()->SetPosition(120, 380);
	auto GoVersus = std::make_unique<dae::GameObject>();
	GoVersus->AddComponent<dae::TextComponent>(	"- Versus	Press X", font);
	GoVersus->GetComponent<dae::TextComponent>()->SetPosition(120, 410);
	input.AddGamepad(std::make_unique<GamepadInput>(0));
	input.AddKeyboardCommand(SDLK_a, std::make_unique<SetSolo>());
	input.AddGamepadCommand(0, dae::GamepadData::AButton, std::make_unique<SetSolo>());
	input.AddKeyboardCommand(SDLK_b, std::make_unique<SetCoop>());
	input.AddGamepadCommand(0, dae::GamepadData::BButton, std::make_unique<SetCoop>());
	input.AddKeyboardCommand(SDLK_x, std::make_unique<SetVersus>());
	input.AddGamepadCommand(0, dae::GamepadData::XButton, std::make_unique<SetVersus>());

	scene.Add(std::move(GoSolo));
	scene.Add(std::move(GoCoop));
	scene.Add(std::move(GoVersus));
	scene.Add(std::move(Title));
	dae::servicelocator::GetSoundSystem().Play(dae::make_sdbm_hash("GameStart"), 100, 0);
}

void load()
{
	dae::servicelocator::GetSoundSystem().Load("Game Start.wav", dae::make_sdbm_hash("GameStart"), false);
	dae::servicelocator::GetSoundSystem().Load("Ambience.wav", dae::make_sdbm_hash("Ambience"), true);
	dae::servicelocator::GetSoundSystem().Load("Lose Life.wav", dae::make_sdbm_hash("LoseLife"), false);
	dae::servicelocator::GetSoundSystem().Load("They Meet.wav", dae::make_sdbm_hash("TheyMeet"), false);
	dae::servicelocator::GetSoundSystem().Load("The Chase.wav", dae::make_sdbm_hash("TheChase"), false);
	dae::servicelocator::GetSoundSystem().Load("Junior.wav", dae::make_sdbm_hash("Junior"), false);
	dae::servicelocator::GetSoundSystem().Load("The End.wav", dae::make_sdbm_hash("TheEnd"), false);
	LoadStartScreen();
	
}

int main(int, char* []) {
	std::filesystem::path data_location = "./Data/";
	if (!std::filesystem::exists(data_location))
		data_location = "../Data/";
	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}
