#include "PlayerStateHitStop.h"
#include"../Player.h"

namespace 
{
	//HitParam_.NextStateName_を保管する変数
	std::string nextState = "";
}

void PlayerStateHitStop::Enter(Player* _player)
{
	nextState = _player->GetParams()->HitParam_.NextStateName_;
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

		if (nextState == "Hit")
		{
			//被弾状態へ移行
			_player->ChangeState(Player::S_Hit);

			//遷移指示が出た後は文字列を空にしておく
			_player->GetParams()->HitParam_.NextStateName_ = "";
		}
		else if (nextState == "Fence")
		{
			//柵に接触状態へ移行
			_player->ChangeState(Player::S_FenceHit);

			//遷移指示が出た後は文字列を空にしておく
			_player->GetParams()->HitParam_.NextStateName_ = "";
		}
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
