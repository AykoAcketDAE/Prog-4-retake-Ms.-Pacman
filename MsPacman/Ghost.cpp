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







Ghost::Ghost(dae::GameObject* owner, Grid* gridComp, const glm::vec2& startPos, Player* playerComp, const glm::vec2& cornerPos, TileTypes type)
    :BaseComponent(owner), 
    m_GridComp{ gridComp }, 
    m_GhostInfo{ startPos,glm::vec2{0,0},cornerPos }, 
    m_MsPacman{ &playerComp->m_PlayerInfo },
    m_GhostType{type}
{
    m_RenderComp = GetOwner()->GetComponent<dae::RenderComponent>();
    m_ScatterState = std::make_unique<GhostScatter>();
    m_ChaseState = std::make_unique<GhostChase>();
    m_SpawnState = std::make_unique<GhostSpawn>();
    m_OriginalTexture = m_RenderComp->GetTexture();
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


    
}

void Ghost::Render() const
{

}

void Ghost::SetGhostLocation(const glm::vec2& pos)
{
    m_GhostInfo.pos = pos;
    GetOwner()->SetLocalPosition({ (pos.x * 24),(pos.y * 24) + 100 ,0 });
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
            GetOwner()->SetLocalPosition({ std::lerp((m_GhostInfo.pos.x) * 24,(m_GhostInfo.pos.x + m_GhostInfo.direction.x) * 24,t),
                                std::lerp(((m_GhostInfo.pos.y) * 24) + 100,((m_GhostInfo.pos.y + m_GhostInfo.direction.y) * 24) + 100,t),0 });
        }
        else
        {
            // previous tile
            currentTile->m_Contents[static_cast<int>(m_GhostType)] = false;
            // current tile
            nextTile->m_Contents[static_cast<int>(m_GhostType)] = true;
            if (transition)
            {
                m_GhostInfo.pos = { nextTile->row ,nextTile->col };
                m_LerpTimer = 0;
                GetOwner()->SetLocalPosition({ (nextTile->row * 24),
                    (nextTile->col * 24) + 100 ,0 });
                
            }
            else
            {
                m_GhostInfo.pos = m_GhostInfo.pos + m_GhostInfo.direction;
                m_LerpTimer = 0;
                GetOwner()->SetLocalPosition({ (m_GhostInfo.pos.x) * 24,
                    ((m_GhostInfo.pos.y) * 24) + 100 ,0 });
                m_FoundPath = false;
                
            }
        }
    }
    else
    {
        m_LerpTimer = 0;
        FindClosestNode(m_GhostInfo.pos, true);
        currentTile->m_Contents[static_cast<int>(m_GhostType)] = true;
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

