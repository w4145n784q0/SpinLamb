#include "PlayerStateAttack.h"
#include "../Player.h"
#include "../../Engine/Audio.h"

void PlayerStateAttack::Enter(Player* _player)
{
}

void PlayerStateAttack::Update(Player* _player)
{
	//攻撃状態 正面の方向に移動し操作不可

	//ダッシュ画像の更新
	_player->OnUpdateDashImage();

	//攻撃中のエフェクトを出す
	_player->OnAttackLocusVFX(_player->GetPosition());

	//正面ベクトルの方向に移動
	_player->OnCharacterAttackMove(_player->GetParams()->MoveParam_.ForwardVector_);

	//摩擦量(攻撃摩擦量)分速度を減少
	_player->OnFrictionAttackDeceleration();

	//高速X回転
	_player->OnFastRotateX();

	//加速量が0になったら
	if (_player->OnIsAcceleStop())
	{
		//明示的に加速量を0にする
		_player->OnAccelerationStop();

		//通常状態へ戻る
		_player->ChangeState(Player::S_Idle);
	}

	//攻撃SE再生
	Audio::Play(_player->GetParams()->SoundParam_.hSoundattack_);
}

void PlayerStateAttack::Exit(Player* _player)
{
	//状態遷移の際は一度x回転をストップ
	_player->OnRotateXStop();

	//ダッシュ攻撃画像のアニメーション関連の変数をリセット
	_player->GetParams()->AnimeParam_.DashAnimeFrame_ = 0;
	_player->GetParams()->AnimeParam_.DashFrameCount_ = 0;
}

bool PlayerStateAttack::IsAttackState() const
{
	//攻撃状態であることを返す
	return true;
}
