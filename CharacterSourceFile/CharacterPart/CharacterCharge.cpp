#include "CharacterCharge.h"
#include "../Character.h" 
#include"../../Engine/Model.h"


CharacterCharge::CharacterCharge(GameObject* parent)
	:GameObject(parent, "CharacterCharge"), character_(nullptr), params_(nullptr)
{
}

void CharacterCharge::InitArrow()
{
	if (params_ == nullptr)
	{
		return;
	}

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
	if (params_ == nullptr)
	{
		return;
	}

	//チャージ中のSE再生
	character_->OnPlayChargeSound();

	//チャージ中,仮の値に一定の加速量を加算し続ける
	//チャージ解放時に実際にCommonAcceleration_に代入する
	if (params_->MoveParam_.TmpAccele_ < params_->MoveParam_.AttackFullAccelerate_)
	{
		params_->MoveParam_.TmpAccele_ += params_->MoveParam_.AttackAcceleValue_;
	}
	else
	{
		//最大チャージエフェクトを出す
		if(character_ != nullptr)
		{
			character_->OnFullChargeVFX(character_->GetPosition());
		}

		//加速度が最大を超えないようにする
		params_->MoveParam_.TmpAccele_ = params_->MoveParam_.AttackFullAccelerate_;
	}
}

void CharacterCharge::ChargeRelease()
{
	if (params_ == nullptr)
	{
		return;
	}

	//実際の加速度に溜めた仮の値を代入
	params_->MoveParam_.CommonAcceleration_ = params_->MoveParam_.TmpAccele_;

	//溜めている速度をリセット
	ChargeReset();
}

void CharacterCharge::ChargeReset()
{
	if (params_ == nullptr)
	{
		return;
	}

	params_->MoveParam_.TmpAccele_ = 0.0f;
}

void CharacterCharge::SetArrow()
{
	if (params_ == nullptr)
	{
		return;
	}

	if (character_ != nullptr)
	{
		//正面ベクトルに前方向の調整値を乗算し、矢印の正面位置を計算
		XMVECTOR FrontArrow = XMVectorScale(params_->MoveParam_.ForwardVector_, params_->MoveParam_.AddArrowDepth_);

		//現在位置を取得
		XMFLOAT3 tmp = character_->GetPosition();
		XMVECTOR PosVec = XMLoadFloat3(&tmp);

		//矢印の正面位置と現在位置を加算
		XMVECTOR arrowPosVec = XMVectorAdd(PosVec, FrontArrow);

		//矢印のトランスフォームに代入
		XMStoreFloat3(&params_->MoveParam_.ArrowTransform_.position_, arrowPosVec);
	}
}

void CharacterCharge::DrawArrow()
{
	if (params_ == nullptr)
	{
		return;
	}

	//矢印モデルを描画
	Model::SetAndDraw(params_->MoveParam_.hMoveArrow_, params_->MoveParam_.ArrowTransform_);
}