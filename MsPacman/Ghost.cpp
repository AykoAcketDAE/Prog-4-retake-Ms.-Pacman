#include "Ghost.h"
#include "GhostStates.h"
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "Player.h"
#include "Grid.h"
#include "Timer.h"
#include <cmath>
#include <iostream>


std::unique_ptr<GhostState> Ghost::m_ScatterState = std::make_unique<GhostScatter>();
std::unique_ptr<GhostState> Ghost::m_ChaseState = std::make_unique<GhostChase>();
std::unique_ptr<GhostState> Ghost::m_SpawnState = std::make_unique<GhostSpawn>();

Ghost::Ghost(dae::GameObject* owner, Grid* gridComp, const glm::vec2& startPos, Player* playerComp, const glm::vec2& cornerPos)
    :BaseComponent(owner), m_GridComp{ gridComp }, m_GhostInfo{ startPos,glm::vec2{0,0},cornerPos }, m_MsPacman{ &playerComp->m_PlayerInfo }
{
    m_RenderComp = GetOwner()->GetComponent<dae::RenderComponent>();
    
}

void Ghost::Update()
{
    if (m_CurrentState)
    {
        m_CurrentState->Update(*this);
    }
    else
    {
        SetState(m_SpawnState.get());
    }

    //if (m_GhostInfo.direction == glm::vec2{ 0,0 } or m_CurrentNode == nullptr)
    //    FindClosestNode(m_GhostInfo.pos,true);
    //else {
    //    
    //    for (auto& node : m_GridComp->m_Vertices)
    //    {
    //        if (node.first.pos == m_GhostInfo.pos)
    //        {
    //            //m_PrevNode = &node.first;
    //            
    //            
    //            if(!m_FoundPath)
    //            {
    //                if (m_PrevNode)
    //                    std::cout << "PrevNode: " << m_PrevNode->pos.x << " ," << m_PrevNode->pos.y << std::endl;
    //                SetTargetLocation(m_MsPacman->gridPos);
    //                FindPath(m_TargetPos);
    //                
    //            }
    //            break;
    //        }
    //    }
    //    
    //}
    //Movement();
    
}

void Ghost::Render() const
{

}

void Ghost::SetGhostLocation(const glm::vec2& pos)
{
    
    m_GhostInfo.pos = pos;

    GetOwner()->SetLocalPosition({ (pos.x * 24),
                                     (pos.y * 24) + 100 ,0 });

}

void Ghost::SetState(GhostState* state)
{
    if (m_CurrentState)
    {
        m_CurrentState->OnExit(*this);
    }
    if(state)
    {
        state->OnEnter(*this);
        m_CurrentState = state;
    }
}

void Ghost::Movement()
{
    FindPath(m_TargetPos);
    TileInfo* nextTile;
    TileInfo* currentTile;
    bool transition{ false };

    if (m_GhostInfo.pos.x + m_GhostInfo.direction.x <= -1)// When player teleports to other side of map
    {
        nextTile = &m_GridComp->m_Grid[27][static_cast<int>(m_GhostInfo.pos.y + m_GhostInfo.direction.y)]->m_TileInfo;
        currentTile = &m_GridComp->m_Grid[27][static_cast<int>(m_GhostInfo.pos.y)]->m_TileInfo;
        transition = true;
    }
    else if (m_GhostInfo.pos.x + m_GhostInfo.direction.x >= 28)// When player teleports to other side of map
    {
        nextTile = &m_GridComp->m_Grid[0][static_cast<int>(m_GhostInfo.pos.y + m_GhostInfo.direction.y)]->m_TileInfo;
        currentTile = &m_GridComp->m_Grid[0][static_cast<int>(m_GhostInfo.pos.y)]->m_TileInfo;
        transition = true;
    }
    else
    {
        nextTile = &m_GridComp->m_Grid[static_cast<int>(m_GhostInfo.pos.x + m_GhostInfo.direction.x)][static_cast<int>(m_GhostInfo.pos.y + m_GhostInfo.direction.y)]->m_TileInfo;
        currentTile = &m_GridComp->m_Grid[static_cast<int>(m_GhostInfo.pos.x)][static_cast<int>(m_GhostInfo.pos.y)]->m_TileInfo;
    }

    if (nextTile->isWalkable == true)
    {
        m_LerpTimer += dae::Time::GetInstance().GetDeltaTime();
        if (float t = m_LerpTimer / m_GhostInfo.time; t < 1)
        {
            if (transition)
            {
                
            }
            else
            {
                GetOwner()->SetLocalPosition({ std::lerp((m_GhostInfo.pos.x) * 24,(m_GhostInfo.pos.x + m_GhostInfo.direction.x) * 24,t),
                                    std::lerp(((m_GhostInfo.pos.y) * 24) + 100,((m_GhostInfo.pos.y + m_GhostInfo.direction.y) * 24) + 100,t),0 });
            }
        }
        else
        {
            // previous tile
            currentTile->m_Contents[static_cast<int>(TileTypes::Blinky)] = false;
            // current tile
            nextTile->m_Contents[static_cast<int>(TileTypes::Blinky)] = true;
            if (transition)
            {
                m_GhostInfo.pos = { nextTile->row ,nextTile->col };
                m_LerpTimer = 0;
                GetOwner()->SetLocalPosition({ (nextTile->row * 24),
                    (nextTile->col * 24) + 100 ,0 });
                //std::cout << "x: " << m_GhostInfo.pos.x << "  ,y: " << m_GhostInfo.pos.y << std::endl;
            }
            else
            {
                m_GhostInfo.pos = m_GhostInfo.pos + m_GhostInfo.direction;
                m_LerpTimer = 0;
                GetOwner()->SetLocalPosition({ (m_GhostInfo.pos.x) * 24,
                    ((m_GhostInfo.pos.y) * 24) + 100 ,0 });
                m_FoundPath = false;
                std::cout << "x: " << m_GhostInfo.pos.x << "  ,y: " << m_GhostInfo.pos.y << std::endl;
            }
        }
    }
    else
    {
        m_LerpTimer = 0;
        FindClosestNode(m_GhostInfo.pos, true);
        currentTile->m_Contents[static_cast<int>(TileTypes::Blinky)] = true;
    }
}

void Ghost::FindClosestNodeInDirection()
{
    Directions trueDirection;
    if (!m_CurrentNode) return;
    auto& currentNode = m_CurrentNode->neighbours;
    if (m_GhostInfo.direction == trueDirection.NORTH)
    {
        auto foundNode = std::find_if(currentNode.begin(), currentNode.end(), [&](std::pair<Node*, int> node)
            {
                auto direction = node.first->pos.y - m_CurrentNode->pos.y;
                if (direction <= trueDirection.NORTH.y) return true;
                else return false;
            });
        if (foundNode == currentNode.end())return;
    }
    if (m_GhostInfo.direction == trueDirection.SOUTH)
    {
        auto foundNode = std::find_if(currentNode.begin(), currentNode.end(), [&](std::pair<Node*, int> node)
            {
                auto direction = node.first->pos.y - m_CurrentNode->pos.y;
                if (direction >= trueDirection.SOUTH.y) return true;
                else return false;
            });
        if (foundNode == currentNode.end())return;
    }
    if (m_GhostInfo.direction == trueDirection.EAST)
    {
        auto foundNode = std::find_if(currentNode.begin(), currentNode.end(), [&](std::pair<Node*, int> node)
            {
                auto direction = node.first->pos.x - m_CurrentNode->pos.x;
                if (direction >= trueDirection.EAST.x) return true;
                else return false;
            });
        if (foundNode == currentNode.end())return;
    }
    if (m_GhostInfo.direction == trueDirection.WEST)
    {
        auto foundNode = std::find_if(currentNode.begin(), currentNode.end(), [&](std::pair<Node*, int> node)
            {
                auto direction = node.first->pos.x - m_CurrentNode->pos.x;
                if (direction <= trueDirection.WEST.x) return true;
                else return false;
            });
        if (foundNode == currentNode.end())return;
    }
    

}

void Ghost::FindClosestNode(const glm::vec2& startPos, bool excludingSelf)
{
    //auto ghostPos = m_GhostInfo.pos;
    
    auto& grid = m_GridComp->m_Vertices;
    auto minDistance = std::numeric_limits<float>::max();
    glm::vec2 directionNode{};
    for (auto& node : grid)
    {
        auto distance = glm::distance(startPos, node.first.pos);
        
        if (distance < minDistance)
        {
            minDistance = distance;
            m_ClosestNode = &node.first;
            directionNode =  node.first.pos - startPos;
            if (minDistance == 0) 
            {
                //m_PrevNode = m_CurrentNode;
                
                m_CurrentNode = m_ClosestNode;
            }
        }
    }
    if (excludingSelf)
    {
        Directions trueDirection;
        if (directionNode.x > 0) m_GhostInfo.direction = trueDirection.EAST;
        else if (directionNode.x < 0) m_GhostInfo.direction = trueDirection.WEST;
        else if (directionNode.y > 0) m_GhostInfo.direction = trueDirection.SOUTH;
        else if (directionNode.y < 0) m_GhostInfo.direction = trueDirection.NORTH;
    }
    
    
    
    
    
}

void Ghost::FindPath(glm::vec2 targetPos)
{
    
    FindClosestNode(m_GhostInfo.pos, false);

    if (m_ClosestNode == m_CurrentNode)
    {
        
        auto& neighbours = m_CurrentNode->neighbours;
        glm::vec2 directionNode{};
        auto minDistance = std::numeric_limits<float>::max();
        for (auto& node : neighbours)
        {
            if (node.first == m_PrevNode) 
                continue;
            auto distance = glm::distance(targetPos, node.first->pos);
            if (distance < minDistance)
            {
                minDistance = distance;
                m_ClosestNode = node.first;
                directionNode = node.first->pos - m_GhostInfo.pos;
                m_PrevNode = m_CurrentNode;
            }
        }
        //m_PrevNode = m_CurrentNode;
        Directions trueDirection;
        if(std::abs(directionNode.x) > std::abs(directionNode.y))
        {
            if (directionNode.x > 0) 
                m_GhostInfo.direction = trueDirection.EAST;
            else if (directionNode.x < 0) 
                m_GhostInfo.direction = trueDirection.WEST;
        }
        else
        {
            if (directionNode.y > 0) 
                m_GhostInfo.direction = trueDirection.SOUTH;
            else if (directionNode.y < 0)
                m_GhostInfo.direction = trueDirection.NORTH;
        }
        m_FoundPath = true;
        
    }
}

