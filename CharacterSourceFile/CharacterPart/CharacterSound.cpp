#include "CharacterSound.h"
#include"../../Engine/Audio.h"

CharacterSound::CharacterSound(GameObject* parent)
	:GameObject(parent, "CharacterSound"),params_(nullptr)
{
}

void CharacterSound::LoadSound()
{
	//サウンドの読み込み
	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string SoundSE = "Sound\\SE\\";

	params_->SoundParam_.hSoundcharge_ = Audio::Load(SoundSE + "Charge.wav", false, Audio::ChargeSoundNum_);
	assert(params_->SoundParam_.hSoundcharge_ >= 0);

	params_->SoundParam_.hSoundattack_ = Audio::Load(SoundSE + "Attack.wav", false, Audio::AttackSoundNum_);
	assert(params_->SoundParam_.hSoundattack_ >= 0);

	params_->SoundParam_.hSoundCharacterHit_ = Audio::Load(SoundSE + "CharacterCollision.wav", false, Audio::HitSoundNum_);
	assert(params_->SoundParam_.hSoundCharacterHit_ >= 0);

	params_->SoundParam_.hSoundFenceHit_ = Audio::Load(SoundSE + "FenceCollision.wav", false, Audio::FenceHitSoundNum_);
	assert(params_->SoundParam_.hSoundFenceHit_ >= 0);

	params_->SoundParam_.hSoundJump_ = Audio::Load(SoundSE + "Jump.wav", false, Audio::JumpSoundNum_);
	assert(params_->SoundParam_.hSoundJump_ >= 0);

	params_->SoundParam_.hSoundLanding_ = Audio::Load(SoundSE + "Landing.wav", false, Audio::LandingSoundNum_);
	assert(params_->SoundParam_.hSoundLanding_ >= 0);
}

void CharacterSound::PlayChargeSound()
{
	//チャージ音再生
	Audio::Play(params_->SoundParam_.hSoundcharge_);
}

void CharacterSound::PlayAttackSound()
{
	//ダッシュ攻撃音再生
	Audio::Play(params_->SoundParam_.hSoundattack_);
}

void CharacterSound::PlayCharacterHitSound()
{
	//キャラクターとの衝撃音
	Audio::Play(params_->SoundParam_.hSoundCharacterHit_);
}

void CharacterSound::PlayFenceHitSound()
{
	//柵との衝撃音
	Audio::Play(params_->SoundParam_.hSoundFenceHit_);
}

void CharacterSound::PlayJumpSound()
{
	//ジャンプ音再生
	Audio::Play(params_->SoundParam_.hSoundJump_);
}

void CharacterSound::PlayLandingSound()
{
	//着地音再生
	Audio::Play(params_->SoundParam_.hSoundLanding_);
}
