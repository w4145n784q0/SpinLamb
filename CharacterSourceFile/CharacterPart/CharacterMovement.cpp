#include "CharacterMovement.h"
#include"../Character.h"

CharacterMovement::CharacterMovement(GameObject* parent)
	:GameObject(parent, "CharacterMovement"), params_(nullptr),character_(nullptr)
{
}

void CharacterMovement::InitStartPosition()
{
	// 初期位置を親（Character）のtransformに設定する
	if (character_ != nullptr)
	{
		character_->SetPosition(params_->InitParam_.StartPosition_);
	}
	else
	{
		//親がなければ自分のtransformにセット(安全策)
		this->transform_.position_ = params_->InitParam_.StartPosition_;
	}
}

void CharacterMovement::MoveUpdate(XMVECTOR _dir)
{
	//受け取った移動量が0でなければ（わずかでも入力されていたら0ではない）加速度上昇
	if (!XMVector3Equal(_dir, XMVectorZero()))
	{
		AddAcceleration();
	}
	else
	{
		//摩擦による減速
		FrictionDeceleration();
	}

	//加速度が0以下なら停止
	if (IsAcceleStop())
	{
		AccelerationStop();
	}

	//移動する
	CharacterMove(_dir);

}

void CharacterMovement::AddAcceleration()
{
	//一定の加速量を加算し続ける
	if (params_->MoveParam_.CommonAcceleration_ < params_->MoveParam_.NormalFullAccelerate_)
	{
		params_->MoveParam_.CommonAcceleration_ += params_->MoveParam_.NormalAcceleValue_;
	}
	else
	{
		//最大加速度を超えないようにする
		params_->MoveParam_.CommonAcceleration_ = params_->MoveParam_.NormalFullAccelerate_;
	}
}

void CharacterMovement::CharacterMove(XMVECTOR _direction)
{
	//受け取った方向が0ベクトルなら処理しない(0ベクトルを正規化はできない)
	if (XMVector3Equal(_direction, XMVectorZero()))
	{
		return;
	}

	//単位ベクトル化し、移動方向を確定
	params_->MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	//移動ベクトルを作成
	CreateMoveVector();

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, params_->MoveParam_.NewPosition_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void CharacterMovement::CreateMoveVector()
{
	//移動ベクトル = 移動方向 * ( 加速度 * 1fの移動量のスケーリング )
	//移動ベクトル化する
	XMVECTOR MoveVector = XMVectorScale(
		params_->MoveParam_.MoveDirection_, 
			 params_->MoveParam_.CommonAcceleration_ * DeltaTime);

	//現在位置と移動ベクトルを加算し
	//移動後のベクトルを作成(この時点では移動確定していない)
	XMFLOAT3 tmp = character_->GetPosition();
	XMVECTOR PrevPos = XMLoadFloat3(&tmp);
	params_->MoveParam_.NewPosition_ = PrevPos + MoveVector;
}

bool CharacterMovement::IsOutsideStage(XMFLOAT3 _position)
{
	//指定位置が一つでもステージ端を超えるかどうか判定し、真偽を返す
	//ステージ外判定をする際に使用

	if (_position.x > params_->EndParam_.EastEnd_ || _position.x <  params_->EndParam_.WestEnd_ ||
		_position.z > params_->EndParam_.NorthEnd_ || _position.z < params_->EndParam_.SouthEnd_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CharacterMovement::MoveConfirm()
{
	//移動後のベクトルをtransform_.positionに代入し、移動を確定する
	XMFLOAT3 tmp = character_->GetPosition();
	XMStoreFloat3(&tmp, params_->MoveParam_.NewPosition_);
	character_->SetPosition(tmp);
}

void CharacterMovement::Deceleration()
{
	params_->MoveParam_.CommonAcceleration_ -= params_->MoveParam_.NormalAcceleValue_;
}

void CharacterMovement::FrictionDeceleration()
{
	params_->MoveParam_.CommonAcceleration_ -= params_->MoveParam_.Friction_;
}

void CharacterMovement::AccelerationStop()
{
	params_->MoveParam_.CommonAcceleration_ = 0.0f;
}

bool CharacterMovement::IsAcceleStop()
{
	if (params_->MoveParam_.CommonAcceleration_ <= 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}