#include "CharacterFence.h"
#include"../Character.h"
#include"../../StageSourceFile/UpperWire.h"
#include"../../StageSourceFile/LowerWire.h"
#include"../../StageSourceFile/LeftWire.h"
#include"../../StageSourceFile/RightWire.h"

CharacterFence::CharacterFence(GameObject* parent)
	:GameObject(parent, "CharacterFence"), params_(nullptr), character_(nullptr),
	VFXListener_(nullptr), ChargeListener_(nullptr), MovementListener_(nullptr)
{
}

void CharacterFence::SetEventListener(IVFXEventListener* _VfxListener,
	IChargeEventListener* _ChargeListener, IMovementEventListener* _MovementListener)
{
	VFXListener_ = _VfxListener;
	ChargeListener_ = _ChargeListener;
	MovementListener_ = _MovementListener;
}

void CharacterFence::GetWireNormal()
{
	//�e�C���X�^���X�����̖@�����擾
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
	assert(pUpperWire != nullptr);
	params_->FenceHitParam_.UpperNormal_ = pUpperWire->GetNormal();

	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
	assert(pLowerWire != nullptr);
	params_->FenceHitParam_.LowerNormal_ = pLowerWire->GetNormal();

	RightWire* pRightWire = (RightWire*)FindObject("RightWire");
	assert(pRightWire != nullptr);
	params_->FenceHitParam_.RightNormal_ = pRightWire->GetNormal();

	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
	assert(pLeftWire != nullptr);
	params_->FenceHitParam_.LeftNormal_ = pLeftWire->GetNormal();

	//�擾�����@����z��Ɋi�[
	//����炪��ɐڐG�����ۂ̒��˕Ԃ�����ɂȂ�
	params_->FenceHitParam_.NormalArray_ = {
		params_->FenceHitParam_.UpperNormal_, 
		params_->FenceHitParam_.LowerNormal_,
		params_->FenceHitParam_.RightNormal_, 
		params_->FenceHitParam_.LeftNormal_, 
	};
}


void CharacterFence::FenceReflect(XMVECTOR _normal)
{
	//�ڐG�G�t�F�N�g��ʒm
	VFXListener_->OnHitVFX();

	//���߂Ă��鑬�x�����Z�b�g
	ChargeListener_->OnChargeReset();

	//�_�b�V�����̑��x���Z�b�g
	MovementListener_->OnAccelerationStop();

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
	params_->HitParam_.KnockBack_Direction_ = { tmp };

	//�m�b�N�o�b�N�ʂ𑬓x�ɑ��(���l)
	params_->HitParam_.KnockBack_Velocity_.x = params_->FenceHitParam_.KnockBackPower_;
	params_->HitParam_.KnockBack_Velocity_.z = params_->FenceHitParam_.KnockBackPower_;

	//���G��Ԃ�ݒ�
	params_->FenceHitParam_.IsInvincibility_ = true;

	//�ڐG�ʒm
	NotifyFenceHit();

	//�m�b�N�o�b�N����Y����]�p�̌Œ�
	//KnockBackAngleY(HitParam_.KnockBack_Direction_, FenceHitParam_.KnockBackPower_);

	//�U������̖��O�����Z�b�g
	//�����̖��O�������邱�ƂŁA��������Ɏg��
	params_->HitParam_.AttackedName_ = character_->GetObjectName();
}

void CharacterFence::NotifyFenceHit()
{
	//�ʒm���󂯎�鑤��override���Ă��Ȃ������ꍇ�͉����N����Ȃ�
	//BattleScene�ł̂ݗL��(�Ď��Ώۂ��Ȃ�=AddObserver���Ă΂Ȃ��ꍇ�A
	//InitParam_.observers���̂���Ȃ̂�for�����X���[�����)
	for (IGameObserver* observer : params_->InitParam_.observers)
	{
		//�Ď��҂֍�Ƀq�b�g�������Ɓi�Ō�ɓ�������(�U������)�L�����N�^�[�̖��O�j��
		//��ɓ��������L�����N�^�[�̖��O(NotifyFenceHit()���Ăяo�����������g)��ʒm
		observer->OnCharacterFenceHit(params_->HitParam_.AttackedName_, character_->GetObjectName());
	}
}

void CharacterFence::InvincibilityTimeCalculation()
{
	//���G���Ԃ̌v�Z

	//���G���Ԃ��ǂ�������(���̊֐��͖��t���[���Ă΂�邽��)
	if (params_->FenceHitParam_.IsInvincibility_)
	{
		//�^�C�}�[���I���l�𒴂����疳�G���Ԃ��I���
		if (++params_->FenceHitParam_.InvincibilityTime_ > params_->FenceHitParam_.InvincibilityValue_)
		{
			params_->FenceHitParam_.InvincibilityTime_ = 0;
			params_->FenceHitParam_.IsInvincibility_ = false;
		}
	}
}