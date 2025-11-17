#include "EnemyStateApproach.h"
#include"../Enemy.h"

void EnemyStateApproach::Enter(Enemy* _enemy)
{
}

void EnemyStateApproach::Update(Enemy* _enemy)
{
	//Playerを追跡している状態

	//プレイヤーのいる方向へY回転する
	_enemy->LookPlayer();

	//更新した方向へ移動
	_enemy->OnMoveUpdate(_enemy->GetAutoAttackDirection());

	//進行方向に合わせてY軸を回転
	_enemy->RotateFromDirection(_enemy->GetAutoAttackDirection());

	//自身とPlayerの距離を測る 
	float dist = _enemy->PlayerEnemyDistanceX();
	if (dist < _enemy->GetChaseLength())
	{
		//一定以下なら攻撃準備状態へ
		_enemy->ChangeState(Enemy::S_Aim);

	}
}

void EnemyStateApproach::Exit(Enemy* _enemy)
{
	//状態遷移の際は一度x回転をストップ
	_enemy->OnRotateXStop();
}
