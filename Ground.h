#pragma once
#include "Engine/GameObject.h"
#include<array>
#include<vector>

using std::vector;

class Ground :
    public GameObject
{
	int hModel_Ground;
	int hModel_Wall;

	struct StageData {
		int height;
		int type;
	};
	StageData stageTable[20][20];
	std::array<int, 2> blockArray;

	int stageWidth_;
	int stageHeight_;
public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	bool IsMoveFront(int x, int y);
	bool CanMoveFront(int x, int z);
	int GetGrassHandle() { return hModel_Ground; }
	int GetWallHandle() { return hModel_Wall; }
};

