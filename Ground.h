#pragma once
#include "Engine/GameObject.h"
#include"Engine/CsvReader.h"
#include<array>
#include<vector>

using std::vector;

class Ground :
    public GameObject
{
	int hGrass_;
public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	Point GetRandomMovePoint();
};