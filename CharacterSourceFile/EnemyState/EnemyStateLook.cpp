#include "EnemyStateLook.h"
#include"../Enemy.h"

void EnemyStateLook::Enter(Enemy* _enemy)
{
}

void EnemyStateLook::Update(Enemy* _enemy)
{
	//Playerを様子見している状態

	//プレイヤーのいる方向へY回転する
	_enemy->LookPlayer();

	//進行方向に合わせてY軸を回転
	_enemy->RotateFromDirection(_enemy->GetAutoAttackDirection());
}

void EnemyStateLook::Exit(Enemy* _enemy)
{	
	//状態遷移の際は一度x回転をストップ
	_enemy->rotate_->RotateXStop();
}
