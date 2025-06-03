#pragma once
#include "Engine/GameObject.h"
class UpperWire :
    public GameObject
{
private:
	XMVECTOR Normal_;//çÚÇÃñ@ê¸
public:
	UpperWire(GameObject* parent);
	~UpperWire();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* pTarget) override;
	void InitCollision(XMFLOAT3 pos, XMFLOAT3 size);

	void SetNormal(XMVECTOR _normal) { Normal_ = _normal; }
	XMVECTOR GetNormal() { return Normal_; }
};

