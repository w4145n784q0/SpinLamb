#include "CharacterCharge.h"
#include"../../Engine/Audio.h"
#include"../../Engine/Model.h"
#include "../Character.h" 

CharacterCharge::CharacterCharge(GameObject* parent)
	:GameObject(parent, "CharacterCharge"), params_(nullptr), VFXListener_(nullptr)
{
}

void CharacterCharge::SetEventListener(IVFXEventListener* listener)
{
	VFXListener_ = listener;
}

void CharacterCharge::InitArrow()
{
	//矢印モデルのトランスフォームを初期化
	params_->MoveParam_.ArrowTransform_.position_ = { 0.0f,0.0f, 0.0f };
	params_->MoveParam_.ArrowTransform_.rotate_ = params_->MoveParam_.ArrowRotate_;
	params_->MoveParam_.ArrowTransform_.scale_ = params_->MoveParam_.ArrowScale_;

	//矢印モデル読み込み
	params_->MoveParam_.hMoveArrow_ = Model::Load("Model\\AttackArrow.fbx");
	assert(params_->MoveParam_.hMoveArrow_ >= 0);
}


void CharacterCharge::Charging()
{
	//チャージ中のSE再生
	Audio::Play(params_->SoundParam_.hSoundcharge_);

	//チャージ中,仮の値に一定の加速量を加算し続ける
	//チャージ解放時に実際にAcceleration_に代入する
	if (params_->MoveParam_.TmpAccele_ < params_->MoveParam_.FullAccelerate_)
	{
		params_->MoveParam_.TmpAccele_ += params_->MoveParam_.AcceleValue_;
	}
	else
	{
		//チャージ中エフェクトをリスナークラスから呼び出す
		VFXListener_->OnFullChargeVFX();

		//加速度が最大を超えないようにする
		params_->MoveParam_.TmpAccele_ = params_->MoveParam_.FullAccelerate_;
	}
}

void CharacterCharge::ChargeRelease()
{
	//実際の加速度に溜めた仮の値を代入
	params_->MoveParam_.Acceleration_ = params_->MoveParam_.TmpAccele_;

	//溜めている速度をリセット
	ChargeReset();
}

void CharacterCharge::ChargeReset()
{
	params_->MoveParam_.TmpAccele_ = 0.0f;
}

void CharacterCharge::SetArrow()
{
	//正面ベクトルに前方向の調整値を乗算し、矢印の正面位置を計算
	XMVECTOR FrontArrow = XMVectorScale(params_->MoveParam_.ForwardVector_, params_->MoveParam_.AddArrowDepth_);

	//現在位置を取得
	XMVECTOR PosVec = XMLoadFloat3(&this->transform_.position_);

	//矢印の正面位置と現在位置を加算
	XMVECTOR arrowPosVec = XMVectorAdd(PosVec, FrontArrow);

	//矢印のトランスフォームに代入
	XMStoreFloat3(&params_->MoveParam_.ArrowTransform_.position_, arrowPosVec);
}

void CharacterCharge::DrawArrow()
{
	//矢印モデルを描画
	Model::SetAndDraw(params_->MoveParam_.hMoveArrow_, params_->MoveParam_.ArrowTransform_);
}