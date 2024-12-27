#pragma once
#include "Engine/GameObject.h"
class StageObject :
    public GameObject
{
private:
	int hModel_Wall_;

	Transform trans_wall;

public:
	StageObject(GameObject* parent);
	~StageObject();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	int GetWallHandle() { return hModel_Wall_; }
};

