#pragma once
#include "Engine/GameObject.h"
#include"Character.h"

class Player :
    public Character
{
private:
	//----------モデルハンドル----------

	//プレイヤーモデル
	int hPlayer_;

	//影のモデル
	int hShadow_;

	//----------プレイヤーステート----------
	enum State
	{
		S_IDLE,//通常
		S_CHARGE,//チャージ中
		S_ATTACK,//攻撃
		S_HIT,//弾かれる
		S_WALLHIT,//壁にヒット
		S_STOP,//プレイヤーを止める状態
		S_MAX
	};
	State PlayerState_;

	//----------カメラステート----------
	enum CameraState
	{
		S_NORMALCAMERA,//通常カメラ
		S_DEBUGCAMERA,//デバッグ用カメラ
		S_MAXCAMERA,
	};
	CameraState CameraState_;
	
	//----------移動関係----------
	XMFLOAT3 Direction_;//プレイヤーの操作方向(xzどこに進むか)
	
	//----------カメラ関係----------
	XMFLOAT3 CameraPosition_;//カメラ位置
	XMFLOAT3 CameraTarget_;//カメラ注視点
	Transform cameraTransform_;//カメラのTransform 回転だけ使う
	XMVECTOR BackCamera_;//プレイヤーの後ろに置くカメラの位置


public:

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateIdle();//通常状態
	void UpdateCharge();//チャージ状態
	void UpdateAttack();//攻撃状態
	void UpdateHit();//ヒット状態
	void UpdateWallHit();//壁に接触状態
	void UpdateStop();//プレイヤーを止める状態

	bool IsCanWallReflect(){
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
			return true;
		else
			return false;
	}

	void PlayerReflect(XMVECTOR reflect) {
		WallReflect(reflect);
		PlayerState_ = S_WALLHIT;
	}

	//プレイヤーに移動を許可
	void PlayerStart() { PlayerState_ = S_IDLE; }

	//プレイヤーを止める
	void PlayerStop() { PlayerState_ = S_STOP; }
	
	//ジャンプ開始
	void SetJump();

	//カメラの操作（一部のステートのみ）
	void CameraControl();

	//カメラの設定(毎フレーム呼び出す)
	void CameraUpdate();

	/// キーボード移動操作
	void KeyBoradMove();

	/// Player限定のデータをCSV読み込み
	void SetCSVPlayer();
};

