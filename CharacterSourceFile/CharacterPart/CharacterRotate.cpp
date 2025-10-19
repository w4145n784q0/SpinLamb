#include "CharacterRotate.h"
#include"../Character.h"

CharacterRotate::CharacterRotate(GameObject* parent)
	:GameObject(parent, "CharacterRotate"), params_(nullptr), character_(nullptr)
{
}

float CharacterRotate::RotateDirectionVector(XMVECTOR _MoveVector)
{
	//��ɃR���g���[���[�E�L�[�{�[�h�̓��͂Ɏg��
	//�󂯎���������x�N�g���ƑO�����x�N�g���̊O�ϋ��߂�
	XMVECTOR cross = XMVector3Cross(_MoveVector, params_->InitParam_.FrontDirection_);

	//Y�O�ς��Ƃ�+��-���œ|����]���������߂�
	float crossY = XMVectorGetY(cross);

	//���ʃx�N�g���Ƃ̃��W�A���p���Ƃ�
	XMVECTOR r = XMVector3AngleBetweenVectors(_MoveVector, params_->InitParam_.FrontDirection_);

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