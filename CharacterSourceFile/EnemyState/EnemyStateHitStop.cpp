#include "EnemyStateHitStop.h"
#include"../Enemy.h"

void EnemyStateHitStop::Enter(Enemy* _enemy)
{
	//加速度をリセット
	_enemy->OnAccelerationStop();
}

void EnemyStateHitStop::Update(Enemy* _enemy)
{
	//ヒットストップ状態

	//タイマーを増加
	_enemy->OnHitStopTimerAdd();

	//ヒットストップする時間を超えたら被弾状態へ
	if (_enemy->OnIsTimeOverHitStopTime())
	{
		//タイマーをリセット
		_enemy->OnHitStopTimerReset();

		//被弾状態へ移行
		_enemy->ChangeState(Enemy::S_Hit);
	}
}

void EnemyStateHitStop::Exit(Enemy* _enemy)
{
	//状態遷移の際は一度x回転をストップ
	_enemy->OnRotateXStop();
}

bool EnemyStateHitStop::IsHitStopState() const
{
	//ヒットストップ状態であることを返す
	return true;
}
