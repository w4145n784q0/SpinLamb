#pragma once
#include "Engine/GameObject.h"
class RightWire :
    public GameObject
{
private:
	XMVECTOR Normal_;//çÚÇÃñ@ê¸
public:
	RightWire(GameObject* parent);
	~RightWire();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* pTarget) override;
	void InitCollision(XMFLOAT3 pos, XMFLOAT3 size);

	void SetNormal(XMVECTOR _normal) { Normal_ = _normal; }
	XMVECTOR GetNormal() { return Normal_; }

};

