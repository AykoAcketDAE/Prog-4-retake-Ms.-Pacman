#include "SoundSystem.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <mutex>
#include "Events.h"
#include <queue>
#include <atomic>
namespace fs = std::filesystem;
struct Sound
{
	dae::sound_id id;
	const int volume;
	const int loops;
};
class dae::SDL_SoundSystem::SDL_SoundSystemImpl
{
public:

	bool Queue()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		while (m_IsRunning)
		{
			
			m_CondVar.wait(lock, [this] { 
				return not m_Queue.empty() or not m_IsRunning;
				});
			if (not m_IsRunning and m_Queue.empty())
				return true;

			Sound sound = m_Queue.front();
			m_Queue.pop();
			lock.unlock();

			if (IsLoaded(sound.id))
			{
				if (Mix_PlayingMusic() == 0)
				{
					
					Mix_PlayMusic(m_MusicMap[sound.id], sound.loops);
				}
				Mix_PlayChannel(-1, m_EffectMap[sound.id], sound.loops);
			}
			lock.lock();
		}
		return true;
	}
	void Pause()
	{
		Mix_PauseMusic();
	};
	void Stop() 
	{
		Mix_HaltMusic();
	};
	void Resume() 
	{
		Mix_ResumeMusic();
	};
	void LoadMusic(const std::string& path, sound_id id, const std::filesystem::path& data)
	{
		const auto fullPath = data/path;

		m_MusicMap[id] = Mix_LoadMUS(fullPath.generic_string().c_str());
	};
	void LoadEffect(const std::string& path, sound_id id, const std::filesystem::path& data)
	{
		const auto fullPath = data / path;

		m_EffectMap[id] = Mix_LoadWAV(fullPath.generic_string().c_str());
	};
	bool IsLoaded(sound_id id) {
		if (m_MusicMap[id] != nullptr or m_EffectMap[id] != nullptr) return true;
		
		std::cout << "\n no sound loaded";
		return false; 
	};
	void AddToQueue(Sound sound)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Queue.push(sound);
		m_CondVar.notify_one();
	}
	void Mute()
	{
		
		if (m_Volume == 100) m_Volume = 0;
		else m_Volume = 100;
		Mix_MasterVolume(m_Volume);
		Mix_VolumeMusic(m_Volume);
	}
	void Close()
	{
		for (auto& [temp, music] : m_MusicMap)
		{
			Mix_FreeMusic(music);
			music = nullptr;
		}
		for (auto& [temp, effect] : m_EffectMap)
		{
			Mix_FreeChunk(effect);
			effect = nullptr;
		}
		{	
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_IsRunning = false;
		}

		m_CondVar.notify_all();

		m_MusicMap.clear();
		m_EffectMap.clear();
	};

private:
	int m_Volume{100};
	std::condition_variable m_CondVar{};
	std::mutex m_Mutex{};
	std::queue<Sound> m_Queue{};
	std::map<sound_id, Mix_Music*> m_MusicMap;
	std::map<sound_id, Mix_Chunk*> m_EffectMap;

	bool m_IsRunning{ true };
};

dae::SDL_SoundSystem::SDL_SoundSystem(const std::filesystem::path& data)
{
	
	m_DataPath = data;
	m_SDL_SoundSystemPimpl = std::make_unique<SDL_SoundSystemImpl>();
	std::jthread thread(&dae::SDL_SoundSystem::SDL_SoundSystemImpl::Queue,m_SDL_SoundSystemPimpl.get());
	m_Thread = std::move(thread);
}

dae::SDL_SoundSystem::~SDL_SoundSystem()
{
	
	m_SDL_SoundSystemPimpl->Close();
	m_Thread.join();
}

void dae::SDL_SoundSystem::Play(const sound_id id, const int volume, const int loops)
{
	m_SDL_SoundSystemPimpl->AddToQueue({ id,volume,loops });
}

void dae::SDL_SoundSystem::Pause()
{
	m_SDL_SoundSystemPimpl->Pause();
}

void dae::SDL_SoundSystem::Stop()
{
	m_SDL_SoundSystemPimpl->Stop();
}

void dae::SDL_SoundSystem::Mute()
{
	m_SDL_SoundSystemPimpl->Mute();
}

void dae::SDL_SoundSystem::Resume()
{
	m_SDL_SoundSystemPimpl->Resume();
}

void dae::SDL_SoundSystem::Load(const std::string& path, sound_id id, bool music)
{
	if(music) 
		m_SDL_SoundSystemPimpl->LoadMusic(path, id, m_DataPath);
	else 
		m_SDL_SoundSystemPimpl->LoadEffect(path, id, m_DataPath);
}

bool dae::SDL_SoundSystem::IsLoaded(const sound_id id)
{
	return m_SDL_SoundSystemPimpl->IsLoaded(id);
}


dae::SoundSystem& dae::servicelocator::GetSoundSystem()
{
	return *_ss_instance;
}

void dae::servicelocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
{
	_ss_instance = ss == nullptr ? std::make_unique<null_sound_system>() : std::move(ss);
}
