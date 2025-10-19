#include "CharacterAir.h"
#include"../../Engine/Audio.h"

CharacterAir::CharacterAir(GameObject* parent)
	:GameObject(parent, "CharacterAir"), params_(nullptr)
{
}

void CharacterAir::CharacterGravity()
{
	//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	params_->JumpParam_.JumpSpeed_ -= params_->JumpParam_.Gravity_;

	//フィールドに乗っているかは関係なく重力はかかり続ける
	this->transform_.position_.y += params_->JumpParam_.JumpSpeed_;

	//プレイヤーめりこみ防止に一定以下のy座標になったら値を固定する
	if (this->transform_.position_.y <= params_->JumpParam_.HeightLowerLimit_)
	{
		this->transform_.position_.y = params_->JumpParam_.HeightLowerLimit_;

		//一定以下のy座標になった場合,着地している判定にする
		params_->JumpParam_.IsOnGround_ = true;
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