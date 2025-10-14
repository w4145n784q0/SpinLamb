#include "CharacterModelBlink.h"
#include"../../Engine/Model.h"

CharacterModelBlink::CharacterModelBlink(GameObject* parent)
	:GameObject(parent, "CharacterBlink")
{
}

void CharacterModelBlink::DrawCharacterModel(int _handle, Transform _transform)
{
	//���G���Ԓ����ǂ����Ń��f���̓_�ŕ\�����s��

	if (FenceHitParam_.IsInvincibility_)
	{
		//���G���Ԓ��Ȃ�^�C�}�[���g���A���t���[�������Ƀ��f����`��
		if (++FenceHitParam_.BlinkTimer_ > FenceHitParam_.BlinkValue_) {

			FenceHitParam_.BlinkTimer_ = 0;
			Model::SetAndDraw(_handle, _transform);
		}
	}
	else
	{
		//���G���ԂłȂ��Ȃ�ʏ�̕`��
		Model::SetAndDraw(_handle, _transform);
	}
}