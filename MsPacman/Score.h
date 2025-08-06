#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
class Score: public dae::BaseComponent
{
public:
	Score(dae::GameObject* owner);

	void Update() override;
	void Render() const override;
	void AddScore(int points) {
		m_Score += points;
	}
private:
	dae::TextComponent* m_TextComp;
	int m_Score{};
};