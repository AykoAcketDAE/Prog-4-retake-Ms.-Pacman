#include "Command.h"
#include "Timer.h"
#include "SoundSystem.h"
dae::MoveCommand::MoveCommand(GameObject* go, glm::vec3 direction,float speed):
	Command(), m_GameObject{go}, m_Direction{direction}, m_Speed{speed}
{

}

void dae::MoveCommand::Execute()
{
	m_GameObject->SetLocalPosition(m_GameObject->GetPosition() + (m_Direction * m_Speed * Time::GetInstance().GetDeltaTime()));
}

dae::PlayMusic::PlayMusic(const sound_id id, const int volume, const int loops):
	Command(),m_SoundId{id},m_Volume{volume}, m_Loops{loops}
{

}

void dae::PlayMusic::Execute()
{
	if(servicelocator::GetSoundSystem().IsLoaded(m_SoundId))
		servicelocator::GetSoundSystem().Play(m_SoundId, m_Volume,m_Loops);
}

dae::PlayEffect::PlayEffect(const sound_id id, const int volume, const int loops):
	Command(), m_SoundId{ id }, m_Volume{ volume }, m_Loops{ loops }
{

}
void dae::PlayEffect::Execute()
{
	if (servicelocator::GetSoundSystem().IsLoaded(m_SoundId))
		servicelocator::GetSoundSystem().Play(m_SoundId, m_Volume, m_Loops);
}

dae::MuteSound::MuteSound()
	:Command()
{
}

void dae::MuteSound::Execute()
{
	servicelocator::GetSoundSystem().Mute();
}
