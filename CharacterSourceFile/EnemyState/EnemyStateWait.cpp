#include "EnemyStateWait.h"
#include"../Enemy.h"

void EnemyStateWait::Enter(Enemy* _enemy)
{
	//加速度をリセット
	_enemy->GetModuleMovement()->AccelerationStop();
}

void EnemyStateWait::Update(Enemy* _enemy)
{
	//待機時間を増加
	_enemy->GetModuleWait()->WaitTimerAdd();
	
	//待機時間が経過したら
	if (_enemy->GetModuleWait()->IsTimeOverWaitTime())
	{
		//待機時間をリセットしておく
		_enemy->GetModuleWait()->WaitTimeReset();

		//ルート状態へ戻る
		_enemy->ChangeState(Enemy::S_Root);
	}
}

void EnemyStateWait::Exit(Enemy* _enemy)
{
	//状態遷移の際は一度x回転をストップ
	_enemy->GetModuleRotate()->RotateXStop();
}
