#pragma once
#include "Singleton.h"
#include <map>
#include "Events.h"
#include <memory>
#include <filesystem>
#include <thread>

namespace dae
{
	//TODO : 
	// free all sound chunks and music at the end
	// stop, resume and pause should be able to do it for individual sounds

	using sound_id = unsigned int;

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(const sound_id id, const int volume, const int loops) = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;
		virtual void Mute() = 0;
		virtual void Resume() = 0;
		virtual void Load(const std::string& path, sound_id id, bool music) = 0;
		virtual bool IsLoaded(const sound_id id) = 0;
	};

	class null_sound_system final : public SoundSystem
	{
		void Play(const sound_id, const int, const int) override {};
		void Pause() override {};
		void Stop() override {};
		void Mute() override {};
		void Resume() override {};
		void Load(const std::string& , sound_id ,bool) override {};
		bool IsLoaded(const sound_id) override { return false; };
	};

	class servicelocator final
	{
		static std::unique_ptr<SoundSystem> _ss_instance;
	public:
		static SoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss);
	};

	class SDL_SoundSystem final : public SoundSystem
	{
		class SDL_SoundSystemImpl;
		std::unique_ptr<SDL_SoundSystemImpl> m_SDL_SoundSystemPimpl;
		std::filesystem::path m_DataPath;
		std::jthread m_Thread;
	public:
		SDL_SoundSystem(const std::filesystem::path& data);
		~SDL_SoundSystem() override;
		
		void Play(const sound_id id, const int volume, const int loops) override;
		void Pause() override;
		void Stop() override;
		void Mute() override;
		void Resume() override;
		void Load(const std::string& path, sound_id id,bool music) override;
		bool IsLoaded(const sound_id id) override;
	};
}	

