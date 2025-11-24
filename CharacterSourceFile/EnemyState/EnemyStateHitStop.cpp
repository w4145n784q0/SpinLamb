#include "EnemyStateHitStop.h"
#include"../Enemy.h"

namespace
{
	//HitParam_.NextStateName_を保管する変数
	std::string nextState = "";
}

void EnemyStateHitStop::Enter(Enemy* _enemy)
{
	if (_enemy == nullptr)
	{
		return;
	}

	nextState = _enemy->GetParams()->HitParam_.NextStateName_;
}

void EnemyStateHitStop::Update(Enemy* _enemy)
{
	//ヒットストップ状態

	if (_enemy == nullptr)
	{
		return;
	}

	//タイマーを増加
	_enemy->OnHitStopTimerAdd();

	//ヒットストップする時間を超えたら被弾状態へ
	if (_enemy->OnIsTimeOverHitStopTime())
	{
		if (nextState == "Hit")
		{
			//タイマーをリセット
			_enemy->OnHitStopTimerReset();

			//被弾状態へ移行
			_enemy->ChangeState(Enemy::S_Hit);

			//遷移指示が出た後は文字列を空にしておく
			_enemy->GetParams()->HitParam_.NextStateName_ = "";
		}
		else if (nextState == "Fence")
		{
			//被弾状態へ移行
			_enemy->ChangeState(Enemy::S_FenceHit);

			//遷移指示が出た後は文字列を空にしておく
			_enemy->GetParams()->HitParam_.NextStateName_ = "";
		}
	}
}

void EnemyStateHitStop::Exit(Enemy* _enemy)
{
	if (_enemy == nullptr)
	{
		return;
	}

	//状態遷移の際は一度x回転をストップ
	_enemy->OnRotateXStop();
}

bool EnemyStateHitStop::IsHitStopState() const
{
	//ヒットストップ状態であることを返す
	return true;
}
