#include "CharacterMovement.h"
#include"../Character.h"

CharacterMovement::CharacterMovement(GameObject* parent)
	:GameObject(parent, "CharacterMovement"), params_(nullptr),character_(nullptr),
	RotateListener_(nullptr)
{
}

void CharacterMovement::SetEventListener(IRotateEventListener* _RotateListener)
{
	RotateListener_ = _RotateListener;
}

void CharacterMovement::InitStartPosition()
{
	// �����ʒu��e�iCharacter�j��transform�ɐݒ肷��
	if (character_ != nullptr)
	{
		character_->SetPosition(params_->InitParam_.StartPosition_);
	}
	else
	{
		//�e���Ȃ���Ύ�����transform�ɃZ�b�g(���S��)
		this->transform_.position_ = params_->InitParam_.StartPosition_;
	}
}

void CharacterMovement::MoveUpdate(XMVECTOR _input)
{
	//���͂�����Ȃ����
	if (!XMVector3Equal(_input, XMVectorZero()))
	{
		AddAcceleration();
	}
	//���͂��Ȃ��Ȃ猸��
	else
	{
		//���C�ɂ�錸��
		FrictionDeceleration();
	}

	//�����x��0�ȉ��Ȃ��~
	if (IsAcceleStop())
	{
		AccelerationStop();
	}
	else
	{
		//�����x��0�ȏ�Ȃ�X��]����
		RotateListener_->OnMoveRotateX();
	}

	//�󂯎����������0�x�N�g���Ȃ�
	if (!XMVector3Equal(_input, XMVectorZero()))
	{
		//�P�ʃx�N�g�������A�ړ��������m��
		params_->MoveParam_.MoveDirection_ = XMVector3Normalize(_input);
	}

	//�ړ��x�N�g�� = �ړ����� * ( �����x * 1f�̈ړ��ʂ̃X�P�[�����O )
	//�ړ��x�N�g��������
	XMVECTOR MoveVector = XMVectorScale(
		params_->MoveParam_.MoveDirection_,
		params_->MoveParam_.CommonAcceleration_ * DeltaTime);

	//���݈ʒu�ƈړ��x�N�g�������Z��
	//�ړ���̃x�N�g�����쐬(���̎��_�ł͈ړ��m�肵�Ă��Ȃ�)
	XMFLOAT3 currentPos = character_->GetPosition();
	XMVECTOR PrevPos = XMLoadFloat3(&currentPos);
	params_->MoveParam_.NewPosition_ = PrevPos + MoveVector;

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, params_->MoveParam_.NewPosition_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}

}

void CharacterMovement::AddAcceleration()
{
	//���̉����ʂ����Z��������
	if (params_->MoveParam_.CommonAcceleration_ < params_->MoveParam_.NormalFullAccelerate_)
	{
		params_->MoveParam_.CommonAcceleration_ += params_->MoveParam_.NormalAcceleValue_;
	}
	else
	{
		//�ő�����x�𒴂��Ȃ��悤�ɂ���
		params_->MoveParam_.CommonAcceleration_ = params_->MoveParam_.NormalFullAccelerate_;
	}
}

void CharacterMovement::CharacterMove(XMVECTOR _direction)
{
	//�󂯎����������0�x�N�g���Ȃ珈�����Ȃ�(0�x�N�g���𐳋K���͂ł��Ȃ�)
	if (XMVector3Equal(_direction, XMVectorZero()))
	{
		return;
	}

	//�P�ʃx�N�g�������A�ړ��������m��
	params_->MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	//�ړ��x�N�g�����쐬
	CreateMoveVector();

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, params_->MoveParam_.NewPosition_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void CharacterMovement::CreateMoveVector()
{
	//�ړ��x�N�g�� = �ړ����� * ( �����x * 1f�̈ړ��ʂ̃X�P�[�����O )
	//�ړ��x�N�g��������
	XMVECTOR MoveVector = XMVectorScale(
		params_->MoveParam_.MoveDirection_, 
			 params_->MoveParam_.CommonAcceleration_ * DeltaTime);

	//���݈ʒu�ƈړ��x�N�g�������Z��
	//�ړ���̃x�N�g�����쐬(���̎��_�ł͈ړ��m�肵�Ă��Ȃ�)
	XMFLOAT3 tmp = character_->GetPosition();
	XMVECTOR PrevPos = XMLoadFloat3(&tmp);
	params_->MoveParam_.NewPosition_ = PrevPos + MoveVector;
}

bool CharacterMovement::IsOutsideStage(XMFLOAT3 _position)
{
	//�w��ʒu����ł��X�e�[�W�[�𒴂��邩�ǂ������肵�A�^�U��Ԃ�
	//�X�e�[�W�O���������ۂɎg�p

	if (_position.x > params_->EndParam_.EastEnd_ || _position.x <  params_->EndParam_.WestEnd_ ||
		_position.z > params_->EndParam_.NorthEnd_ || _position.z < params_->EndParam_.SouthEnd_)
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
	XMFLOAT3 tmp = character_->GetPosition();
	XMStoreFloat3(&tmp, params_->MoveParam_.NewPosition_);
	character_->SetPosition(tmp);
}

void CharacterMovement::Deceleration()
{
	params_->MoveParam_.CommonAcceleration_ -= params_->MoveParam_.NormalAcceleValue_;
}

void CharacterMovement::FrictionDeceleration()
{
	params_->MoveParam_.CommonAcceleration_ -= params_->MoveParam_.Friction_;
}

void CharacterMovement::AccelerationStop()
{
	params_->MoveParam_.CommonAcceleration_ = 0.0f;
}

bool CharacterMovement::IsAcceleStop()
{
	if (params_->MoveParam_.CommonAcceleration_ <= 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}