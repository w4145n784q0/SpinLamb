#pragma once
#include "Engine/GameObject.h"
#include<array>
class Ground :
    public GameObject
{
	int hModel_Ground;
	int hModel_grass;
	int hModel_hole;

	struct StageData {
		int height;
		int type;
	};
	StageData stageTable[20][20];
	std::array<int, 2> blockArray;
public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	bool IsMoveFront(int x, int y);
};

