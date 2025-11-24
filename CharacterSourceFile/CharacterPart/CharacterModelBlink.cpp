#include "CharacterModelBlink.h"
#include"../../Engine/Model.h"

CharacterModelBlink::CharacterModelBlink(GameObject* parent)
	:GameObject(parent, "CharacterBlink"), params_(nullptr)
{
}

void CharacterModelBlink::DrawCharacterModel(int _handle, Transform _transform)
{
	//無敵時間中かどうかでモデルの点滅表現を行う

	if (params_ == nullptr)
	{
		return;
	}

	if (params_->FenceHitParam_.IsInvincibility_)
	{
		//無敵時間中ならタイマーを使い、一定フレームおきにモデルを描画
		if (++params_->FenceHitParam_.BlinkTimer_ > params_->FenceHitParam_.BlinkValue_) {

			params_->FenceHitParam_.BlinkTimer_ = 0;
			Model::SetAndDraw(_handle, _transform);
		}
	}
	else
	{
		//無敵時間でないなら通常の描画
		Model::SetAndDraw(_handle, _transform);

		//点滅カウントは常に0
		params_->FenceHitParam_.BlinkTimer_ = 0;
	}
}