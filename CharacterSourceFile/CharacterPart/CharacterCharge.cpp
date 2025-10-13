#include "CharacterCharge.h"
#include"../../Engine/Audio.h"
#include"../../Engine/Model.h"

void CharacterCharge::Charging()
{
	//チャージ中のSE再生
	Audio::Play(SoundParam_.hSoundcharge_);

	//チャージ中,仮の値に一定の加速量を加算し続ける
	//チャージ解放時に実際にAcceleration_に代入する
	if (MoveParam_.TmpAccele_ < MoveParam_.FullAccelerate_)
	{
		MoveParam_.TmpAccele_ += MoveParam_.AcceleValue_;
	}
	else
	{
		vfx_->SetFullChargeEffect();
		MoveParam_.TmpAccele_ = MoveParam_.FullAccelerate_;
	}
}

void CharacterCharge::ChargeRelease()
{
	//実際の加速度に溜めた仮の値を代入
	MoveParam_.Acceleration_ = MoveParam_.TmpAccele_;

	//溜めている速度をリセット
	ChargeReset();
}

void CharacterCharge::ChargeReset()
{
	MoveParam_.TmpAccele_ = 0.0f;
}

void CharacterCharge::SetArrow()
{
	//正面ベクトルに前方向の調整値を乗算し、矢印の正面位置を計算
	XMVECTOR FrontArrow = XMVectorScale(MoveParam_.ForwardVector_, MoveParam_.AddArrowDepth_);

	//現在位置を取得
	XMVECTOR PosVec = XMLoadFloat3(&this->transform_.position_);

	//矢印の正面位置と現在位置を加算
	XMVECTOR arrowPosVec = XMVectorAdd(PosVec, FrontArrow);

	//矢印のトランスフォームに代入
	XMStoreFloat3(&MoveParam_.ArrowTransform_.position_, arrowPosVec);
}

void CharacterCharge::DrawArrow()
{
	//矢印モデルを描画
	Model::SetAndDraw(MoveParam_.hMoveArrow_, MoveParam_.ArrowTransform_);
}