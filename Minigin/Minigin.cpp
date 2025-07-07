#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <thread>
#include "Minigin.h"
#include "InputManager.h"
//#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "SoundSystem.h"
#include "Scene.h"

std::unique_ptr<dae::SoundSystem> dae::servicelocator::_ss_instance{ std::make_unique<dae::null_sound_system>() };

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::filesystem::path& dataPath)
{
	
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		throw std::runtime_error(std::string("Failed to initialize SDL_Mixer: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		720,
		720,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);
	ResourceManager::GetInstance().Init(dataPath);
	servicelocator::RegisterSoundSystem(std::make_unique<SDL_SoundSystem>(dataPath));
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	bool doContinue = true;

	while (doContinue)
	{
		auto tempTime = std::chrono::high_resolution_clock::now();

		doContinue = input.ProcessInput();
		sceneManager.Update();
		renderer.Render();
		sceneManager.Cleanup();
		auto lag = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - tempTime);

		if (lag.count() <= 16000)
		{
			auto times = 16000 - lag.count();
			std::this_thread::sleep_for(std::chrono::microseconds(times));
		}
		auto passedtime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - tempTime);
		Time::GetInstance().SetDeltaTime(static_cast<float>(passedtime.count()));
	}
}
