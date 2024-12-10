#pragma once
#include "Engine/GameObject.h"
class Enemy :
    public GameObject
{
private:
    int hModel_Enemy;

	enum EnemyState {
		S_IDLE = 0,//待機
		S_SUSPICIOUS,//怪しむ
		S_CHASE,//追いかける
		S_ATTACK,//攻撃
		S_MOVE,//移動
		S_WINCE,//ひるむ
		S_MAX
	};
	EnemyState enemy_state;

	XMVECTOR BasicVector;//敵の基本ベクトル
	XMVECTOR EnemyDirection;//敵の方向ベクトル
	XMVECTOR EnemyPosition;//敵の位置ベクトル
	float RotateY;//回転角度（基本Ｙ軸のみ）
	bool IsTargetted;//敵の視界に入ったか

	XMVECTOR sightLength;//敵の視界（扇型の視界）の長さ

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
	int GetModelHandle() { return hModel_Enemy; }
	bool ComparePosition(XMFLOAT3 pos);
};

