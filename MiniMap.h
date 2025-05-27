#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

class MiniMap :
    public GameObject
{
private:
	//----------画像ハンドル----------

	//マップ全体
	int hMap_;

	//プレイヤーのアイコン
	int hPlayerIcon_;

	//敵のアイコン
	int hEnemyIcon_;

	Transform mTrans;
	Transform pTrans;
	Transform eTrans;

	//----------インスタンス----------
	Player* pPlayer_;
	Enemy* pEnemy_;

public:
	MiniMap(GameObject* parent);
	~MiniMap();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

