#include "CharacterFence.h"

CharacterFence::CharacterFence(GameObject* parent)
	:GameObject(parent, "CharacterFence")
{
}

void CharacterFence::GetWireNormal()
{
	//�e�C���X�^���X�����̖@�����擾
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
	assert(pUpperWire != nullptr);
	FenceHitParam_.UpperNormal_ = pUpperWire->GetNormal();

	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
	assert(pLowerWire != nullptr);
	FenceHitParam_.LowerNormal_ = pLowerWire->GetNormal();

	RightWire* pRightWire = (RightWire*)FindObject("RightWire");
	assert(pRightWire != nullptr);
	FenceHitParam_.RightNormal_ = pRightWire->GetNormal();

	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
	assert(pLeftWire != nullptr);
	FenceHitParam_.LeftNormal_ = pLeftWire->GetNormal();

	//�擾�����@����z��Ɋi�[
	//����炪��ɐڐG�����ۂ̒��˕Ԃ�����ɂȂ�
	FenceHitParam_.NormalArray_ = { FenceHitParam_.UpperNormal_,  FenceHitParam_.LowerNormal_,
		 FenceHitParam_.RightNormal_, FenceHitParam_.LeftNormal_, };
}


void CharacterFence::FenceReflect(XMVECTOR _normal)
{
	//�ڐG�G�t�F�N�g���w��
	vfx_->SetFenceHitEffect();

	//���߂Ă��鑬�x�����Z�b�g
	charge_->ChargeReset();

	//�_�b�V�����̑��x���Z�b�g
	movement_->AccelerationStop();

	//�O�̂��ߐ��K������
	//���˕�����0�Ȃ珈�����Ȃ�(0�x�N�g���𐳋K���͂ł��Ȃ�)
	if (XMVector3Equal(_normal, XMVectorZero()))
	{
		return;
	}

	//�P�ʃx�N�g���ɕϊ�
	_normal = XMVector3Normalize(_normal);

	//XMFLOAT3�^�ɕϊ�
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, _normal);

	//�󂯎�����@�����m�b�N�o�b�N�����ɑ��
	HitParam_.KnockBack_Direction_ = { tmp };

	//�m�b�N�o�b�N�ʂ𑬓x�ɑ��(���l)
	HitParam_.KnockBack_Velocity_.x = FenceHitParam_.KnockBackPower_;
	HitParam_.KnockBack_Velocity_.z = FenceHitParam_.KnockBackPower_;

	//���G��Ԃ�ݒ�
	FenceHitParam_.IsInvincibility_ = true;

	//�ڐG�ʒm
	NotifyFenceHit();

	//�m�b�N�o�b�N����Y����]�p�̌Œ�
	KnockBackAngleY(HitParam_.KnockBack_Direction_, FenceHitParam_.KnockBackPower_);

	//�U������̖��O�����Z�b�g
	//�����̖��O�������邱�ƂŁA��������Ɏg��
	HitParam_.AttackedName_ = this->GetObjectName();
}

void CharacterFence::NotifyFenceHit()
{
	//�ʒm���󂯎�鑤��override���Ă��Ȃ������ꍇ�͉����N����Ȃ�
	//BattleScene�ł̂ݗL��(�Ď��Ώۂ��Ȃ�=AddObserver���Ă΂Ȃ��ꍇ�A
	//InitParam_.observers���̂���Ȃ̂�for�����X���[�����)
	for (IGameObserver* observer : InitParam_.observers)
	{
		//�Ď��҂֍�Ƀq�b�g�������Ɓi�Ō�ɓ�������(�U������)�L�����N�^�[�̖��O�j��
		//��ɓ��������L�����N�^�[�̖��O(NotifyFenceHit()���Ăяo�����������g)��ʒm
		observer->OnCharacterFenceHit(HitParam_.AttackedName_, this->GetObjectName());
	}
}

void CharacterFence::InvincibilityTimeCalculation()
{
	//���G���Ԃ̌v�Z

	//���G���Ԃ��ǂ�������(���̊֐��͖��t���[���Ă΂�邽��)
	if (FenceHitParam_.IsInvincibility_)
	{
		//�^�C�}�[���I���l�𒴂����疳�G���Ԃ��I���
		if (++FenceHitParam_.InvincibilityTime_ > FenceHitParam_.InvincibilityValue_)
		{
			FenceHitParam_.InvincibilityTime_ = 0;
			FenceHitParam_.IsInvincibility_ = false;
		}
	}
}