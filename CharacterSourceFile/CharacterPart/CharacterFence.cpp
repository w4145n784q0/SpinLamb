#include "CharacterFence.h"
#include"../Character.h"
#include"../../StageSourceFile/UpperWire.h"
#include"../../StageSourceFile/LowerWire.h"
#include"../../StageSourceFile/LeftWire.h"
#include"../../StageSourceFile/RightWire.h"

CharacterFence::CharacterFence(GameObject* parent)
	:GameObject(parent, "CharacterFence"), params_(nullptr), character_(nullptr),
	VFXListener_(nullptr), ChargeListener_(nullptr), MovementListener_(nullptr)
{
}

void CharacterFence::SetEventListener(IVFXEventListener* _VfxListener,
	IChargeEventListener* _ChargeListener, IMovementEventListener* _MovementListener)
{
	VFXListener_ = _VfxListener;
	ChargeListener_ = _ChargeListener;
	MovementListener_ = _MovementListener;
}

void CharacterFence::GetWireNormal()
{
	//各インスタンスから柵の法線を取得
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
	assert(pUpperWire != nullptr);
	params_->FenceHitParam_.UpperNormal_ = pUpperWire->GetNormal();

	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
	assert(pLowerWire != nullptr);
	params_->FenceHitParam_.LowerNormal_ = pLowerWire->GetNormal();

	RightWire* pRightWire = (RightWire*)FindObject("RightWire");
	assert(pRightWire != nullptr);
	params_->FenceHitParam_.RightNormal_ = pRightWire->GetNormal();

	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
	assert(pLeftWire != nullptr);
	params_->FenceHitParam_.LeftNormal_ = pLeftWire->GetNormal();

	//取得した法線を配列に格納
	//これらが柵に接触した際の跳ね返る方向になる
	params_->FenceHitParam_.NormalArray_ = {
		params_->FenceHitParam_.UpperNormal_, 
		params_->FenceHitParam_.LowerNormal_,
		params_->FenceHitParam_.RightNormal_, 
		params_->FenceHitParam_.LeftNormal_, 
	};
}


void CharacterFence::FenceReflect(XMVECTOR _normal)
{
	//接触エフェクトを通知
	VFXListener_->OnHitVFX();

	//溜めている速度をリセット
	ChargeListener_->OnChargeReset();

	//ダッシュ中の速度リセット
	MovementListener_->OnAccelerationStop();

	//念のため正規化する
	//反射方向が0なら処理しない(0ベクトルを正規化はできない)
	if (XMVector3Equal(_normal, XMVectorZero()))
	{
		return;
	}

	//単位ベクトルに変換
	_normal = XMVector3Normalize(_normal);

	//XMFLOAT3型に変換
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, _normal);

	//受け取った法線をノックバック方向に代入
	params_->HitParam_.KnockBack_Direction_ = { tmp };

	//ノックバック量を速度に代入(一定値)
	params_->HitParam_.KnockBack_Velocity_.x = params_->FenceHitParam_.KnockBackPower_;
	params_->HitParam_.KnockBack_Velocity_.z = params_->FenceHitParam_.KnockBackPower_;

	//無敵状態を設定
	params_->FenceHitParam_.IsInvincibility_ = true;

	//接触通知
	NotifyFenceHit();

	//ノックバック時のY軸回転角の固定
	//KnockBackAngleY(HitParam_.KnockBack_Direction_, FenceHitParam_.KnockBackPower_);

	//攻撃相手の名前をリセット
	//自分の名前を代入することで、自爆判定に使う
	params_->HitParam_.AttackedName_ = character_->GetObjectName();
}

void CharacterFence::NotifyFenceHit()
{
	//通知を受け取る側がoverrideしていなかった場合は何も起こらない
	//BattleSceneでのみ有効(監視対象がない=AddObserverを呼ばない場合、
	//InitParam_.observers自体が空なのでfor文がスルーされる)
	for (IGameObserver* observer : params_->InitParam_.observers)
	{
		//監視者へ柵にヒットしたこと（最後に当たった(攻撃した)キャラクターの名前）と
		//柵に当たったキャラクターの名前(NotifyFenceHit()を呼び出した自分自身)を通知
		observer->OnCharacterFenceHit(params_->HitParam_.AttackedName_, character_->GetObjectName());
	}
}

void CharacterFence::InvincibilityTimeCalculation()
{
	//無敵時間の計算

	//無敵時間かどうか判定(この関数は毎フレーム呼ばれるため)
	if (params_->FenceHitParam_.IsInvincibility_)
	{
		//タイマーが終了値を超えたら無敵時間を終わる
		if (++params_->FenceHitParam_.InvincibilityTime_ > params_->FenceHitParam_.InvincibilityValue_)
		{
			params_->FenceHitParam_.InvincibilityTime_ = 0;
			params_->FenceHitParam_.IsInvincibility_ = false;
		}
	}
}