#pragma once
#include "Engine/GameObject.h"
#include"Player.h"

class Player;

class Enemy :
    public GameObject
{
private:
    int hEnemy_;
	Player* pPlayer_;
	enum State {
		S_IDLE = 0,//待機
		S_SUSPICIOUS,//怪しむ
		S_CHASE,//追いかける
		S_ATTACK,//攻撃
		S_MOVE,//移動
		S_WINCE,//ひるむ
		S_MAX
	};
	State EnemyState_;

	XMVECTOR EnemyFrontDirection_;//敵の正面ベクトル
	XMVECTOR EnemyPosition_;//敵の位置ベクトル
	float RotateY_;//回転角度（基本Ｙ軸のみ）
	float FrontLength_;//敵の視界の長さ
	float Eye_;
	bool IsHit_;

	XMFLOAT3 pPosition_;
	XMVECTOR pPositionVec_;

	XMFLOAT3 ChasePoint_;

public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();
	void UpdateSuspicious();
	void UpdateChase();
	void UpdateAttack();
	void UpdateMove();
	void UpdateWince();

	void OnCollision(GameObject* pTarget) override;
};

