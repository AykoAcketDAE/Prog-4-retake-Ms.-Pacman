#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"

class Background : public dae::BaseComponent
{
public:
	Background(dae::GameObject* owner);
	~Background() = default;
	Background(const Background& other) = delete;
	Background(Background&& other) = delete;
	Background& operator=(const Background& other) = delete;
	Background& operator=(Background&& other) = delete;

	void Update()override;
	void Render() const override;

private:
	dae::RenderComponent* m_BackgroundImage{nullptr};
	bool m_IsMoving{false};
	float m_Height{};
	float m_FrameHeight{ 720.f };
};

