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

	float NorthEnd_;
	float SouthEnd_;
	float EastEnd_;
	float WestEnd_;

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
	//何かに当たった
	//引数：pTarget 当たった相手
	void OnCollision(GameObject* pTarget) override;

	void SetSCV();
};

