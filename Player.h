#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"

namespace {
	const XMFLOAT3 StartPosition = { 0.0f,0.5f,0.0f };
}

class Player :
    public GameObject
{
private:
	//モデルハンドル
	int hPlayer_;
	int hLandingPoint_;
	int hNextPoint_;

	//音ハンドル
	int hCollisionSound_;

	//インスタンス関係
	Ground* pGround_;

	//ステート
	enum State
	{
		S_IDLE,
		S_JUMP,
		S_HIT,
		S_CHARGE,
		S_ATTACK,
		S_OUT,
		S_DEAD,
		S_MAX
	};
	State PlayerState_;

	//ジャンプ関係
	bool IsOnGround_;
	float JumpSpeed_;//+ならジャンプしている状態 -なら下降～地面にいる状態


	//ダッシュ関係
	bool IsDash_; //ダッシュ中か
	bool IsDashStart_ = false;//ダッシュ開始したか
	float Acceleration_;//加速度
	
	//移動関係
	bool CanMove_;//移動できるか
	int PlayerHeight_;//プレイヤーの高さ
	XMFLOAT3 Direction_;//プレイヤーの方向(xzどこに進むか)
	XMVECTOR PlayerPosition_;//位置ベクトル
	XMVECTOR NewPos_;//プレイヤーの移動先
	XMFLOAT3 PlayerFront;//プレイヤーの正面位置 当たり判定用
	XMVECTOR ForwardVector_;//プレイヤーの正面ベクトル

	//カメラ関係
	XMFLOAT3 CameraPosition_;
	XMFLOAT3 CameraTarget_;
	Transform cameraTransform_;//カメラのTransform 回転だけ使う
	XMVECTOR BackCamera_;//プレイヤーの後ろに置くカメラの位置

	//タイマー
	int deadTimer_;//復活までの時間

	//落下関係

public:

	XMVECTOR GetPlayerDirection() { return ForwardVector_; }//方向ベクトル取得
	XMVECTOR GetPlayerPosition() { return PlayerPosition_; }//位置ベクトル取得

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateIdle();
	//void UpdateJump();
	void UpdateHit();
	void UpdateCharge();
	void UpdateAttack();
	void UpdateOut();
	void UpdateDead();
	void Dash();
	void PlayerStop() { PlayerState_ = S_MAX; }
	
	/// <summary>
	/// カメラの操作（一部のステートのみ）
	/// </summary>
	void CameraControl();

	void EnemyReflect(XMVECTOR _vector);

	void SetStartPosition() { this->transform_.position_ = StartPosition; }
};

