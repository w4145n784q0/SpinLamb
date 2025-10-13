#include "CharacterCharge.h"
#include"../../Engine/Audio.h"
#include"../../Engine/Model.h"

void CharacterCharge::Charging()
{
	//�`���[�W����SE�Đ�
	Audio::Play(SoundParam_.hSoundcharge_);

	//�`���[�W��,���̒l�Ɉ��̉����ʂ����Z��������
	//�`���[�W������Ɏ��ۂ�Acceleration_�ɑ������
	if (MoveParam_.TmpAccele_ < MoveParam_.FullAccelerate_)
	{
		MoveParam_.TmpAccele_ += MoveParam_.AcceleValue_;
	}
	else
	{
		vfx_->SetFullChargeEffect();
		MoveParam_.TmpAccele_ = MoveParam_.FullAccelerate_;
	}
}

void CharacterCharge::ChargeRelease()
{
	//���ۂ̉����x�ɗ��߂����̒l����
	MoveParam_.Acceleration_ = MoveParam_.TmpAccele_;

	//���߂Ă��鑬�x�����Z�b�g
	ChargeReset();
}

void CharacterCharge::ChargeReset()
{
	MoveParam_.TmpAccele_ = 0.0f;
}

void CharacterCharge::SetArrow()
{
	//���ʃx�N�g���ɑO�����̒����l����Z���A���̐��ʈʒu���v�Z
	XMVECTOR FrontArrow = XMVectorScale(MoveParam_.ForwardVector_, MoveParam_.AddArrowDepth_);

	//���݈ʒu���擾
	XMVECTOR PosVec = XMLoadFloat3(&this->transform_.position_);

	//���̐��ʈʒu�ƌ��݈ʒu�����Z
	XMVECTOR arrowPosVec = XMVectorAdd(PosVec, FrontArrow);

	//���̃g�����X�t�H�[���ɑ��
	XMStoreFloat3(&MoveParam_.ArrowTransform_.position_, arrowPosVec);
}

void CharacterCharge::DrawArrow()
{
	//��󃂃f����`��
	Model::SetAndDraw(MoveParam_.hMoveArrow_, MoveParam_.ArrowTransform_);
}