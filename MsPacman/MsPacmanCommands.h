#pragma once
#include "Command.h"
#include "GameObject.h"

class SetPlayerDirection final : public dae::Command
{
public:
	SetPlayerDirection(dae::GameObject* go, glm::vec2 direction);
	virtual ~SetPlayerDirection() = default;
	void Execute() override;

private:
	bool m_State{ false };
	dae::GameObject* m_GameObject{ nullptr };
	glm::vec2 m_Direction{};
};

class AddPelletScore final : public dae::Command
{
public:
	AddPelletScore(dae::GameObject* playerGo, dae::GameObject* scoreGo);
	virtual ~AddPelletScore() = default;
	void Execute() override;

private:
	
	dae::GameObject* m_PlayerGo{ nullptr };
	dae::GameObject* m_ScoreGo{ nullptr };
	const int m_Score{ 10 };
};
class AddPowerPelletScore final : public dae::Command
{
public:
	AddPowerPelletScore(dae::GameObject* playerGo, dae::GameObject* scoreGo);
	virtual ~AddPowerPelletScore() = default;
	void Execute() override;

private:

	dae::GameObject* m_PlayerGo{ nullptr };
	dae::GameObject* m_ScoreGo{ nullptr };
	const int m_Score{ 10 };
};

class SetGhostToScatter final : public dae::Command
{
public:
	SetGhostToScatter(dae::GameObject* Ghost);
	virtual ~SetGhostToScatter() = default;
	void Execute() override;

private:

	dae::GameObject* m_Ghost{ nullptr };
	
};
class SetGhostToChase final : public dae::Command
{
public:
	SetGhostToChase(dae::GameObject* Ghost);
	virtual ~SetGhostToChase() = default;
	void Execute() override;

private:

	dae::GameObject* m_Ghost{ nullptr };
	
};
class SetGhostToSpawn final : public dae::Command
{
public:
	SetGhostToSpawn(dae::GameObject* Ghost);
	virtual ~SetGhostToSpawn() = default;
	void Execute() override;

private:

	dae::GameObject* m_Ghost{ nullptr };
	
};
class SkipLevel final : public dae::Command
{
public:
	SkipLevel() {};
	virtual ~SkipLevel() = default;
	void Execute() override;

private:

	

};