#include "InputManager.h"
#include <SDL.h>

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type){
		case SDL_QUIT:
			return false;
		default:
			break;
		}
		
		
		//if (auto search = m_KeyboardCommands.find(); search != m_KeyboardCommands.end())
		//{
		//	search->second->Execute();
		//}
		// etc...
	}

	const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
	
	for (const auto& commands : m_KeyboardCommands[dae::SceneManager::GetInstance().GetCurrentScene()])
	{
		SDL_Scancode scancode = SDL_GetScancodeFromKey(commands.first);
		if (currentKeyStates[scancode])
		{
			commands.second->Execute();
		}
	}
	for (auto& gamepad : m_Gamepads[dae::SceneManager::GetInstance().GetCurrentScene()]) gamepad->ProcessControllerInput();
	for (int index{};index <m_Gamepads.size();++index)
	{
		if (m_Gamepads[dae::SceneManager::GetInstance().GetCurrentScene()].empty())continue;
		if (m_Gamepads[dae::SceneManager::GetInstance().GetCurrentScene()].size() <= index)continue;
		for (auto& button : m_Gamepads[dae::SceneManager::GetInstance().GetCurrentScene()][index]->GetButtonsPressed())
		{
			for(auto& command: m_GamepadCommands[dae::SceneManager::GetInstance().GetCurrentScene()][index])
			if (static_cast<unsigned int>(command.button) == button)
			{
				command.command->Execute();
			}
		}
	}
	

	return true;

}

void dae::InputManager::AddKeyboardCommand(SDL_Keycode key, std::unique_ptr<Command> command)
{
	m_KeyboardCommands[dae::SceneManager::GetInstance().GetCurrentScene()][key] = std::move(command);
}

void dae::InputManager::AddGamepadCommand(int index,GamepadData key, std::unique_ptr<Command> command)
{
	
	m_GamepadCommands[dae::SceneManager::GetInstance().GetCurrentScene()][index].push_back({ key ,std::move(command) });
}

void dae::InputManager::AddGamepad(std::unique_ptr<GamepadInput> gamepad)
{
	m_Gamepads[dae::SceneManager::GetInstance().GetCurrentScene()].push_back(std::move(gamepad));
	m_GamepadCommands[dae::SceneManager::GetInstance().GetCurrentScene()].resize(m_Gamepads.size());
}
