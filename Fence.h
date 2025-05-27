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

	//鉄線のトランスフォーム
	Transform wire;
	
	//左上
	XMFLOAT3 piller_UpperLeft_;

	//右上
	XMFLOAT3 piller_UpperRight_;

	//左下
	XMFLOAT3 piller_LowerLeft_;

	//右下
	XMFLOAT3 piller_LowerRight_;
	

public:
	Fence(GameObject* parent);
	~Fence();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// 4つの柵の位置決定
	/// </summary>
	/// <param name="upper">北端</param>
	/// <param name="lower">南端</param>
	/// <param name="left">西端</param>
	/// <param name="right">東端</param>
	/// <param name="height">柱の高さ</param>
	void SetPiller(float upper, float lower, float left, float right, float height);

	void InitWireTransform(Transform _t) { wire = _t; }

	void InitPillerTransform(Transform _t);
	
};
