#pragma once
#include "Engine/GameObject.h"

//左の柵の当たり判定を行うクラス
class LeftWire :
    public GameObject
{
private:
	XMVECTOR Normal_;//柵の法線
public:
	LeftWire(GameObject* parent);
	~LeftWire();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* pTarget) override;
	void InitCollision(XMFLOAT3 pos, XMFLOAT3 size);

	void SetNormal(XMVECTOR _normal) { Normal_ = _normal; }
	XMVECTOR GetNormal() { return Normal_; }
};

