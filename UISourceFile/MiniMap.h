#pragma once
#include "../Engine/GameObject.h"

//画面右下のミニマップを表示するクラス
class MiniMap :
	public GameObject
{
private:
	//----------マップ上の位置----------

	//Player1の位置
	XMFLOAT3 FirstPos_;

	//Player2,Enemyの位置
	XMFLOAT3 SecondPos_;

	//Player1の本来の位置 各プレイシーンから渡される
	XMFLOAT3 OriginalFirstPos_;

	//Player2,Enemyの本来の位置 各プレイシーンから渡される
	XMFLOAT3 OriginalSecondPos_;

public:
	MiniMap(GameObject* parent);
	~MiniMap();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//CSVファイルから必要パラメータを読み込みする
	void SetMiniMapCSV();

	//----------セッター・ゲッター関数----------

	void SetFirstPos(XMFLOAT3 _pos) { FirstPos_ = _pos; }
	void SetSecondPos(XMFLOAT3 _pos) { SecondPos_ = _pos; }
	XMFLOAT3 GetFirstPos() const { return FirstPos_; }
	XMFLOAT3 GetSecondPos() const { return SecondPos_; }

	void SetOriginalFirstPos(XMFLOAT3 _pos) { OriginalFirstPos_ = _pos; }
	void SetOriginalSecondPos(XMFLOAT3 _pos) { OriginalSecondPos_ = _pos; }
	XMFLOAT3 GetOriginalFirstPos() const { return OriginalFirstPos_; }
	XMFLOAT3 GetOriginalSecondPos() const { return OriginalSecondPos_; }

};
