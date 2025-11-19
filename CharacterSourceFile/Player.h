#pragma once
#include "../Engine/GameObject.h"
#include"Character.h"

#include <memory>
#include <unordered_map>
#include"../CharacterSourceFile/PlayerState/BasePlayerState.h"

//プレイヤーキャラクターの処理を行うクラス
class Player :
    public Character
{
public:
	//----------状態遷移----------

	//プレイヤーの状態遷移
	//これらの値に応じて各Update関数を呼び出す
	enum PlayerState
	{
		S_Idle,		//通常
		S_Charge,	//チャージ中
		S_Attack,	//攻撃
		S_HitStop,  //ヒットストップ
		S_Hit,		//弾かれる
		S_FenceHit,	//柵にヒット
		S_Stop,		//プレイヤーを止める状態
		S_MaxState,
	};
	
private:
	//----------モデルハンドル----------

	//プレイヤーモデル
	int hPlayer_;

	//カメラの状態
	//これらの値に応じてカメラの位置・回転量を変化させる
	enum CameraState
	{
		S_NormalCamera,		//通常カメラ
		S_DebugCamera,		//デバッグ用カメラ
		S_MaxCamera,
	};
	CameraState CameraState_;
	
	//----------初期化----------

	//コントローラー使用時のID(キャラクタークラスのIDとは別)
	int ControllerID_;

	//----------移動関係----------

	//キーボード使用時、プレイヤーの操作方向(xzどこに進むか)
	XMFLOAT3 Direction_;

	//受け取った入力方向ベクトル(カメラ等を考慮しない)
	XMVECTOR PlayerInput_;
	
	//----------カメラ関係----------

	//カメラ位置
	XMFLOAT3 CameraPosition_;

	//カメラ注視点
	XMFLOAT3 CameraTarget_;

	//カメラのTransform 回転だけ使う
	Transform cameraTransform_;

	//プレイヤーの後ろに置くカメラの位置
	XMVECTOR BackCamera_;

	//----------ステートクラス関係----------

	//各ステートの実体を格納するテーブル(連想配列)
	std::unordered_map<PlayerState, std::unique_ptr<BasePlayerState>> StateTable_;

	//現在アクティブな状態
	BasePlayerState* CurrentState_;

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

	//Characterの仮想関数
	bool IsCharacterStateHitStop() override { return CurrentState_->IsHitStopState(); }
	bool IsCharacterStateHit() override { return CurrentState_->isHitState(); }
	bool IsCharacterStateFenceHit() override { return CurrentState_->IsFenceHitState(); }

	//動的に呼び出す更新処理
	void PlayerRun();

	//状態遷移を行う(ステートマシン)
	void ChangeState(PlayerState _newState);

	//----------Player処理関数----------

	//ImGuiの描画
	void DrawImGui();

	//プレイヤーに移動を許可(ステートマシン)
	void PlayerStartState() { ChangeState(Player::S_Idle); }
	
	//プレイヤーを止める(ステートマシン)
	void PlayerStopState() { ChangeState(Player::S_Stop); }

	/// <summary>
	/// プレイヤーの初期化(別関数から呼ぶ用)
	/// </summary>
	/// <param name="_CSVpath">読み込むcsvファイルのパス</param>
	/// <param name="_Modelpath">読み込むfbxファイルのパス</param>
	void PlayerInit(std::string _CSVpath, std::string _Modelpath);

	//カメラの操作（一部のステートのみ）
	void CameraControl();

	//カメラの設定(毎フレーム呼び出す)
	void CameraUpdate();

	//キーボードの移動処理を受け取る
	void InputKeyBoard();

	/// <summary>
	/// コントローラーの処理を受け取る
	/// </summary>
	/// <param name="_PadID">使用しているゲームパッドのID(接続順)</param>
	void InputCotroller(int _PadID);
	
	/// <summary>
	/// プレイヤー専用回転処理
	/// </summary>
	/// <param name="_move">移動方向 回転処理の基準となるベクトル</param>
	void PlayerRotate(XMVECTOR _move);
	
	/// <summary>
	/// カメラのY軸回転を使用し入力ベクトルをカメラ基準の方向に変換する
	/// </summary>
	/// <param name="input">プレイヤーの入力ベクトル</param>
	/// <returns>カメラのY軸回転を反映した移動ベクトル</returns>
	XMVECTOR ConvertCameraDirection(XMVECTOR _input);

	/// <summary>
	/// 受け取った名前に応じて要素を受け取り、反射処理をする
	/// </summary>
	/// <param name="_name">接触したオブジェクト名</param>
	void CollisionCharacter(std::string _name);

	//被弾・柵に衝突・無敵時間かどうかを取得
	bool IsDamage();

	/// <summary>
	/// Player限定のデータをCSV読み込み
	/// </summary>
	/// <param name="_path">読み込むcsvファイルのパス</param>
	void SetCSVPlayer(std::string _path);

	//----------セッター・ゲッター関数----------
	void SetCameraPosition(XMFLOAT3 _pos) { CameraPosition_ = _pos; }
	XMFLOAT3 GetCameraPosition() const { return CameraPosition_; }

	void SetCameraTarget(XMFLOAT3 _tar) { CameraTarget_ = _tar; }
	XMFLOAT3 GetCameraTarget() const { return CameraTarget_; }

	void SetControllerID(int _controllerID) { ControllerID_ = _controllerID; }
	int GetControllerID() const { return ControllerID_; }

	void SetPlayerInput(XMVECTOR _input) { PlayerInput_ = _input; }
	XMVECTOR GetPlayerInput() const { return PlayerInput_; }

	void SetChargeRotateY(float _rotate);
	float GetChargeRotateY() const;
};

