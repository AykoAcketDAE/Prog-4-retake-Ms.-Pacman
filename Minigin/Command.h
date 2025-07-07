#pragma once
#include "GameObject.h"
#include "SoundSystem.h"
#include "glm.hpp"
#include "SDL_keycode.h"
namespace dae
{
	class GameObject;
	class Command
	{
	public:
		explicit Command() {};
		virtual ~Command() {};
		virtual void Execute() = 0;
	};

	class MoveCommand final: public Command
	{
	public:
		MoveCommand(GameObject* go, glm::vec3 direction,float speed);
		virtual ~MoveCommand() = default;
		void Execute() override;

	private:
		bool m_State{false};
		GameObject* m_GameObject{ nullptr };
		glm::vec3 m_Direction{};
		float m_Speed{1};
	};

	class PlayMusic final :public Command
	{
		sound_id m_SoundId{};
		int m_Volume{};
		int m_Loops{};
	public:
		PlayMusic(const sound_id id, const int volume, const int loops);
		virtual ~PlayMusic() = default;
		void Execute() override;
	};

	class PlayEffect final :public Command
	{
		sound_id m_SoundId{};
		int m_Volume{};
		int m_Loops{};
	public:
		PlayEffect(const sound_id id, const int volume, const int loops);
		virtual ~PlayEffect() = default;
		void Execute() override;
	};
	class MuteSound final :public Command
	{
	public:
		MuteSound();
		virtual ~MuteSound() = default;
		void Execute() override;
	};
}

