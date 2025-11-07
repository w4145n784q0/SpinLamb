#include "PlayerStateAttack.h"
#include "../Player.h"
#include "../../Engine/Audio.h"

void PlayerStateAttack::Enter(Player* _player)
{
}

void PlayerStateAttack::Update(Player* _player)
{
	//攻撃状態 正面の方向に移動し操作不可

	//攻撃中のエフェクトを出す
	_player->vfx_->SetAttackLocusEffect();

	//正面ベクトルの方向に移動
	_player->movement_->CharacterAttackMove(_player->GetParams()->MoveParam_.ForwardVector_);

	//摩擦量分速度を減少
	_player->movement_->FrictionDeceleration();

	//高速X回転
	_player->rotate_->FastRotateX();

	//加速量が0になったら
	if (_player->movement_->IsAcceleStop())
	{
		//明示的に加速量を0にする
		_player->movement_->AccelerationStop();

		//通常状態へ戻る
		//PlayerState_ = S_Idle;
		//_player->ChangeState(Player::S_Idle);

		//状態遷移の際は一度x回転をストップ
		_player->rotate_->RotateXStop();
	}

	//攻撃SE再生
	Audio::Play(_player->GetParams()->SoundParam_.hSoundattack_);
}

void PlayerStateAttack::Exit(Player* _player)
{
}
