#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"Terrain.h"
#include"Tree.h"
#include"TreeManager.h"

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

	//インスタンス関係
	Ground* pGround_;
	Terrain* pTerrain_;

	//ステート
	enum State
	{
		S_IDLE,
		S_HIDE,
		S_JUMPBEFORE,
		S_JUMP,
		S_HIT,
	};
	State PlayerState_;

	//ジャンプ関係
	bool IsOnGround_;
	float JumpSpeed_;//+ならジャンプしている状態 -なら下降～地面にいる状態
	float PrevHeight_;//元の高さを保管する
	//XMFLOAT3 JumpDirection;//移動方向（ベクトル計算用）
	XMVECTOR JumpTarget_;
	XMVECTOR JumpLength_;

	XMFLOAT3 JumpValue;

	XMVECTOR PlayerStart_;//計算用　プレイヤーの始点
	Transform JumpTransform_;//着地位置（Transform）
	XMFLOAT3 LandingPoint;//着地位置（XMFLOAT3）

	//ダッシュ関係
	bool IsDash_; //ダッシュ中か
	float Acceleration_;//加速度
	
	//移動関係
	bool CanMove_;//移動できるか
	int PlayerHeight_;//プレイヤーの高さ
	XMFLOAT3 Direction_;//プレイヤーの方向(xzどこに進むか)
	XMVECTOR PlayerFrontDirection_;//正面ベクトル
	XMVECTOR PlayerPosition_;//位置ベクトル
	XMVECTOR NewPos_;//プレイヤーの移動先
	XMFLOAT3 PlayerFront;//プレイヤーの正面位置 当たり判定用

	int nextX, nextY ,nextZ;

	//カメラ関係
	XMFLOAT3 CameraPosition_;
	XMFLOAT3 CameraTarget_;
	Transform cameraTransform_;//カメラのTransform 回転だけ使う
	XMVECTOR BackCamera_;//プレイヤーの後ろに置くカメラの位置


	//他オブジェクト関係
	bool IsHitWall;

public:

	XMVECTOR GetPlayerDirection() { return PlayerFrontDirection_; }//方向ベクトル取得
	XMVECTOR GetPlayerPosition() { return PlayerPosition_; }//位置ベクトル取得

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateIdle();
	void UpdateJumpBefore();
	void UpdateJump();
	void UpdateHit();

	void Dash();

	/// <summary>
	/// カメラの操作（一部のステートのみ）
	/// </summary>
	void CameraControl();

	void SetStartPosition() { this->transform_.position_ = StartPosition; }
};

