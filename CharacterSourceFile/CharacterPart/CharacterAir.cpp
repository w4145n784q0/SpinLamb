#include "CharacterAir.h"
#include"../../Engine/Audio.h"

CharacterAir::CharacterAir(GameObject* parent)
	:GameObject(parent, "CharacterAir"), params_(nullptr)
{
}

void CharacterAir::CharacterGravity()
{
	//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	params_->JumpParam_.JumpSpeed_ -= params_->JumpParam_.Gravity_;

	//�t�B�[���h�ɏ���Ă��邩�͊֌W�Ȃ��d�͂͂����葱����
	this->transform_.position_.y += params_->JumpParam_.JumpSpeed_;

	//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�ɂȂ�����l���Œ肷��
	if (this->transform_.position_.y <= params_->JumpParam_.HeightLowerLimit_)
	{
		this->transform_.position_.y = params_->JumpParam_.HeightLowerLimit_;

		//���ȉ���y���W�ɂȂ����ꍇ,���n���Ă��锻��ɂ���
		params_->JumpParam_.IsOnGround_ = true;
	}

	//�}�C�i�X��������̂ŁA�O�̂��߃I�[�o�[�t���[�h�~
	//JumpSpeed���Œ�l�ȉ��ɂȂ�����Œ�l�̒l�ŌŒ�
	if (params_->JumpParam_.JumpSpeed_ <= params_->JumpParam_.MinusLimit_)
	{
		params_->JumpParam_.JumpSpeed_ = params_->JumpParam_.MinusLimit_;
	}
}

void CharacterAir::SetJump()
{
	//�W�����v���J�n���鏈��

	//�n�㔻���false�ɂ���
	params_->JumpParam_.IsOnGround_ = false;

	//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫�����邱�ƂŁA�L�����N�^�[����яオ��
	params_->JumpParam_.JumpSpeed_ = params_->JumpParam_.JumpHeight;

	//�`���[�W����SE�Đ�
	Audio::Play(params_->SoundParam_.hSoundJump_);
}