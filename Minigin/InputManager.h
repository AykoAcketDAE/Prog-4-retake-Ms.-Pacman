#pragma once
#include "Singleton.h"
#include <memory>
#include <SDL.h>
#include <map>
#include <vector>
#include "Command.h"
#include "GamepadInput.h"
#include "Scene.h"

class GamepadInput;
namespace dae
{
	enum class GamepadData :unsigned int
	{
		DPadUp = 0x0001,
		DPadDown = 0x0002,
		DPadLeft = 0x0004,
		DPadRight = 0x0008,
		AButton = 0x1000,
		BButton = 0x2000,
		XButton = 0x4000,
		YButton = 0x8000
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void AddKeyboardCommand(SDL_Keycode key, std::unique_ptr<Command> command);
		void AddGamepadCommand(int index,GamepadData key, std::unique_ptr<Command> command);
		void AddGamepad(std::unique_ptr<GamepadInput> gamepad);
	private:

		struct GamepadCommand
		{
			GamepadData button;
			std::unique_ptr<Command> command;
		};
		

		std::map<Scene*,std::map<SDL_Keycode, std::unique_ptr<Command>>> m_KeyboardCommands{};
		std::map<Scene*,std::vector<std::unique_ptr<GamepadInput>>> m_Gamepads{};
		std::map<Scene*,std::vector<std::vector<GamepadCommand>>> m_GamepadCommands{};
	};

}
