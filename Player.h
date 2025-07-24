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
	
	//----------初期化----------

	//コントローラー使用時のID(キャラクタークラスのIDとは別)
	int ControllerID_;

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

	//動的に呼び出す更新処理
	void PlayerRun();

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
	
	/// <summary>
	/// プレイヤーの初期化(別関数から呼ぶ用)
	/// </summary>
	/// <param name="_CSVpath">読み込むcsvファイルのパス</param>
	/// <param name="_Modelpath">読み込むfbxファイルのパス</param>
	void PlayerInit(std::string _CSVpath, std::string _Modelpath);

	//ジャンプ開始
	void SetJump();

	//カメラの操作（一部のステートのみ）
	void CameraControl();

	//カメラの設定(毎フレーム呼び出す)
	void CameraUpdate();

	//キーボード移動操作
	void KeyBoradMove();

	/// <summary>
	/// コントローラ移動操作
	/// </summary>
	/// <param name="_PadID">使用しているゲームパッドのID(接続順)</param>
	void ControllerMove(int _PadID);

	/// <summary>
	/// プレイヤー専用移動処理
	/// </summary>
	/// <param name="_move">移動する方向</param>
	void PlayerMove(XMVECTOR _move);

	/// <summary>
	/// 受け取った名前に応じて要素を受け取り、反射処理をする
	/// </summary>
	/// <param name="_name">接触したオブジェクト名</param>
	void CollisionCharacter(std::string _name);

	void DrawImGui();

	/// <summary>
	/// Player限定のデータをCSV読み込み
	/// </summary>
	/// <param name="_path">読み込むcsvファイルのパス</param>
	void SetCSVPlayer(std::string _path);

	//----------セッター・ゲッター関数----------
	XMFLOAT3 GetCameraPosition() const { return CameraPosition_; }
	XMFLOAT3 GetCameraTarget() const { return CameraTarget_; }

	void SetControllerID(int _controllerID) { ControllerID_ = _controllerID; }
	int GetControllerID() const { return ControllerID_; }
};

