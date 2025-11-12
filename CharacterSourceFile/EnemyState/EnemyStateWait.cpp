#include "EnemyStateWait.h"
#include"../Enemy.h"

void EnemyStateWait::Enter(Enemy* _enemy)
{
	//加速度をリセット
	_enemy->movement_->AccelerationStop();
}

void EnemyStateWait::Update(Enemy* _enemy)
{
	//待機時間を増加
	_enemy->WaitTimerAdd();
	
	//待機時間が経過したら通常状態へ戻る
	if (_enemy->IsTimeOverWaitTime())
	{
		_enemy->ChangeState(Enemy::S_Root);
	}
}

void EnemyStateWait::Exit(Enemy* _enemy)
{
}
