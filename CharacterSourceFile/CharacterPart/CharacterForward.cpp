#include "CharacterForward.h"

CharacterForward::CharacterForward(GameObject* parent)
	:GameObject(parent, "CharacterForward")
{
}

XMVECTOR CharacterForward::RotateVecFront(float _rotY, XMVECTOR _front)
{
	//回転させたいベクトル（方向）を代入
	//基本はローカル正面ベクトル
	XMVECTOR v = _front;

	//Y回転をラジアンにし、回転行列を作成
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(_rotY));

	//方向ベクトルを回転行列で変換し、ワールド座標での向いている方向が出る
	v = XMVector3TransformCoord(v, m);

	return v;
}


void CharacterForward::FrontVectorConfirm()
{
	//ローカル正面ベクトルを現在のy軸回転量で変形すると、正面からどれだけ回転したかが計算される
	//その値がワールド正面ベクトルとなる
	MoveParam_.ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, InitParam_.FrontDirection_);
}