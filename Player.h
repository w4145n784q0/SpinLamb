#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"Engine/VFX.h"

namespace {
	const XMFLOAT3 StartPosition = { 0.0f,0.5f,0.0f };
}

class Player :
    public GameObject
{
private:
	//モデルハンドル
	int hPlayer_;

	int hPlayerIdle_;//待機モーション
	int hPlayerMove_;//移動
	int hPlayerDash_;//ダッシュ
	int hPlayerHit_;//攻撃を食らった
	int hPlayerDead_;//倒れる

	int hAttackArrow_;//チャージ中の矢印

	//サウンド関係
	int hCollisionSound_;

	//VFX関係
	int hChargeEmit_;//チャージエフェクト
	int hHitEmit_;//敵とのヒットエフェクト
	int hAttackEmitLocus_;//攻撃エフェクト(軌跡)
	int hAttackEmitAura_;//攻撃エフェクト(オーラ)

	//インスタンス関係
	Ground* pGround_;

	//プレイヤーステート
	enum State
	{
		S_IDLE,//通常
		S_HIT,//弾かれる
		S_CHARGE,//チャージ中
		S_ATTACK,//攻撃
		S_WALLHIT,//壁にヒット
		S_DEAD,//倒れる
		S_MAX
	};
	State PlayerState_;

	enum CameraState
	{
		S_NORMALCAMERA,//通常カメラ
		S_DEBUGCAMERA,//デバッグ用カメラ
		S_MAXCAMERA,
	};
	CameraState CameraState_;

	//ジャンプ関係
	bool IsOnGround_;
	float JumpSpeed_;//+ならジャンプしている状態 -なら下降～地面にいる状態


	//ダッシュ関係
	float Acceleration_;//加速度
	float AcceleValue_;//1fで加速度に加算する量
	
	//移動関係
	bool CanMove_;
	int PlayerHeight_;//プレイヤーの高さ
	XMFLOAT3 Direction_;//プレイヤーの方向(xzどこに進むか)
	XMVECTOR PlayerPosition_;//位置ベクトル
	XMVECTOR NewPos_;//プレイヤーの移動先
	XMFLOAT3 PlayerFront;//プレイヤーの正面位置 
	XMVECTOR ForwardVector_;//プレイヤーの正面ベクトル
	Transform ArrowTransform_;//矢印モデルの位置情報

	//カメラ関係
	XMFLOAT3 CameraPosition_;
	XMFLOAT3 CameraTarget_;
	Transform cameraTransform_;//カメラのTransform 回転だけ使う
	XMVECTOR BackCamera_;//プレイヤーの後ろに置くカメラの位置

	//被弾(HIT)関係
	float ColliderSize_;//スフィアコライダーのサイズ
	XMFLOAT3 KnockBack_Direction_;//ノックバックする方向
	XMFLOAT3 KnockBack_Velocity_;//ノックバックする速度

	//ダメージ関係
	int CharacterLife_;//キャラクターのHP
	int deadTimer_;//復活までの時間
	int InvincibilityTime_;//ダメージ後の無敵時間
	bool IsInvincibility_;//無敵時間か

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
	void UpdateHit();
	void UpdateCharge();
	void UpdateAttack();
	void UpdateOut();
	void UpdateWallHit();
	void UpdateDead();
	void PlayerStop() { PlayerState_ = S_MAX; }
	
	/// <summary>
	/// カメラの操作（一部のステートのみ）
	/// </summary>
	void CameraControl();

	/// <summary>
	/// 敵からはじかれる処理
	/// </summary>
	/// <param name="_vector">敵からノックバックする方向(正規化済)</param>
	/// <param name="_IsAttack">敵が攻撃中か</param>
	void EnemyReflect(XMVECTOR _vector, bool _IsAttack);

	//初期位置に戻す
	void SetStartPosition() { this->transform_.position_ = StartPosition; }

	bool IsAttackState() { if (PlayerState_ == S_ATTACK) return true; else return false; }

	//チャージ状態エフェクトをつける
	void SetChargeEffect();

	//突撃エフェクトつける
	void SetAttackAuraEffect();
	void SetAttackLocusEffect();

	//衝撃エフェクトつける
	void SetHitEffect();

	//ヒットエフェクトを数フレームだけパーティクル表示
	void HitEffectStop();
	
	//軌跡エフェクトを数フレームだけパーティクル表示
	void LocusEffectStop();

	void AuraEffectStop();

	void SetCharacterLife(int _hp) { CharacterLife_ = _hp; }
	int GetCharacterLife() { return CharacterLife_; }

	//void AttackEffectStop(int& EffectEndCount, bool& _IsEffect, int& VFXhandle);
};

