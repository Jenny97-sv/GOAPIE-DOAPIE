#pragma once
#include "Food.h"

class Banana : public Food
{
public:
	bool Init() override;
	void Start() override;

	void Update(const float deltaTime) override;
	void Render() override;
private:
	bool InitAnimations();
	bool InitStats();
	bool InitStates();
};