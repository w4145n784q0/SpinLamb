#include "CharacterRotate.h"

CharacterRotate::CharacterRotate(GameObject* parent)
	:GameObject(parent, "CharacterRotate")
{
}

float CharacterRotate::RotateDirectionVector(XMVECTOR _MoveVector)
{
	//主にコントローラー・キーボードの入力に使う
	//受け取った方向ベクトルと前向きベクトルの外積求める
	XMVECTOR cross = XMVector3Cross(_MoveVector, InitParam_.FrontDirection_);

	//Y外積をとり+か-かで倒し回転方向を求める
	float crossY = XMVectorGetY(cross);

	//正面ベクトルとのラジアン角をとる
	XMVECTOR r = XMVector3AngleBetweenVectors(_MoveVector, InitParam_.FrontDirection_);

	//ラジアン角度を取得
	float angle = XMVectorGetX(r);

	//ディグリー角に直す
	float angleDeg = XMConvertToDegrees(angle);

	// crossYの正負に応じて回転角度の符号を変える
	if (crossY > 0)
	{
		angleDeg = -angleDeg;
	}

	return angleDeg;
}

void CharacterRotate::MoveRotateX()
{
	this->transform_.rotate_.x += RotateParam_.MoveRotateX;
}

void CharacterRotate::MoveRotateXReverse()
{
	this->transform_.rotate_.x -= RotateParam_.MoveRotateX;
}

void CharacterRotate::FastRotateX()
{
	this->transform_.rotate_.x += RotateParam_.FastRotateX;
}

void CharacterRotate::FastRotateReverse()
{
	this->transform_.rotate_.x -= RotateParam_.FastRotateX;
}

void CharacterRotate::RotateXStop()
{
	this->transform_.rotate_.x = 0.0f;
}