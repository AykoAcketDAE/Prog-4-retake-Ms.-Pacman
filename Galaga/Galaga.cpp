#include <SDL.h>
#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Galaga.h"
#include "Background.h"
#include "Player.h"
#include "Bullet.h"
#include "GalagaCommands.h"
#include "CollisionComponent.h"
#include "Boss.h"
#include "BossCollisionComponent.h"
#include "SpriteAnimation.h"
#include "Enemy.h"
#include "HealthComponent.h"
#include "EnemyCollisionComponent.h"
#include "PlayerCollisionComponent.h"
#include "EnemySpawning.h"
#include "InputManager.h"

void loadWave(dae::Scene& scene)
{
	auto bullet = std::make_unique<dae::GameObject>();
	auto bullet2 = std::make_unique<dae::GameObject>();
	auto Player = std::make_unique<dae::GameObject>();

	auto& inputmanager = dae::InputManager::GetInstance();
	//background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("Background.png");
	go->AddComponent<Background>();
	scene.Add(std::move(go));

	SDL_FRect src = { 0.f,0.f,48.f,16.f };
	SDL_FRect dst = { 0.f, 0.f, 96.f, 32.f };
	auto health = std::make_unique<dae::GameObject>();
	health->AddComponent<dae::RenderComponent>("Health.png", src, dst);
	health->AddComponent<HealthComponent>(4);
	health->SetLocalPosition({ 624.f,300.f,0.f });
	
	//Bindings
	inputmanager.AddKeyboardCommand(SDLK_F2, std::make_unique<dae::MuteSound>());
	inputmanager.AddKeyboardCommand(SDLK_d, std::make_unique<dae::MoveCommand>(Player.get(), glm::vec3{ 1.f,0.f,0.f }, 200.f));
	inputmanager.AddKeyboardCommand(SDLK_a, std::make_unique<dae::MoveCommand>(Player.get(), glm::vec3{ -1.f,0.f,0.f }, 200.f));
	inputmanager.AddKeyboardCommand(SDLK_SPACE, std::make_unique<SpawnBullet>(Player.get()));
	inputmanager.AddGamepad(std::make_unique<GamepadInput>(0));
	inputmanager.AddGamepadCommand(0,dae::GamepadData::DPadRight, std::make_unique<dae::MoveCommand>(Player.get(), glm::vec3{ 1.f,0.f,0.f }, 200.f));
	inputmanager.AddGamepadCommand(0, dae::GamepadData::DPadLeft, std::make_unique<dae::MoveCommand>(Player.get(), glm::vec3{ -1.f,0.f,0.f }, 200.f));
	inputmanager.AddGamepadCommand(0, dae::GamepadData::AButton, std::make_unique<SpawnBullet>(Player.get()));
	
	dae::servicelocator::GetSoundSystem().Load("Start.wav", dae::make_sdbm_hash("main"), true);

	dae::servicelocator::GetSoundSystem().Play(static_cast<dae::sound_id>(dae::make_sdbm_hash("main")), 100, 100);

	dae::servicelocator::GetSoundSystem().Load("PlayerShoot.wav", dae::make_sdbm_hash("PlayerShoot"), false);
	dae::servicelocator::GetSoundSystem().Load("ButterflyDead.wav", dae::make_sdbm_hash("ButterflyDead"), false);
	dae::servicelocator::GetSoundSystem().Load("BeeDead.wav", dae::make_sdbm_hash("BeeDead"), false);
	
	src = { 0.f,0.f,3.f,8.f };
	dst = { 24.f, 24.f, 9.f,24.f };
	
	bullet->AddComponent<dae::RenderComponent>("Bullets.png", src, dst);
	bullet->AddComponent<Bullet>(24.f);

	SDL_FRect HitBox{ 0.f,0.f,9.f,24.f };
	bullet->AddComponent<BulletCollisionComponent>(HitBox);
	bullet->SetParent(Player.get(), false);

	bullet2->AddComponent<dae::RenderComponent>("Bullets.png", src, dst);
	bullet2->AddComponent<Bullet>(24.f);
	bullet2->AddComponent<BulletCollisionComponent>(HitBox);
	bullet2->SetParent(Player.get(), false);

	
	src = { 80.f,0.f,16.f,16.f };
	dst = { -24.f, 0.f, 48.f, 48.f };
	Player->AddComponent<dae::RenderComponent>("Sprites.png", src, dst);
	Player->AddComponent<PlayerComponent>();
	Player->AddComponent<PlayerCollisionComponent>(dst,health.get());
	Player->SetLocalPosition({ 320.f,600.f,0.f });
	scene.Add(std::move(Player));
	scene.Add(std::move(bullet));
	scene.Add(std::move(bullet2));
	scene.Add(std::move(health));

	//enemies

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<EnemySpawning>(scene);
	scene.Add(std::move(go));
}
void loadMenu(dae::Scene& scene,std::shared_ptr<dae::Font> font)
{
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("Background.png");
	go->AddComponent<Background>();
	scene.Add(std::move(go));

	auto logo = std::make_unique<dae::GameObject>();
	SDL_FRect src = { 0.f,0.f,119.f,67.f };
	SDL_FRect dst = { 0.f, 0.f, 357.f, 201.f };
	logo->AddComponent<dae::RenderComponent>("Logo.png", src, dst);
	logo->SetLocalPosition({ 200,50,0 });
	scene.Add(std::move(logo));

	auto text1 = std::make_unique<dae::GameObject>();
	text1->AddComponent<dae::TextComponent>("1 - player",font);
	text1->GetComponent<dae::TextComponent>()->SetPosition(200, 300);
	scene.Add(std::move(text1));

	auto text2 = std::make_unique<dae::GameObject>();
	text2->AddComponent<dae::TextComponent>("2 - player", font);
	text2->GetComponent<dae::TextComponent>()->SetPosition(200, 400);
	scene.Add(std::move(text2));
}
void loadWave2(dae::Scene& scene)
{
	auto bullet = std::make_unique<dae::GameObject>();
	auto bullet2 = std::make_unique<dae::GameObject>();
	auto Player = std::make_unique<dae::GameObject>();
	auto bullet3 = std::make_unique<dae::GameObject>();
	auto bullet4 = std::make_unique<dae::GameObject>();
	auto Player2 = std::make_unique<dae::GameObject>();

	auto& inputmanager = dae::InputManager::GetInstance();
	//background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("Background.png");
	go->AddComponent<Background>();
	scene.Add(std::move(go));

	SDL_FRect src = { 0.f,0.f,48.f,16.f };
	SDL_FRect dst = { 0.f, 0.f, 96.f, 32.f };
	auto health = std::make_unique<dae::GameObject>();
	health->AddComponent<dae::RenderComponent>("Health.png", src, dst);
	health->AddComponent<HealthComponent>(4);
	health->SetLocalPosition({ 624.f,300.f,0.f });

	//Bindings
	inputmanager.AddKeyboardCommand(SDLK_F2, std::make_unique<dae::MuteSound>());
	inputmanager.AddKeyboardCommand(SDLK_d, std::make_unique<dae::MoveCommand>(Player.get(), glm::vec3{ 1.f,0.f,0.f }, 200.f));
	inputmanager.AddKeyboardCommand(SDLK_a, std::make_unique<dae::MoveCommand>(Player.get(), glm::vec3{ -1.f,0.f,0.f }, 200.f));
	inputmanager.AddKeyboardCommand(SDLK_SPACE, std::make_unique<SpawnBullet>(Player.get()));
	inputmanager.AddGamepad(std::make_unique<GamepadInput>(0));
	inputmanager.AddGamepadCommand(0, dae::GamepadData::DPadRight, std::make_unique<dae::MoveCommand>(Player2.get(), glm::vec3{ 1.f,0.f,0.f }, 200.f));
	inputmanager.AddGamepadCommand(0, dae::GamepadData::DPadLeft, std::make_unique<dae::MoveCommand>(Player2.get(), glm::vec3{ -1.f,0.f,0.f }, 200.f));
	inputmanager.AddGamepadCommand(0, dae::GamepadData::AButton, std::make_unique<SpawnBullet>(Player2.get()));
	//inputmanager.AddGamepad(std::make_unique<GamepadInput>(1));
	//inputmanager.AddGamepadCommand(1, dae::GamepadData::DPadRight, std::make_unique<dae::MoveCommand>(Player2.get(), glm::vec3{ 1.f,0.f,0.f }, 200.f));
	//inputmanager.AddGamepadCommand(1, dae::GamepadData::DPadLeft, std::make_unique<dae::MoveCommand>(Player2.get(), glm::vec3{ -1.f,0.f,0.f }, 200.f));
	//inputmanager.AddGamepadCommand(1, dae::GamepadData::AButton, std::make_unique<SpawnBullet>(Player2.get()));
	
	dae::servicelocator::GetSoundSystem().Load("Start.wav", dae::make_sdbm_hash("main"), true);
	dae::servicelocator::GetSoundSystem().Load("PlayerShoot.wav", dae::make_sdbm_hash("PlayerShoot"), false);
	dae::servicelocator::GetSoundSystem().Load("ButterflyDead.wav", dae::make_sdbm_hash("ButterflyDead"), false);
	dae::servicelocator::GetSoundSystem().Load("BeeDead.wav", dae::make_sdbm_hash("BeeDead"), false);
	//inputmanager.AddKeyboardCommand(SDLK_f, std::make_unique<dae::PlayMusic>(static_cast<dae::sound_id>(dae::make_sdbm_hash("main")), 100, 100));
	//inputmanager.AddKeyboardCommand(SDLK_e, std::make_unique<dae::PlayEffect>(static_cast<dae::sound_id>(dae::make_sdbm_hash("PlayerShoot")), 100, 0));
	src = { 0.f,0.f,3.f,8.f };
	dst = { 24.f, 24.f, 9.f,24.f };

	bullet->AddComponent<dae::RenderComponent>("Bullets.png", src, dst);
	bullet->AddComponent<Bullet>(24.f);
	bullet2->AddComponent<dae::RenderComponent>("Bullets.png", src, dst);
	bullet2->AddComponent<Bullet>(24.f);
	bullet3->AddComponent<dae::RenderComponent>("Bullets.png", src, dst);
	bullet3->AddComponent<Bullet>(24.f);
	bullet4->AddComponent<dae::RenderComponent>("Bullets.png", src, dst);
	bullet4->AddComponent<Bullet>(24.f);
	SDL_FRect HitBox{ 0.f,0.f,9.f,24.f };
	bullet->AddComponent<BulletCollisionComponent>(HitBox);
	bullet->SetParent(Player.get(), false);
	bullet2->AddComponent<BulletCollisionComponent>(HitBox);
	bullet2->SetParent(Player.get(), false);
	bullet3->AddComponent<BulletCollisionComponent>(HitBox);
	bullet3->SetParent(Player2.get(), false);
	bullet4->AddComponent<BulletCollisionComponent>(HitBox);
	bullet4->SetParent(Player2.get(), false);



	src = { 80.f,0.f,16.f,16.f };
	dst = { -24.f, 0.f, 48.f, 48.f };
	Player->AddComponent<dae::RenderComponent>("Sprites.png", src, dst);
	Player->AddComponent<PlayerComponent>();
	Player->AddComponent<PlayerCollisionComponent>(dst, health.get());
	Player->SetLocalPosition({ 320.f,600.f,0.f });
	Player2->AddComponent<dae::RenderComponent>("Sprites.png", src, dst);
	Player2->AddComponent<PlayerComponent>();
	Player2->AddComponent<PlayerCollisionComponent>(dst, health.get());
	Player2->SetLocalPosition({ 320.f,600.f,0.f });
	scene.Add(std::move(Player));
	scene.Add(std::move(bullet));
	scene.Add(std::move(bullet2));
	scene.Add(std::move(Player2));
	scene.Add(std::move(bullet3));
	scene.Add(std::move(bullet4));
	scene.Add(std::move(health));

	//enemies

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<EnemySpawning>(scene);
	scene.Add(std::move(go));
}
void load()
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("font.ttf", 24);
	//auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	//load game
	auto& galaga = dae::SceneManager::GetInstance().CreateScene("1PGalaga");
	loadWave(galaga);
	auto& coop = dae::SceneManager::GetInstance().CreateScene("2PGalaga");
	loadWave2(coop);
	//load menu
	auto& MenuScene = dae::SceneManager::GetInstance().CreateScene("Menu");
	auto& inputmanager = dae::InputManager::GetInstance();
	inputmanager.AddKeyboardCommand(SDLK_F2, std::make_unique<dae::MuteSound>());
	inputmanager.AddKeyboardCommand(SDLK_1, std::make_unique<SwitchToScene>(nullptr, "1PGalaga"));
	inputmanager.AddKeyboardCommand(SDLK_2, std::make_unique<SwitchToScene>(nullptr, "2PGalaga"));
	loadMenu(MenuScene,font);
};

int main(int, char* []) {
	std::filesystem::path data_location = "./Data/";
	if (!std::filesystem::exists(data_location))
		data_location = "../Data/";
	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}