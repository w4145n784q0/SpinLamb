#include "CharacterAir.h"
#include"../Character.h"
#include"../../Engine/Audio.h"

CharacterAir::CharacterAir(GameObject* parent)
	:GameObject(parent, "CharacterAir"), params_(nullptr),character_(nullptr)
{
}

void CharacterAir::CharacterGravity()
{
	//前フレームの着地状態を保存
	bool prevOnGround = params_->JumpParam_.PrevOnGround_;

	//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	params_->JumpParam_.JumpSpeed_ -= params_->JumpParam_.Gravity_;

	//フィールドに乗っているかは関係なく重力はかかり続ける
	if (character_ != nullptr)
	{
		//親の位置を取得してy座標を更新する（コンポーネント自身のtransform_ではなく親を更新）
		XMFLOAT3 parentPos = character_->GetPosition();
		parentPos.y += params_->JumpParam_.JumpSpeed_;

		//プレイヤーめりこみ防止に一定以下のy座標になったら値を固定する
		if (parentPos.y <= params_->JumpParam_.HeightLowerLimit_)
		{
			parentPos.y = params_->JumpParam_.HeightLowerLimit_;

			//一定以下のy座標になった場合,着地している判定にする
			params_->JumpParam_.IsOnGround_ = true;

			//前回は着地していなかった かつ 今着地している
			//false → true の瞬間だけ実行
			if (!prevOnGround && params_->JumpParam_.IsOnGround_)
			{
				//着地した瞬間にエフェクトを出す
				character_->OnLandingVFX(parentPos);

				//着地のSE再生
				Audio::Play(params_->SoundParam_.hSoundLanding_);
			}
		}
		else
		{
			//空中判定
			params_->JumpParam_.IsOnGround_ = false;
		}

		//次フレームのために現在の着地状態を保存
		params_->JumpParam_.PrevOnGround_ = params_->JumpParam_.IsOnGround_;

		//親の位置を更新
		character_->SetPosition(parentPos);
	}

	//マイナスし続けるので、念のためオーバーフロー防止
	//JumpSpeedが最低値以下になったら最低値の値で固定
	if (params_->JumpParam_.JumpSpeed_ <= params_->JumpParam_.MinusLimit_)
	{
		params_->JumpParam_.JumpSpeed_ = params_->JumpParam_.MinusLimit_;
	}
}

void CharacterAir::SetJump()
{
	//ジャンプを開始する処理

	//地上判定をfalseにする
	params_->JumpParam_.IsOnGround_ = false;

	//一時的にy方向にマイナスされている値を大きくすることで、キャラクターが飛び上がる
	params_->JumpParam_.JumpSpeed_ = params_->JumpParam_.JumpHeight;

	//チャージ中のSE再生
	Audio::Play(params_->SoundParam_.hSoundJump_);
}