#include "CharacterForward.h"
#include"../Character.h"

CharacterForward::CharacterForward(GameObject* parent)
	:GameObject(parent, "CharacterForward"),params_(nullptr), character_(nullptr)
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
	params_->MoveParam_.ForwardVector_ = 
		RotateVecFront(character_->GetRotate().y, params_->InitParam_.FrontDirection_);
}