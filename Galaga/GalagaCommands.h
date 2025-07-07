#pragma once
#include "Command.h"
#include "GameObject.h"
#include "HealthComponent.h"
//class HudControls : public dae::Command
//{	
//public:
//	HudControls(dae::GameObject* go, int direction, bool pressed);
//	virtual ~HudControls() = default;
//	void Execute() override;
//
//private:
//	bool m_Pressed{ false };
//	dae::GameObject* m_GameObject{ nullptr };
//	int m_Direction{};
//};

class SpawnBullet : public dae::Command
{
public:
	SpawnBullet(dae::GameObject* go);
	virtual ~SpawnBullet() = default;
	void Execute() override;

private:
	dae::GameObject* m_GameObject{ nullptr };
};

class LoseLife : public dae::Command
{
public:
	LoseLife(dae::GameObject* go);
	virtual ~LoseLife() = default;
	void Execute() override;

private:
	dae::GameObject* m_GameObject{ nullptr };
	
};

class SwitchToScene : public dae::Command
{
public:
	SwitchToScene(dae::GameObject* go, const std::string& scene);
	virtual ~SwitchToScene() = default;
	void Execute() override;

private:
	dae::GameObject* m_GameObject{ nullptr };
	std::string m_Scene{};
};