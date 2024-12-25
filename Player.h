#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
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
		S_Normal,
		S_Hide,
		S_Jump,
		S_Hit,
	};
	State PlayerState;

	//ジャンプ関係
	bool IsOnGround_;
	float JumpSpeed_;//+ならジャンプしている状態 -なら下降～地面にいる状態
	XMFLOAT3 JumpDirection;//移動方向（ベクトル計算用）
	XMVECTOR MovePoint;
	XMVECTOR LandingPoint; //着地点
	Transform JumpTrans;//

	//ダッシュ関係
	bool IsDash_; //ダッシュ中か
	float Acceleration_;//加速度
	
	//移動関係
	XMFLOAT3 Direction;//プレイヤーの方向(xzどこに進むか)
	XMVECTOR PlayerDirection;//方向ベクトル(実際に向いている方向)
	XMVECTOR PlayerPosition;//位置ベクトル

	Transform cameraTransform;//カメラのTransform 回転だけ使う
	XMVECTOR BackCamera;//プレイヤーの後ろに置くカメラの位置
	
	Transform StartPosition;//開始位置
	XMVECTOR NewPos;//プレイヤーの移動先

	//カメラ関係
	XMFLOAT3 CameraPosition;
	XMFLOAT3 CameraTarget;



	//インスタンス関係
	Ground* pGround;

public:

	XMVECTOR GetPlayerDirection() { return PlayerDirection; }//方向ベクトル取得
	XMVECTOR GetPlayerPosition() { return PlayerPosition; }//位置ベクトル取得

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateNormal();
	void UpdateHide();
	void UpdateJump();
	void UpdateHit();

	void PlayerRayCast(int handle);

	void Dash();
};

