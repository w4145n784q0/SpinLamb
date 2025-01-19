#pragma once
#include "Engine/GameObject.h"
class GoalItem :
    public GameObject
{
private:
	int hCarrot_;
public:
	GoalItem(GameObject* parent);
	~GoalItem();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
};

