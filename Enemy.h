#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Ground.h"
class Player;

class Enemy :
    public GameObject
{
private:
	//モデルハンドル
    int hEnemy_;

	//インスタンス関係
	Player* pPlayer_;
	Ground* pGround_;

	//ステート
	enum State {
		S_IDLE = 0,//待機
		S_CHASE,//追いかける
		S_ATTACK,//攻撃
		S_MOVE,//移動
		S_HIT,//攻撃を食らった
		S_MAX
	};
	State EnemyState_;

	//追跡関係
	XMVECTOR EnemyFrontDirection_;//敵の正面ベクトル
	XMVECTOR EnemyPosition_;//敵の位置ベクトル
	float RotateY_;//回転角度（基本Ｙ軸のみ）
	float FrontLength_;//敵の視界の長さ
	float Eye_;
	bool IsHit_;
	bool IsOnGround_;//地面にいるか
	float JumpSpeed_;//+ならジャンプしている状態 -なら下降～地面にいる状態
	XMVECTOR pPositionVec_;//プレイヤーの位置を保管
	XMFLOAT3 ChasePoint_;

	//ヒット関係
	XMFLOAT3 ReflectMove;

	//移動関係
	bool isStop_;
	Point EnemyMovePoint_;//敵の移動先
	float moveLengthX_;//移動距離
	float moveLengthZ_;//移動距離
	float distance;

	//タイマー
	int MoveTimer_;

public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();
	void UpdateChase();
	void UpdateHit();
	void UpdateMove();

	void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_vector">プレイヤーから渡されたノックバックする方向(正規化済)</param>
	void PlayerReflect(XMVECTOR _vector);

	void SetState(State s) { EnemyState_ = s; }
	State GetState() { return EnemyState_; }
};

