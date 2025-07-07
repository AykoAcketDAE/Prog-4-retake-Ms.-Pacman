#include "GamepadInput.h"
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")

class GamepadInput::GamepadImpl
{
	int m_ControllerIndex{};

	XINPUT_STATE currentState{};
	XINPUT_STATE previousState{};

	unsigned int buttonsPressedThisFrame{};
	unsigned int buttonsReleasedThisFrame{};
public:
	GamepadImpl(int controllerIndex) :m_ControllerIndex{controllerIndex} {};
	~GamepadImpl() = default;
	bool ProcessControllerInput();
	int GetIndex() const { return m_ControllerIndex; }
	unsigned int GetButtonsUp();
	unsigned int GetButtonsDown();
	unsigned int GetButtonsPressed();
};
bool GamepadInput::GamepadImpl::ProcessControllerInput()
{
	CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
	ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &currentState);

	auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
	buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
	return true;
}

unsigned int GamepadInput::GamepadImpl::GetButtonsUp()
{
	return buttonsReleasedThisFrame;
}

unsigned int GamepadInput::GamepadImpl::GetButtonsDown()
{
	return buttonsPressedThisFrame;
}

unsigned int GamepadInput::GamepadImpl::GetButtonsPressed()
{
	return static_cast<unsigned int>(currentState.Gamepad.wButtons);
}

GamepadInput::GamepadInput(int controllerIndex)
	:m_GamepadPimpl{ std::make_unique<GamepadImpl>(controllerIndex) } 
{

}
GamepadInput::~GamepadInput() {}

bool GamepadInput::ProcessControllerInput()
{
	return m_GamepadPimpl->ProcessControllerInput();
}

int GamepadInput::GetIndex() const
{
	return m_GamepadPimpl->GetIndex();
}

unsigned int GamepadInput::GetButtonsUp()
{
	return m_GamepadPimpl->GetButtonsUp();
}

unsigned int GamepadInput::GetButtonsDown()
{
	return m_GamepadPimpl->GetButtonsDown();
}

std::vector<unsigned int> GamepadInput::GetButtonsPressed()
{
	m_Buttons.clear();
	unsigned int pressed = m_GamepadPimpl->GetButtonsPressed();
	for (int index{1}; index < 32768; index *= 2)
	{
		if (index == 16) index = 4096;
		if (pressed & index) m_Buttons.push_back(index);
	}

	return m_Buttons;
}
