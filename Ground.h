#pragma once
#include "Engine/GameObject.h"
#include<array>
#include<vector>

#include"Ground.h"

using std::vector;

class Ground :
    public GameObject
{
	int hModel_Ground;
	int hModel_Grass;
	int hModel_Wall;
	int hModel_Hole;

	/*struct StageData {
		int height;
		int type;
	};
	StageData stageTable[20][20];
	std::array<int, 2> blockArray;*/

	vector<vector<int>> MapData;
	int stageWidth_;
	int stageHeight_;
	Transform mapTrans;
	Transform ObjectTrans;

public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void ObjectSet();
	bool CanMoveFront(int x, int z);

	int GetGrassHandle() { return hModel_Ground; }
	int GetWallHandle() { return hModel_Wall; }

	Transform GetObjectTrans() { return ObjectTrans; }
};

