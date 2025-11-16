#include "EnemyStateHitStop.h"
#include"../Enemy.h"

void EnemyStateHitStop::Enter(Enemy* _enemy)
{
}

void EnemyStateHitStop::Update(Enemy* _enemy)
{
	//ヒットストップ状態

	//タイマーを増加
	_enemy->GetModuleHitStop()->HitStopTimerAdd();

	//ヒットストップする時間を超えたら被弾状態へ
	if (_enemy->GetModuleHitStop()->IsTimeOverHitStopTime())
	{
		//タイマーをリセット
		_enemy->GetModuleHitStop()->HitStopTimerReset();

		//被弾状態へ移行
		_enemy->ChangeState(Enemy::S_Hit);
	}
}

void EnemyStateHitStop::Exit(Enemy* _enemy)
{
	//状態遷移の際は一度x回転をストップ
	_enemy->GetModuleRotate()->RotateXStop();
}

bool EnemyStateHitStop::IsHitStopState() const
{
	//ヒットストップ状態であることを返す
	return true;
}
