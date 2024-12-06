#pragma once
#include "Engine/GameObject.h"
class Ground :
    public GameObject
{
	int hModel_Ground;
public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

