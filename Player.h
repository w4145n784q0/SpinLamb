#pragma once
#include "Engine/GameObject.h"
#include"Character.h"

//プレイヤーキャラクターの処理を行うクラス
class Player :
    public Character
{
private:
	//----------モデルハンドル----------

	//プレイヤーモデル
	int hPlayer_;

	//進行方向モデル
	int hArrow_;

	//----------プレイヤーステート----------
	//これらの値に応じて各Update関数を呼び出す
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
	//これらの値に応じてカメラの位置・回転量を変化させる
	enum CameraState
	{
		S_NORMALCAMERA,//通常カメラ
		S_DEBUGCAMERA,//デバッグ用カメラ
		S_MAXCAMERA,
	};
	CameraState CameraState_;
	
	//----------移動関係----------

	//キーボード使用時、プレイヤーの操作方向(xzどこに進むか)
	XMFLOAT3 Direction_;
	
	//----------カメラ関係----------

	//カメラ位置
	XMFLOAT3 CameraPosition_;

	//カメラ注視点
	XMFLOAT3 CameraTarget_;

	//カメラのTransform 回転だけ使う
	Transform cameraTransform_;

	//プレイヤーの後ろに置くカメラの位置
	XMVECTOR BackCamera_;

public:
	Player(GameObject* parent);
	~Player();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//当たり判定処理
	void OnCollision(GameObject* pTarget) override;

	//----------PlayerState_に応じて内容が変わるUpdate関数----------

	//通常状態
	void UpdateIdle();

	//チャージ状態
	void UpdateCharge();

	//攻撃状態
	void UpdateAttack();

	//弾かれた状態
	void UpdateHit();

	//壁に接触状態
	void UpdateWallHit();

	//プレイヤーを止める状態
	void UpdateStop();



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

