#pragma once
#include "Engine/GameObject.h"

//柵の当たり判定・法線を扱う基底クラス
//前後左右の柵クラスはこのクラスを継承し、各自で当たり判定と法線を持つ
class Wire :
    public GameObject
{
protected:
	//柵の法線
	XMVECTOR Normal_;

public:
	Wire(GameObject* parent);
	Wire(GameObject* parent, const std::string& name);
	virtual ~Wire();

	//当たり判定処理
	void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// 当たり判定初期化
	/// </summary>
	/// <param name="pos">ボックスコライダーの位置</param>
	/// <param name="size">ボックスコライダーのサイズ</param>
	void InitCollision(XMFLOAT3 pos, XMFLOAT3 size);

	//----------セッター・ゲッター関数----------

	//法線をセット
	void SetNormal(XMVECTOR _normal) { Normal_ = _normal; }

	//法線を取得
	XMVECTOR GetNormal() { return Normal_; }
};

