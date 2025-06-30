#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

//画面右下のミニマップを表示するクラス
class MiniMap :
    public GameObject
{
private:
	//----------インスタンス----------
	Player* pPlayer_;
	Enemy* pEnemy_;

	//----------マップ上の位置----------

	//Playerの位置
	XMFLOAT3 playerPos_;

	//Enemyの位置
	XMFLOAT3 enemyPos_;

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

	//セッター・ゲッター関数
	XMFLOAT3 GetPlayerPos() { return playerPos_; }
	XMFLOAT3 GetEnemyPos() { return enemyPos_; }
};

