#pragma once
#include "Engine/GameObject.h"
class StageObject :
    public GameObject
{
private:
	int hModel_Wall_;

	XMVECTOR UpVec;

	//Player* pPlayer_;

public:
	StageObject(GameObject* parent);
	~StageObject();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	int GetWallHandle() { return hModel_Wall_; }
	XMVECTOR GetUpVector() { return UpVec; }
};

