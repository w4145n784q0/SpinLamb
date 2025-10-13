#include "CharacterMovement.h"

void CharacterMovement::InitStartPosition()
{
	//初期位置に戻す
	this->transform_.position_ = InitParam_.StartPosition_;
}

void CharacterMovement::CharacterMove(XMVECTOR _direction)
{
	//受け取った方向が0ベクトルなら処理しない(0ベクトルを正規化はできない)
	if (XMVector3Equal(_direction, XMVectorZero()))
	{
		return;
	}

	//単位ベクトル化し、移動方向を確定
	MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	//移動ベクトルを作成
	CreateMoveVector();

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPosition_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void CharacterMovement::CreateMoveVector()
{
	//移動ベクトル = 移動方向 * ((初速度 + 加速度) * 1fの移動量のスケーリング)
	//移動ベクトル化する
	XMVECTOR MoveVector = XMVectorScale(MoveParam_.MoveDirection_, (MoveParam_.Velocity_ + MoveParam_.Acceleration_) * DeltaTime);

	//現在位置と移動ベクトルを加算し
	//移動後のベクトルを作成(この時点では移動確定していない)
	XMVECTOR PrevPos = XMLoadFloat3(&this->transform_.position_);
	MoveParam_.NewPosition_ = PrevPos + MoveVector;
}

bool CharacterMovement::IsOutsideStage(XMFLOAT3 _position)
{
	//指定位置が一つでもステージ端を超えるかどうか判定し、真偽を返す
	//ステージ外判定をする際に使用

	if (_position.x > EndParam_.EastEnd_ || _position.x <  EndParam_.WestEnd_ ||
		_position.z > EndParam_.NorthEnd_ || _position.z < EndParam_.SouthEnd_)
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
	XMStoreFloat3(&this->transform_.position_, MoveParam_.NewPosition_);
}

void CharacterMovement::Deceleration()
{
	MoveParam_.Acceleration_ -= MoveParam_.AcceleValue_;
}

void CharacterMovement::FrictionDeceleration()
{
	MoveParam_.Acceleration_ -= MoveParam_.Friction_;
}

void CharacterMovement::AccelerationStop()
{
	MoveParam_.Acceleration_ = 0.0f;
}

bool CharacterMovement::IsDashStop()
{
	if (MoveParam_.Acceleration_ <= 0.0f)
	{
		return true; 
	}
	else
	{
		return false;
	}
}