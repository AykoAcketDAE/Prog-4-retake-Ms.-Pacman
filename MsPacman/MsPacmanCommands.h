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
	dae::GameObject* m_GameObject{};
	glm::vec2 m_Direction{};
};

class AddPelletScore final : public dae::Command
{
public:
	AddPelletScore(dae::GameObject* playerGo, dae::GameObject* scoreGo);
	virtual ~AddPelletScore() = default;
	void Execute() override;

private:
	
	dae::GameObject* m_PlayerGo{};
	dae::GameObject* m_ScoreGo{};
	const int m_Score{ 10 };
};
class AddPowerPelletScore final : public dae::Command
{
public:
	AddPowerPelletScore(dae::GameObject* playerGo, dae::GameObject* scoreGo);
	virtual ~AddPowerPelletScore() = default;
	void Execute() override;

private:

	dae::GameObject* m_PlayerGo{};
	dae::GameObject* m_ScoreGo{};
	const int m_Score{ 10 };
};
class AddOneGhost final : public dae::Command
{
public:
	AddOneGhost(dae::GameObject* playerGo, dae::GameObject* scoreGo);
	virtual ~AddOneGhost() = default;
	void Execute() override;

private:

	dae::GameObject* m_PlayerGo{};
	dae::GameObject* m_ScoreGo{};
	const int m_Score{ 200 };
};
class AddTwoGhost final : public dae::Command
{
public:
	AddTwoGhost(dae::GameObject* playerGo, dae::GameObject* scoreGo);
	virtual ~AddTwoGhost() = default;
	void Execute() override;

private:

	dae::GameObject* m_PlayerGo{};
	dae::GameObject* m_ScoreGo{};
	const int m_Score{ 400 };
};
class AddThreeGhost final : public dae::Command
{
public:
	AddThreeGhost(dae::GameObject* playerGo, dae::GameObject* scoreGo);
	virtual ~AddThreeGhost() = default;
	void Execute() override;

private:

	dae::GameObject* m_PlayerGo{};
	dae::GameObject* m_ScoreGo{};
	const int m_Score{ 800 };
};
class AddFourGhost final : public dae::Command
{
public:
	AddFourGhost(dae::GameObject* playerGo, dae::GameObject* scoreGo);
	virtual ~AddFourGhost() = default;
	void Execute() override;

private:

	dae::GameObject* m_PlayerGo{};
	dae::GameObject* m_ScoreGo{};
	const int m_Score{ 1600 };
};

class SetGhostToScatter final : public dae::Command
{
public:
	SetGhostToScatter(dae::GameObject* Ghost);
	virtual ~SetGhostToScatter() = default;
	void Execute() override;

private:

	dae::GameObject* m_Ghost{};
	
};
class SetGhostToChase final : public dae::Command
{
public:
	SetGhostToChase(dae::GameObject* Ghost);
	virtual ~SetGhostToChase() = default;
	void Execute() override;

private:

	dae::GameObject* m_Ghost{};
	
};
class SetGhostToSpawn final : public dae::Command
{
public:
	SetGhostToSpawn(dae::GameObject* Ghost);
	virtual ~SetGhostToSpawn() = default;
	void Execute() override;

private:

	dae::GameObject* m_Ghost{};
	
};
class SkipLevel final : public dae::Command
{
public:
	SkipLevel() {};
	virtual ~SkipLevel() = default;
	void Execute() override;

private:

};
class MuteMusic final : public dae::Command
{
public:
	MuteMusic() {};
	virtual ~MuteMusic() = default;
	void Execute() override;

private:

};
class PlayerDied : public dae::Command
{
public:
	PlayerDied(dae::GameObject* healthObject);
	virtual ~PlayerDied() = default;
	void Execute() override;

private:

	dae::GameObject* m_GameObject{};

};
class GotoStartScreen : public dae::Command
{
public:
	GotoStartScreen() {};
	virtual ~GotoStartScreen() = default;
	void Execute() override;

private:

};
class SetSolo : public dae::Command
{
public:
	SetSolo() {};
	virtual ~SetSolo() = default;
	void Execute() override;

private:

};
class SetCoop : public dae::Command
{
public:
	SetCoop() {};
	virtual ~SetCoop() = default;
	void Execute() override;

private:

};
class SetVersus : public dae::Command
{
public:
	SetVersus() {};
	virtual ~SetVersus() = default;
	void Execute() override;

private:

};