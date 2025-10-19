#include "CharacterCharge.h"
#include"../../Engine/Audio.h"
#include"../../Engine/Model.h"
#include "../Character.h" 

CharacterCharge::CharacterCharge(GameObject* parent)
	:GameObject(parent, "CharacterCharge"), params_(nullptr), VFXListener_(nullptr)
{
}

void CharacterCharge::SetEventListener(IVFXEventListener* listener)
{
	VFXListener_ = listener;
}

void CharacterCharge::InitArrow()
{
	//��󃂃f���̃g�����X�t�H�[����������
	params_->MoveParam_.ArrowTransform_.position_ = { 0.0f,0.0f, 0.0f };
	params_->MoveParam_.ArrowTransform_.rotate_ = params_->MoveParam_.ArrowRotate_;
	params_->MoveParam_.ArrowTransform_.scale_ = params_->MoveParam_.ArrowScale_;

	//��󃂃f���ǂݍ���
	params_->MoveParam_.hMoveArrow_ = Model::Load("Model\\AttackArrow.fbx");
	assert(params_->MoveParam_.hMoveArrow_ >= 0);
}


void CharacterCharge::Charging()
{
	//�`���[�W����SE�Đ�
	Audio::Play(params_->SoundParam_.hSoundcharge_);

	//�`���[�W��,���̒l�Ɉ��̉����ʂ����Z��������
	//�`���[�W������Ɏ��ۂ�Acceleration_�ɑ������
	if (params_->MoveParam_.TmpAccele_ < params_->MoveParam_.FullAccelerate_)
	{
		params_->MoveParam_.TmpAccele_ += params_->MoveParam_.AcceleValue_;
	}
	else
	{
		//�`���[�W���G�t�F�N�g�����X�i�[�N���X����Ăяo��
		VFXListener_->OnFullChargeVFX();

		//�����x���ő�𒴂��Ȃ��悤�ɂ���
		params_->MoveParam_.TmpAccele_ = params_->MoveParam_.FullAccelerate_;
	}
}

void CharacterCharge::ChargeRelease()
{
	//���ۂ̉����x�ɗ��߂����̒l����
	params_->MoveParam_.Acceleration_ = params_->MoveParam_.TmpAccele_;

	//���߂Ă��鑬�x�����Z�b�g
	ChargeReset();
}

void CharacterCharge::ChargeReset()
{
	params_->MoveParam_.TmpAccele_ = 0.0f;
}

void CharacterCharge::SetArrow()
{
	//���ʃx�N�g���ɑO�����̒����l����Z���A���̐��ʈʒu���v�Z
	XMVECTOR FrontArrow = XMVectorScale(params_->MoveParam_.ForwardVector_, params_->MoveParam_.AddArrowDepth_);

	//���݈ʒu���擾
	XMVECTOR PosVec = XMLoadFloat3(&this->transform_.position_);

	//���̐��ʈʒu�ƌ��݈ʒu�����Z
	XMVECTOR arrowPosVec = XMVectorAdd(PosVec, FrontArrow);

	//���̃g�����X�t�H�[���ɑ��
	XMStoreFloat3(&params_->MoveParam_.ArrowTransform_.position_, arrowPosVec);
}

void CharacterCharge::DrawArrow()
{
	//��󃂃f����`��
	Model::SetAndDraw(params_->MoveParam_.hMoveArrow_, params_->MoveParam_.ArrowTransform_);
}