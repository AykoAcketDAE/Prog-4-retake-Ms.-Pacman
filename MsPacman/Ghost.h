#pragma once
#include "BaseComponent.h"
#include "RenderComponent.h"
#include "Player.h"
#include "Grid.h"
#include <deque>
struct Node;
struct Directions
{
	const glm::vec2 NORTH{ 0,-1 };
	const glm::vec2 SOUTH{ 0,1 };
	const glm::vec2 EAST{ 1, 0};
	const glm::vec2 WEST{ -1,0 };
};
class Ghost: public dae::BaseComponent
{
public: 
	Ghost(dae::GameObject* owner, Grid* gridComp,const glm::vec2& startPos,Player* playerComp);

	void Update() override;
	void Render() const override;

	void SetTargetLocation(const glm::vec2& pos);

	struct GhostInfo {
		glm::vec2 pos;
		glm::vec2 direction;
		std::vector<Node*> path;
		float time{0.2f};
	}m_GhostInfo;


private:
	void FindClosestNodeInDirection();
	void FindClosestNode(const glm::vec2& startPos,bool excludingSelf);
	void FindPath();



	float m_LerpTimer{};
	Player::PlayerInfo* m_MsPacman;

	Node* m_TargetNode{};
	Node* m_CurrentNode{};
	Node* m_PrevNode{};
	Node* m_ClosestNode{};
	Grid* m_GridComp{};
	bool m_FoundPath{false};
	dae::RenderComponent* m_RenderComp{};
	
};

