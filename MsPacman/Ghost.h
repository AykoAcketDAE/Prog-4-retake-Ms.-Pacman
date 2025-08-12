#pragma once
#include "BaseComponent.h"
#include "RenderComponent.h"
#include "Player.h"
#include "Grid.h"
#include "GhostStates.h"


struct Node;
struct Directions
{
	const glm::vec2 NORTH{ 0,-1 };
	const glm::vec2 SOUTH{ 0,1 };
	const glm::vec2 EAST{ 1, 0};
	const glm::vec2 WEST{ -1,0 };
};

class GhostState;

class Ghost: public dae::BaseComponent
{
public: 
	std::unique_ptr<GhostState> m_ScatterState;
	std::unique_ptr<GhostState> m_ChaseState;
	std::unique_ptr<GhostState> m_SpawnState;

	Ghost(dae::GameObject* owner, Grid* gridComp,const glm::vec2& startPos,Player* playerComp,const glm::vec2& cornerPos,TileTypes type);

	void Update() override;
	void Render() const override;

	void Movement();
	void FindClosestNode(const glm::vec2& startPos, bool excludingSelf);

	void SetTargetLocation(const glm::vec2& pos) { m_TargetPos = pos; };
	void SetGhostLocation(const glm::vec2& pos);
	void SetState(GhostState* state);
	
	
	struct GhostInfo {
		glm::vec2 pos;
		glm::vec2 direction;
		glm::vec2 CornerPos;
		std::vector<Node*> path;
		float time{0.2f};
	}m_GhostInfo;
	
	Player::PlayerInfo* m_MsPacman;

	GhostState* m_CurrentState{};
private:

	
	void FindClosestNodeInDirection();
	
	void FindPath(glm::vec2 targetPos);

	float m_LerpTimer{};
	

	Node* m_TargetNode{};
	glm::vec2 m_TargetPos{};
	
	Node* m_CurrentNode{};
	Node* m_PrevNode{};
	Node* m_ClosestNode{};
	Grid* m_GridComp{};
	bool m_FoundPath{false};
	dae::RenderComponent* m_RenderComp{};
	
	
	TileTypes m_GhostType;
};

