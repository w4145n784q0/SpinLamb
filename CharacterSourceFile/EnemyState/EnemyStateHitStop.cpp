#include "EnemyStateHitStop.h"
#include"../Enemy.h"

void EnemyStateHitStop::Enter(Enemy* _enemy)
{
}

void EnemyStateHitStop::Update(Enemy* _enemy)
{
	//ヒットストップ状態

	//タイマーを増加
	_enemy->HitStopTimerAdd();

	//ヒットストップする時間を超えたら被弾状態へ
	if (_enemy->IsTimeOverHitStopTime())
	{
		//タイマーをリセット
		_enemy->HitStopTimerReset();

		//被弾状態へ移行
		_enemy->ChangeState(Enemy::S_Hit);
	}
}

void EnemyStateHitStop::Exit(Enemy* _enemy)
{
	//状態遷移の際は一度x回転をストップ
	_enemy->rotate_->RotateXStop();
}
