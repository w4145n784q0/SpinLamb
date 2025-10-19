#include "CharacterModelBlink.h"
#include"../../Engine/Model.h"

CharacterModelBlink::CharacterModelBlink(GameObject* parent)
	:GameObject(parent, "CharacterBlink"), params_(nullptr)
{
}

void CharacterModelBlink::DrawCharacterModel(int _handle, Transform _transform)
{
	//���G���Ԓ����ǂ����Ń��f���̓_�ŕ\�����s��

	if (params_->FenceHitParam_.IsInvincibility_)
	{
		//���G���Ԓ��Ȃ�^�C�}�[���g���A���t���[�������Ƀ��f����`��
		if (++params_->FenceHitParam_.BlinkTimer_ > params_->FenceHitParam_.BlinkValue_) {

			params_->FenceHitParam_.BlinkTimer_ = 0;
			Model::SetAndDraw(_handle, _transform);
		}
	}
	else
	{
		//���G���ԂłȂ��Ȃ�ʏ�̕`��
		Model::SetAndDraw(_handle, _transform);
	}
}