#include "CharacterRotate.h"

CharacterRotate::CharacterRotate(GameObject* parent)
	:GameObject(parent, "CharacterRotate")
{
}

float CharacterRotate::RotateDirectionVector(XMVECTOR _MoveVector)
{
	//��ɃR���g���[���[�E�L�[�{�[�h�̓��͂Ɏg��
	//�󂯎���������x�N�g���ƑO�����x�N�g���̊O�ϋ��߂�
	XMVECTOR cross = XMVector3Cross(_MoveVector, InitParam_.FrontDirection_);

	//Y�O�ς��Ƃ�+��-���œ|����]���������߂�
	float crossY = XMVectorGetY(cross);

	//���ʃx�N�g���Ƃ̃��W�A���p���Ƃ�
	XMVECTOR r = XMVector3AngleBetweenVectors(_MoveVector, InitParam_.FrontDirection_);

	//���W�A���p�x���擾
	float angle = XMVectorGetX(r);

	//�f�B�O���[�p�ɒ���
	float angleDeg = XMConvertToDegrees(angle);

	// crossY�̐����ɉ����ĉ�]�p�x�̕�����ς���
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