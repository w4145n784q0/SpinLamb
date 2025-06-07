#pragma once
#include "Engine/GameObject.h"

//前方の柵の当たり判定を行うクラス
class UpperWire :
    public GameObject
{
private:
	XMVECTOR Normal_;//柵の法線
	XMFLOAT3 CollisionPos_;//当たり判定の位置
	XMFLOAT3 CollisionSize_;//当たり判定のサイズ
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

