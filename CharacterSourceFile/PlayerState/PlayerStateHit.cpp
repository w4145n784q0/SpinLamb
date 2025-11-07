#include "PlayerStateHit.h"
#include "../Player.h"

void PlayerStateHit::Enter(Player* _player)
{
}

void PlayerStateHit::Update(Player* _player)
{
	//ノックバックする
	_player->hit_->KnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (_player->hit_->IsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		_player->hit_->KnockBackVelocityReset();

		//通常状態へ戻る
		_player->ChangeState(Player::S_Idle);

		//ダッシュ中の速度リセット(ノックバック終了時点でリセット)
		_player->movement_->AccelerationStop();
	}
}

void PlayerStateHit::Exit(Player* _player)
{
	//状態遷移の際は一度x回転をストップ
	_player->rotate_->RotateXStop();
}
