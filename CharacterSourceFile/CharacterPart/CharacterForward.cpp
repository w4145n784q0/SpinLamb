#include "CharacterForward.h"

CharacterForward::CharacterForward(GameObject* parent)
	:GameObject(parent, "CharacterForward")
{
}

XMVECTOR CharacterForward::RotateVecFront(float _rotY, XMVECTOR _front)
{
	//��]���������x�N�g���i�����j����
	//��{�̓��[�J�����ʃx�N�g��
	XMVECTOR v = _front;

	//Y��]�����W�A���ɂ��A��]�s����쐬
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(_rotY));

	//�����x�N�g������]�s��ŕϊ����A���[���h���W�ł̌����Ă���������o��
	v = XMVector3TransformCoord(v, m);

	return v;
}


void CharacterForward::FrontVectorConfirm()
{
	//���[�J�����ʃx�N�g�������݂�y����]�ʂŕό`����ƁA���ʂ���ǂꂾ����]���������v�Z�����
	//���̒l�����[���h���ʃx�N�g���ƂȂ�
	MoveParam_.ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, InitParam_.FrontDirection_);
}