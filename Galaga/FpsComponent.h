#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"

class FpsComponent : public dae::BaseComponent
{
public:
	FpsComponent(dae::GameObject* owner) :dae::BaseComponent(owner) {};
	~FpsComponent() = default;
	FpsComponent(const FpsComponent& other) = delete;
	FpsComponent(FpsComponent&& other) = delete;
	FpsComponent& operator=(const FpsComponent& other) = delete;
	FpsComponent& operator=(FpsComponent&& other) = delete;

	void Update() override;
	void Render() const override {};
private:

	dae::TextComponent* m_TextPtr{ nullptr };
	double m_Fps{};
};



