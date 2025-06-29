#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//前方の柵の当たり判定を行うクラス
class UpperWire :
    public Wire
{
//private:
	//XMVECTOR Normal_;//柵の法線
	//XMFLOAT3 CollisionPos_;//当たり判定の位置
	//XMFLOAT3 CollisionSize_;//当たり判定のサイズ
public:
	UpperWire(GameObject* parent);
	~UpperWire();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//当たり判定処理
	//void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// 当たり判定初期化
	/// </summary>
	/// <param name="pos">ボックスコライダーの位置</param>
	/// <param name="size">ボックスコライダーのサイズ</param>
	//void InitCollision(XMFLOAT3 pos, XMFLOAT3 size);

	//----------セッター・ゲッター関数----------

	//void SetNormal(XMVECTOR _normal) { Normal_ = _normal; }
	//XMVECTOR GetNormal() { return Normal_; }
};

