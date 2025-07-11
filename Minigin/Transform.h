#pragma once
#include <glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);
		glm::vec3 Translate(const glm::vec3& position, const glm::vec3& direction);
	private:
		glm::vec3 m_position;
	};
}
