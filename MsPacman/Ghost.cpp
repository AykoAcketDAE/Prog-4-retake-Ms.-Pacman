#include "Ghost.h"
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "Player.h"
#include "Grid.h"
Ghost::Ghost(dae::GameObject* owner, Grid* gridComp, glm::vec2 startPos)
	:BaseComponent(owner),m_GridComp{gridComp},m_GhostInfo{startPos}
{
}

void Ghost::Update()
{
    FindShortestPath(FindClosestNode(m_GhostInfo.pos,true));
}

void Ghost::Render() const
{
}

void Ghost::SetTargetLocation(glm::vec2 pos)
{
	if (m_TargetNode == FindClosestNode(pos,false)) return;
    m_TargetNode = FindClosestNode(pos,false);
	
}

std::vector<Node*> Ghost::FindShortestPath(Node* startNode)
{
    std::queue<Node*> q;
    std::unordered_map<Node*,Node*> cameFrom;
    std::unordered_set<Node*> visited;

    q.push(startNode);
    visited.insert(startNode);

    while (!q.empty())
    {
        Node* current = q.front();
        q.pop();

        if (current == m_TargetNode)
            break;

        for (auto& [neighbor, _] : current->neighbours)
        {
            if (visited.count(neighbor) == 0)
            {
                visited.insert(neighbor);
                cameFrom[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    
    std::vector<Node*> path;
    for (Node* at = m_TargetNode; at != nullptr; at = cameFrom.count(at) ? cameFrom[at] : nullptr)
    {
        path.push_back(at);
    }

    std::reverse(path.begin(), path.end());

    if (path.front() != startNode)
        return {}; 

    return path;
}

Node* Ghost::FindClosestNode(const glm::vec2& position, bool isGhost)
{
    if (isGhost)
    {
        Node* closest = nullptr;
        float minDist = std::numeric_limits<float>::max();

        for (std::pair<Node, glm::vec2> node : m_GridComp->m_Vertices)
        {

            float dist = glm::distance(node.first.pos, position);
            if (dist < minDist)
            {
                minDist = dist;
                closest = &node.first;
            }
        }

        return closest;
    }
    else
    {
        Node* closest = nullptr;
        float minDist = std::numeric_limits<float>::max();

        for (std::pair<Node, glm::vec2> node : m_GridComp->m_Vertices)
        {
            float dist = glm::distance(node.first.pos, position);
            if (dist < minDist)
            {
                minDist = dist;
                closest = &node.first;
            }
        }

        return closest;
    }
   
}
	

