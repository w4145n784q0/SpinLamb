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
	_enemy->wait_->WaitTimerAdd();
	
	//待機時間が経過したら
	if (_enemy->wait_->IsTimeOverWaitTime())
	{
		//待機時間をリセットしておく
		_enemy->wait_->WaitTimeReset();

		//ルート状態へ戻る
		_enemy->ChangeState(Enemy::S_Root);
	}
}

void EnemyStateWait::Exit(Enemy* _enemy)
{
}
