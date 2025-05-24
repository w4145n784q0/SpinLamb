#pragma once
#include "Engine/GameObject.h"
class Fence :
    public GameObject
{
private:
	//----------モデルハンドル----------

	//柱モデル
	int hPiller_;

	//柵モデル
	int hFence_;

	//----------柵の位置----------

	Transform wire;
	Transform piller;

	XMFLOAT3 piller_UpperLeft_;
	XMFLOAT3 piller_UpperRight_;
	XMFLOAT3 piller_LowerLeft_;
	XMFLOAT3 piller_LowerRight_;
	

public:
	Fence(GameObject* parent);
	~Fence();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* pTarget) override;
	void SetPiller(float upper, float lower, float left, float right);

	void SetWireTransform(Transform _t) {
		wire = _t;
	}
	void SetPillerTransform(Transform _t) {
		piller = _t;
	}

	//void SetSCV();
};

