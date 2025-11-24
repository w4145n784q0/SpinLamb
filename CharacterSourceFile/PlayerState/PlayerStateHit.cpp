#include "PlayerStateHit.h"
#include "../Player.h"

void PlayerStateHit::Enter(Player* _player)
{
}

void PlayerStateHit::Update(Player* _player)
{
	//キャラクタークラス継承した相手オブジェクトと接触した状態 移動不可

	if (_player == nullptr)
	{
		return;
	}

	//ノックバックする
	_player->OnKnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (_player->OnIsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		_player->OnKnockBackVelocityReset();

		//通常状態へ戻る
		_player->ChangeState(Player::S_Idle);

		//加速度リセット(ノックバック終了時点でリセット)
		_player->OnAccelerationStop();
	}
}

void PlayerStateHit::Exit(Player* _player)
{
	if (_player == nullptr)
	{
		return;
	}

	//状態遷移の際は一度x回転をストップ
	_player->OnRotateXStop();
}

bool PlayerStateHit::isHitState() const
{
	//被弾状態であることを返す
	return true;
}
