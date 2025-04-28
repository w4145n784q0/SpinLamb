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
		S_HITSTOP,//ヒットストップ
		S_HIT,//攻撃を食らった
		S_WALLHIT,//壁に当たりダメージ
		S_AIM,//プレイヤーを狙う(攻撃準備)
		S_ONALEAT,//様子見

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
	XMFLOAT3 KnockBack_Direction_;//ノックバックする方向
	XMFLOAT3 KnockBack_Velocity_;//ノックバックする速度
	//ノックバックする速度= ノックバックする強さ(定数) * ノックバックする方向
 
	//移動関係
	Point EnemyMovePoint_;//敵の移動先
	float moveLengthX_;//移動距離
	float moveLengthZ_;//移動距離

	//攻撃関係
	int AimTimer_;//狙ってから攻撃までのタイマー
	XMVECTOR AttackVector_;//攻撃方向
	float Acceleration_;//加速度
	float AcceleValue_;//1fで加速度に加算する量

	//ダメージ関係
	int HitStopTimer_;//ヒットストップ時間
	int deadTimer_;//復活までの時間
	int InvincibilityTime_;//ダメージ後の無敵時間
	bool IsInvincibility_;//無敵時間か

public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();//待機(デバッグ用)
	void UpdateChase();//追跡
	void UpdateAttack();//攻撃
	void UpdateHitStop();//ヒットストップ
	void UpdateHit();//攻撃を喰らった
	void UpdateWallHit();//壁に接触
	void UpdateAim();//プレイヤーを狙っている
	void UpdateOnAlert();//様子見
	//void UpdateRandomAim();


	void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// プレイヤーからはじかれる処理
	/// </summary>
	/// <param name="_vector">プレイヤーから渡されたノックバックする方向(正規化済)</param>
	/// <param name="_isDush">プレイヤーがダッシュ中か</param>
	void PlayerReflect(XMVECTOR _vector, bool _isDush);
	
	void SetState(State s) { EnemyState_ = s; }
	State GetState() { return EnemyState_; }
	bool GetStateAttack() { if (EnemyState_ == S_ATTACK) return true; else return false; }

	void SetStateStop() { EnemyState_ = S_MAX; }

	void LookPlayer();
};

