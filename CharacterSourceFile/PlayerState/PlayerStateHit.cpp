#include "PlayerStateHit.h"
#include "../Player.h"

void PlayerStateHit::Enter(Player* _player)
{
}

void PlayerStateHit::Update(Player* _player)
{
	//ノックバックする
	_player->GetModuleHit()->KnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (_player->GetModuleHit()->IsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		_player->GetModuleHit()->KnockBackVelocityReset();

		//通常状態へ戻る
		_player->ChangeState(Player::S_Idle);

		//ダッシュ中の速度リセット(ノックバック終了時点でリセット)
		_player->GetModuleMovement()->AccelerationStop();
	}
}

void PlayerStateHit::Exit(Player* _player)
{
	//状態遷移の際は一度x回転をストップ
	_player->GetModuleRotate()->RotateXStop();
}

bool PlayerStateHit::isHitState() const
{
	//被弾状態であることを返す
	return true;
}
