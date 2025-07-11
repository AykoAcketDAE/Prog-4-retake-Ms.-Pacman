#include "Player.h"

Player::Player(dae::GameObject* owner, dae::RenderComponent* renderComp)
	:BaseComponent(owner),m_RenderComp{renderComp}
{

}

void Player::Update()
{
}

void Player::Render() const
{
}
