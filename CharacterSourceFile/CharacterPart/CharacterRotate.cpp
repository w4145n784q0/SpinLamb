#include "CharacterRotate.h"
#include"../Character.h"
#include"../../Engine/Global.h"

CharacterRotate::CharacterRotate(GameObject* parent)
	:GameObject(parent, "CharacterRotate"), params_(nullptr), character_(nullptr)
{
}

float CharacterRotate::RotateDirectionVector(XMVECTOR _MoveVector)
{
	if (params_ == nullptr)
	{
		//params_がnullなら0.0を返す
		return 0.0f;
	}

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
	if (params_ == nullptr)
	{
		return;
	}

	if (character_ != nullptr)
	{
		//x軸回転を加算する
		//ローカル変数に親のx軸回転量を代入し、計算してから元に戻す
		float x = character_->GetRotate().x;
		x += params_->RotateParam_.MoveRotateX_;

		//360度を超えたら0度に戻してからセットする
		LimitAngleX(x);
		character_->SetRotateX(x);
	}
}

void CharacterRotate::FastRotateX()
{
	if (params_ == nullptr)
	{
		return;
	}

	if (character_ != nullptr)
	{
		//x軸回転を加算する
		//ローカル変数に親のx軸回転量を代入し、計算してから元に戻す
		float x = character_->GetRotate().x;
		x += params_->RotateParam_.FastRotateX_;

		//360度を超えたら0度に戻してからセットする
		LimitAngleX(x);
		character_->SetRotateX(x);
	}
}

void CharacterRotate::RotateXStop()
{
	//x軸回転を0にする
	if (character_ != nullptr)
	{
		character_->SetRotateX(ANGLE_0_DEG);
	}
}

void CharacterRotate::LimitAngleX(float& _x)
{
	//360度を超えたら0度に戻す
	//元の値から360を引く
	if (_x >= ANGLE_360_DEG)
	{
		_x -= ANGLE_360_DEG;
	}
}
