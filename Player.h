#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
class Player :
    public GameObject
{
private:
	int hModel_Player;
	int hcamera;
	bool IsOnGround_;
	float JumpSpeed_;//+ならジャンプしている状態 -なら下降～地面にいる状態

	bool IsDash_;//ダッシュ関係 ダッシュ中か
	float Acceleration_;//ダッシュ関係　加速度

	enum MoveDirection
	{
		Front = 0, Left,  Back, Right, None,
	};
	MoveDirection moveDir;
	int MoveDirArray[5] = { 0,270,180,90,0 };
	

	XMFLOAT3 Direction;//プレイヤーの方向(xzどこに進むか)
	XMVECTOR PlayerDirection;//方向ベクトル(実際に向いている方向)
	XMVECTOR PlayerPosition;//位置ベクトル
	//float RotateY;//回転角度（基本Ｙ軸のみ）

	Transform cameraTransform;//カメラのTransform 回転だけ使う
	XMVECTOR BackCamera;//プレイヤーの後ろに置くカメラの位置
	
	XMFLOAT3 CameraPosition;
	XMFLOAT3 CameraTarget;

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
	void Dash();
};

