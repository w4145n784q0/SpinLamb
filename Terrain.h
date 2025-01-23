#pragma once
#include "Engine/GameObject.h"
class Terrain :
    public GameObject
{
private:
	int hTerrain_;
public:
	Terrain(GameObject* parent);
	~Terrain();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

