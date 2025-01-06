#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"StageObject.h"

class Player :
    public GameObject
{
private:
	//モデルハンドル
	int hModel_Player;
	int hModel_LandingPoint;
	int hGetGrass;
	int hGetWall;

	//ステート
	enum State
	{
		S_IDLE,
		S_HIDE,
		S_JUMPBEFORE,
		S_JUMP,
		S_HIT,
	};
	State PlayerState;

	//ジャンプ関係
	bool IsOnGround_;
	float JumpSpeed_;//+ならジャンプしている状態 -なら下降～地面にいる状態
	//XMFLOAT3 JumpDirection;//移動方向（ベクトル計算用）

	Transform JumpTransform_;//着地位置（Transform）
	XMFLOAT3 LandingPoint;//着地位置（XMFLOAT3）

	//ダッシュ関係
	bool IsDash_; //ダッシュ中か
	float Acceleration_;//加速度
	
	//移動関係
	XMFLOAT3 Direction;//プレイヤーの方向(xzどこに進むか)
	XMVECTOR PlayerFrontDirection;//正面ベクトル
	XMVECTOR PlayerPosition;//位置ベクトル

	Transform StartPosition;//開始位置
	XMVECTOR NewPos;//プレイヤーの移動先

	//カメラ関係
	XMFLOAT3 CameraPosition;
	XMFLOAT3 CameraTarget;
	Transform cameraTransform;//カメラのTransform 回転だけ使う
	XMVECTOR BackCamera;//プレイヤーの後ろに置くカメラの位置

	//インスタンス関係
	Ground* pGround;
	StageObject* pStageObject;

	//他オブジェクト関係
	bool IsHitWall;

public:

	XMVECTOR GetPlayerDirection() { return PlayerFrontDirection; }//方向ベクトル取得
	XMVECTOR GetPlayerPosition() { return PlayerPosition; }//位置ベクトル取得

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateIdle();
	void UpdateHide();
	void UpdateJumpBefore();
	void UpdateJump();
	void UpdateHit();

	/// <summary>
	/// 地面に向かってレイを飛ばす（必要なときだけ）
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	void PlayerRayCast(int handle);

	void Dash();

	/// <summary>
	/// カメラの操作（一部のステートのみ）
	/// </summary>
	void CameraControl();

	void RideObject();
};

