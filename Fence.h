#pragma once
#include "Engine/GameObject.h"
class Fence :
    public GameObject
{
private:
	int hFence_;
public:
	Fence(GameObject* parent);
	~Fence();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	//‰½‚©‚É“–‚½‚Á‚½
	//ˆø”FpTarget “–‚½‚Á‚½‘Šè
	void OnCollision(GameObject* pTarget) override;
};

