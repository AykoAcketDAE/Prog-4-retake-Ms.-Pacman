#pragma once
#include "BaseComponent.h"
class Player;
class Grid;
struct Node;
class Ghost: public dae::BaseComponent
{
public: 
	Ghost(dae::GameObject* owner, Grid* gridComp,glm::vec2 startPos);

	void Update() override;
	void Render() const override;

	void SetTargetLocation(glm::vec2 pos);
	struct GhostInfo {
		glm::vec2 pos;
		glm::vec2 direction;
		std::vector<Node*> path;
	}m_GhostInfo;
private:

	std::vector<Node*> FindShortestPath(Node* startNode);
	Node* FindClosestNode(const glm::vec2& position, bool isGhost);
	Node* m_TargetNode{};
	Grid* m_GridComp{};
};

