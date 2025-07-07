#include "Transform.h"

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::Transform::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

glm::vec3 dae::Transform::Translate(const glm::vec3& position, const glm::vec3& direction)
{
	return position + direction;
}
