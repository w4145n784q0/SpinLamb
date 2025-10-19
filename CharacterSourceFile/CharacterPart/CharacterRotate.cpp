#include "CharacterRotate.h"
#include"../Character.h"

CharacterRotate::CharacterRotate(GameObject* parent)
	:GameObject(parent, "CharacterRotate"), params_(nullptr), character_(nullptr)
{
}

float CharacterRotate::RotateDirectionVector(XMVECTOR _MoveVector)
{
	//主にコントローラー・キーボードの入力に使う
	//受け取った方向ベクトルと前向きベクトルの外積求める
	XMVECTOR cross = XMVector3Cross(_MoveVector, params_->InitParam_.FrontDirection_);

	//Y外積をとり+か-かで倒し回転方向を求める
	float crossY = XMVectorGetY(cross);

	//正面ベクトルとのラジアン角をとる
	XMVECTOR r = XMVector3AngleBetweenVectors(_MoveVector, params_->InitParam_.FrontDirection_);

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
	float x = character_->GetRotate().x;
	x += params_->RotateParam_.MoveRotateX;
	character_->SetRotateX(x);
}

void CharacterRotate::FastRotateX()
{
	float x = character_->GetRotate().x;
	x += params_->RotateParam_.FastRotateX;
	character_->SetRotateX(x);
}

void CharacterRotate::RotateXStop()
{
	character_->SetRotateX(0.0f);
}