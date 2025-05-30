#pragma once
#include "Engine/GameObject.h"
#include"Player.h"

class Player;

class Enemy :
    public Character
{
private:
	//----------モデルハンドル----------

	//敵モデル
    int hEnemy_;

	//インスタンス関係
	Player* pPlayer_;

	//敵(CPU)ステート
	enum State {
		S_IDLE = 0,//待機
		S_ROOT,//判断用
		S_CHASE,//追いかける
		S_ATTACK,//攻撃
		S_HITSTOP,//ヒットストップ
		S_HIT,//攻撃を食らった
		S_WALLHIT,//壁に当たりダメージ
		S_AIM,//プレイヤーを狙う(攻撃準備)
		S_ONALEAT,//様子見

		S_MAX
	};
	State EnemyState_;

	//----------攻撃関係----------
	int AimTimer_;//狙ってから攻撃までのタイマー

	//----------移動関係----------
	XMVECTOR pPositionVec_;//プレイヤーの位置を保管
	XMFLOAT3 PlayerPosition_;//プレイヤーの位置(座標)
	float PlayerAcceleration_;//プレイヤーの速度

	//----------攻撃関係----------
	int RandomAim_;//EnemyAttackTimeArrayの添え字 

	//ダメージ関係
	int HitStopTimer_;//ヒットストップ時間


public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();//待機(デバッグ用)
	void UpdateRoot();//ここから次のUpdateに移る判断をする
	void UpdateChase();//追跡
	void UpdateAttack();//攻撃
	void UpdateHitStop();//ヒットストップ
	void UpdateHit();//攻撃を喰らった
	void UpdateWallHit();//壁に接触
	void UpdateAim();//プレイヤーを狙っている
	void UpdateOnAlert();//様子見

	void OnCollision(GameObject* pTarget) override;

	//敵に移動を許可
	void EnemyStart() { EnemyState_ = S_ROOT; }

	//敵を止める
	void EnemyStop() { EnemyState_ = S_MAX; }
	
	//void SetState(State s) { EnemyState_ = s; }
	//State GetState() { return EnemyState_; }

	//プレイヤーの方向に回転する
	void LookPlayer();

	//敵とプレイヤーの距離を返す
	float PlayerEnemyDistanceX();

	void SetCSVEnemy();

};

