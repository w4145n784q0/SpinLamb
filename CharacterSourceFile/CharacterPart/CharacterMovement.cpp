#include "CharacterMovement.h"

void CharacterMovement::InitStartPosition()
{
	//�����ʒu�ɖ߂�
	this->transform_.position_ = InitParam_.StartPosition_;
}

void CharacterMovement::CharacterMove(XMVECTOR _direction)
{
	//�󂯎����������0�x�N�g���Ȃ珈�����Ȃ�(0�x�N�g���𐳋K���͂ł��Ȃ�)
	if (XMVector3Equal(_direction, XMVectorZero()))
	{
		return;
	}

	//�P�ʃx�N�g�������A�ړ��������m��
	MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	//�ړ��x�N�g�����쐬
	CreateMoveVector();

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPosition_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void CharacterMovement::CreateMoveVector()
{
	//�ړ��x�N�g�� = �ړ����� * ((�����x + �����x) * 1f�̈ړ��ʂ̃X�P�[�����O)
	//�ړ��x�N�g��������
	XMVECTOR MoveVector = XMVectorScale(MoveParam_.MoveDirection_, (MoveParam_.Velocity_ + MoveParam_.Acceleration_) * DeltaTime);

	//���݈ʒu�ƈړ��x�N�g�������Z��
	//�ړ���̃x�N�g�����쐬(���̎��_�ł͈ړ��m�肵�Ă��Ȃ�)
	XMVECTOR PrevPos = XMLoadFloat3(&this->transform_.position_);
	MoveParam_.NewPosition_ = PrevPos + MoveVector;
}

bool CharacterMovement::IsOutsideStage(XMFLOAT3 _position)
{
	//�w��ʒu����ł��X�e�[�W�[�𒴂��邩�ǂ������肵�A�^�U��Ԃ�
	//�X�e�[�W�O���������ۂɎg�p

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
	//�ړ���̃x�N�g����transform_.position�ɑ�����A�ړ����m�肷��
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