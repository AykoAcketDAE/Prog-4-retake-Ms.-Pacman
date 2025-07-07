#pragma once
#include <iostream>
#include <vector>
class GamepadInput
{
	
	class GamepadImpl;
	std::unique_ptr<GamepadImpl> m_GamepadPimpl;
	std::vector<unsigned int> m_Buttons;
public:
	GamepadInput(int controllerIndex);
	~GamepadInput();
	bool ProcessControllerInput();

	int GetIndex() const;
	unsigned int GetButtonsUp() ;
	unsigned int GetButtonsDown();
	std::vector<unsigned int> GetButtonsPressed();
};



