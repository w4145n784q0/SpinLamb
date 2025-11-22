#include "PlayerStateHitStop.h"
#include"../Player.h"

void PlayerStateHitStop::Enter(Player* _player)
{
	//加速度をリセット
	_player->OnAccelerationStop();
}

void PlayerStateHitStop::Update(Player* _player)
{
	//ヒットストップ状態

	//タイマーを増加
	_player->OnHitStopTimerAdd();

	//ヒットストップする時間を超えたら被弾状態へ
	if (_player->OnIsTimeOverHitStopTime())
	{
		//タイマーをリセット
		_player->OnHitStopTimerReset();

		//被弾状態へ移行
		_player->ChangeState(Player::S_Hit);
	}
}

void PlayerStateHitStop::Exit(Player* _player)
{
	//状態遷移の際は一度x回転をストップ
	_player->OnRotateXStop();
}

bool PlayerStateHitStop::IsHitStopState() const
{
	//ヒットストップ状態であることを返す
	return true;
}
