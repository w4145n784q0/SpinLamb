#include "PlayerStateHit.h"
#include "../Player.h"

void PlayerStateHit::Enter(Player* _player)
{
}

void PlayerStateHit::Update(Player* _player)
{
	//ノックバックする
	_player->OnKnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (_player->OnIsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		_player->OnKnockBackVelocityReset();

		//通常状態へ戻る
		_player->ChangeState(Player::S_Idle);

		//ダッシュ中の速度リセット(ノックバック終了時点でリセット)
		_player->OnAccelerationStop();
	}
}

void PlayerStateHit::Exit(Player* _player)
{
	//状態遷移の際は一度x回転をストップ
	_player->OnRotateXStop();
}

bool PlayerStateHit::isHitState() const
{
	//被弾状態であることを返す
	return true;
}
