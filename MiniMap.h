#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

//画面右下のミニマップを表示するクラス
class MiniMap :
    public GameObject
{
private:
	//----------画像ハンドル----------

	////マップ全体
	//int hMap_;

	////プレイヤーのアイコン
	//int hPlayerIcon_;

	////敵のアイコン
	//int hEnemyIcon_;

	//----------インスタンス----------
	Player* pPlayer_;
	Enemy* pEnemy_;

	//----------マップ上の位置----------
	XMFLOAT3 playerPos_;
	XMFLOAT3 enemyPos_;

public:
	MiniMap(GameObject* parent);
	~MiniMap();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetCSV();

	XMFLOAT3 GetPlayerPos() { return playerPos_; }
	XMFLOAT3 GetEnemyPos() { return enemyPos_; }
};

