#include "CharacterHit.h"
#include "../Character.h"

CharacterHit::CharacterHit(GameObject* parent)
	:GameObject(parent, "CharacterHit"), params_(nullptr), character_(nullptr),
	ChargeListener_(nullptr), RotateListener_(nullptr), MovementListener_(nullptr)
{
}

void CharacterHit::SetEventListener(IChargeEventListener* _ChargeListener, 
	IRotateEventListener* _RotateListener, IMovementEventListener* _MovementListener)
{
	ChargeListener_ = _ChargeListener;
	RotateListener_ = _RotateListener;
	MovementListener_ = _MovementListener;
}

void CharacterHit::Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity,
    std::string _attackName)
{
	//���G��ԂȂ珈�����Ȃ�
	if (params_->FenceHitParam_.IsInvincibility_)
	{
		return;
	}

	//�ڐG����̃x�N�g�����玩�g�̃x�N�g��������
	XMVECTOR subVector = XMVectorSubtract(_targetVector, _myVector);

	//�������x�N�g����0�Ȃ炨�݂��̈ʒu�������Ȃ̂ŏ������Ȃ�(0�x�N�g���𐳋K���͂ł��Ȃ�)
	if (XMVector3Equal(subVector, XMVectorZero()))
	{
		return;
	}

	//�P�ʃx�N�g���ɕϊ�
	subVector = XMVector3Normalize(subVector);

	//�t�x�N�g���ɂ��Ĕ��˕���������
	subVector = XMVectorNegate(subVector);
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, subVector);

	//���˕�����ݒ�
	params_->HitParam_.KnockBack_Direction_ = tmp;

	//���g�̑��x�Ƒ���̑��x�̍������Ƃ�
	//���x�̍����ɉ����ăm�b�N�o�b�N�ʂ�ω�������
	float subVelocity = _myVelocity - _targetVelocity;

	//�m�b�N�o�b�N�ʂ̏�����
	//����l���ω�����̂Ń��[�J���ϐ�
	float KnockBackValue = 0.0f;

	//���x���̔���͐��`�⊮���̍ő�l��K�p
	//���x�������g�̕������ȏ�Ȃ�A���g�̃m�b�N�o�b�N�ʂ�0
	if (subVelocity >= params_->HitParam_.OriginalRangeMax_)
	{
		KnockBackValue = 0.0f;
	}
	else
	{
		//�l���}�C�i�X�Ȃ�v���X�ɕύX
		if (signbit(subVelocity))
		{
			subVelocity = -subVelocity;
		}

		//�m�b�N�o�b�N�ʂ̐��`�⊮���s��
		KnockBackValue = LinearInterpolation(subVelocity,
			params_->HitParam_.OriginalRangeMin_, 
			params_->HitParam_.OriginalRangeMax_,
			params_->HitParam_.ConvertedRangeMin_,
			params_->HitParam_.ConvertedRangeMax_);
	}

	//�ϊ������m�b�N�o�b�N�ʂ��m�b�N�o�b�N���̑��xx,z�ɑ��
	params_->HitParam_.KnockBack_Velocity_.x = KnockBackValue;
	params_->HitParam_.KnockBack_Velocity_.z = KnockBackValue;

	//���߂Ă��鑬�x�����Z�b�g
	ChargeListener_->OnChargeReset();

	//�m�b�N�o�b�N����Y����]�p�̌Œ�
	KnockBackAngleY(params_->HitParam_.KnockBack_Direction_, KnockBackValue);

	//�U������̖��O���擾
	params_->HitParam_.AttackedName_ = _attackName;
}

void CharacterHit::KnockBackAngleY(XMFLOAT3 _KnockBackVector, float _KnockBackValue)
{
	if (_KnockBackValue <= 0.0f)
	{
		//�m�b�N�o�b�N�ʂ�0�ȉ��Ȃ珈�����Ȃ�(�������̂܂�)
		return;
	}

	//�m�b�N�o�b�N����Y���̉�]�p�����߂�
	float angleY = static_cast<float>(atan2(_KnockBackVector.x, _KnockBackVector.z));

	//angleY�̓��W�A���p�Ȃ̂Ńf�B�O���[�p�ɕϊ����AY����]�ɃZ�b�g
	character_->SetRotateY(XMConvertToDegrees(angleY));
}

void CharacterHit::KnockBack()
{
	//x����+��]���s��
	RotateListener_->OnMoveRotateX();

	//���t���[���m�b�N�o�b�N���x������
	params_->HitParam_.KnockBack_Velocity_.x *= params_->HitParam_.DecelerationRate_;
	params_->HitParam_.KnockBack_Velocity_.z *= params_->HitParam_.DecelerationRate_;

	//�m�b�N�o�b�N��̈ʒu���v�Z
	//�ʒu = �ʒu + ���� * ���x
	XMFLOAT3 TmpPos = character_->GetPosition();
	TmpPos.x += params_->HitParam_.KnockBack_Direction_.x * params_->HitParam_.KnockBack_Velocity_.x;
	TmpPos.z += params_->HitParam_.KnockBack_Direction_.z * params_->HitParam_.KnockBack_Velocity_.z;

	//���̎��_�ł͕ύX�����A�ړ���̉��̈ʒu�ɕۊ�
	params_->MoveParam_.NewPosition_ = XMLoadFloat3(&TmpPos);

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, params_->MoveParam_.NewPosition_);

	bool IsOutSide = MovementListener_->OnIsOutsideStage(tmp);
	if (!IsOutSide)
	{
		MovementListener_->OnMoveConfirm();
	}
}

XMVECTOR CharacterHit::HitNormal(std::string _normal)
{
	//�w�肵�����O�̓S����WireArray���猩��������Ή������@����Ԃ�
	//�Ԃ�����������ɐڐG�����ۂ̃m�b�N�o�b�N�����ƂȂ�
	//������Ȃ��ꍇ��0��Ԃ�
	for (int i = 0; i < params_->FenceHitParam_.WireArray_.size(); i++)
	{
		if (_normal == params_->FenceHitParam_.WireArray_[i])
		{
			return params_->FenceHitParam_.NormalArray_[i];
		}
	}

	return { 0,0,0 };
}

bool CharacterHit::IsKnockBackEnd()
{
	//�m�b�N�o�b�N���x���I���l�ɓ��B���������肵�A�^�U��Ԃ�

	if (params_->HitParam_.KnockBack_Velocity_.x <= params_->HitParam_.KnockBackEnd_ ||
		params_->HitParam_.KnockBack_Velocity_.z <= params_->HitParam_.KnockBackEnd_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CharacterHit::KnockBackVelocityReset()
{
	params_->HitParam_.KnockBack_Velocity_ = { 0,0,0 };
}