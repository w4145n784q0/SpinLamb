#include "PlayerStateFenceHit.h"
#include "../Player.h"

void PlayerStateFenceHit::Enter(Player* _player)
{
}

void PlayerStateFenceHit::Update(Player* _player)
{
	//ダメージを受ける柵と接触した状態 操作不可

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

void PlayerStateFenceHit::Exit(Player* _player)
{
	//状態遷移の際は一度x回転をストップ
	_player->rotate_->RotateXStop();
}

bool PlayerStateFenceHit::IsUpdateInvincibility() const
{
	//柵に接触している状態は無敵時間の更新を行わない
	return false;
}

bool PlayerStateFenceHit::IsFenceHitState() const
{
	//柵に接触している状態であることを返す
	return true;
}
