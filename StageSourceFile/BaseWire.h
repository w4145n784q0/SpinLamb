#pragma once
#include "../Engine/GameObject.h"

//柵の当たり判定・法線を扱う基底クラス
//前後左右の鉄線クラスはこのクラスを継承し、各自で当たり判定と法線を持つ
class BaseWire :
    public GameObject
{
protected:
	//柵の法線
	XMVECTOR Normal_;

public:
	BaseWire(GameObject* parent);
	BaseWire(GameObject* parent, const std::string& name);
	virtual ~BaseWire();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//解放
	void Release() override;

	/// <summary>
	/// 当たり判定初期化
	/// </summary>
	/// <param name="pos">ボックスコライダーの位置</param>
	/// <param name="size">ボックスコライダーのサイズ</param>
	void InitCollision(XMFLOAT3 _pos, XMFLOAT3 _size);

	//----------セッター・ゲッター関数----------

	//法線をセット
	void SetNormal(XMVECTOR _normal) { Normal_ = _normal; }

	//法線を取得
	XMVECTOR GetNormal() const { return Normal_; }
};

