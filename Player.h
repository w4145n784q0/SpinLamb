#pragma once
#include "Engine/GameObject.h"
#include"Character.h"
#include"Ground.h"
#include"Engine/VFX.h"
#include"Engine/CsvReader.h"

class Player :
    public Character
{
private:
	//----------モデルハンドル----------

	//プレイヤーモデル
	int hPlayer_;

	//チャージ中の矢印
	//int hAttackArrow_;

	//----------サウンドハンドル----------

	//接触音
	int hCollisionSound_;

	//----------インスタンス関係----------
	//Ground* pGround_;

	//----------プレイヤーステート----------
	enum State
	{
		S_IDLE,//通常
		S_HIT,//弾かれる
		S_CHARGE,//チャージ中
		S_ATTACK,//攻撃
		S_WALLHIT,//壁にヒット
		S_MAX
	};
	State PlayerState_;

	//----------カメラステート----------
	enum CameraState
	{
		S_NORMALCAMERA,//通常カメラ
		S_DEBUGCAMERA,//デバッグ用カメラ
		S_ROCKONCAMERA,//敵を注視するカメラ
		S_MAXCAMERA,
	};
	CameraState CameraState_;
	
	//----------移動関係----------
	XMVECTOR PlayerPosition_;//位置ベクトル(毎フレーム保存)
	XMVECTOR NewPos_;//プレイヤーの移動先 
	XMFLOAT3 Direction_;//プレイヤーの操作方向(xzどこに進むか)
	Transform ArrowTransform_;//矢印モデルの位置情報
	
	//----------カメラ関係----------
	XMFLOAT3 CameraPosition_;//カメラ位置
	XMFLOAT3 CameraTarget_;//カメラ注視点
	Transform cameraTransform_;//カメラのTransform 回転だけ使う
	XMVECTOR BackCamera_;//プレイヤーの後ろに置くカメラの位置

public:

	//XMVECTOR GetPlayerDirection() { return ForwardVector_; }//方向ベクトル取得
	//XMVECTOR GetPlayerPosition() { return PlayerPosition_; }//位置ベクトル取得

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateIdle();
	void UpdateHit();
	void UpdateCharge();
	void UpdateAttack();
	void UpdateWallHit();

	//プレイヤーに移動を許可
	void PlayerStart() { PlayerState_ = S_IDLE; }

	//プレイヤーを止める
	void PlayerStop() { PlayerState_ = S_MAX; }
	
	//プレイヤーの能動的移動
	//void PlayerMove();

	//プレイヤーがはじかれる動作
	void Blown();

	//プレイヤーの重力
	//void Gravity();

	//カメラの操作（一部のステートのみ）
	void CameraControl();

	//カメラの設定(毎フレーム呼び出す)
	void CameraUpdate();



	void NormalCamera();
	void DebugCamera();
	void RockOnCamra();

	/// <summary>
	/// 敵からはじかれる処理
	/// </summary>
	/// <param name="_vector">敵からノックバックする方向(正規化済)</param>
	/// <param name="_IsAttack">敵が攻撃中か</param>
	void EnemyReflect(XMVECTOR _vector, bool _IsAttack);

	//攻撃中かどうか取得(接触判定に使用)
	bool IsAttackState() { if (PlayerState_ == S_ATTACK) return true; else return false; }

	bool GetIsInvincibility() { if (IsInvincibility_) return true; else return false; }

	//チャージ状態エフェクトをつける
	void SetChargingEffect();

	//突撃エフェクトつける
	void SetAttackLocusEffect();

	//衝撃エフェクトつける
	void SetHitEffect();

};

