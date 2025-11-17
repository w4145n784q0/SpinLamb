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

	if (_enemy->IsAttackDecision())
	{
		//無敵時間や被弾・柵へ接触状態が終わったらルート状態へ戻る
		_enemy->ChangeState(Enemy::S_Root);
	}
}

void EnemyStateLook::Exit(Enemy* _enemy)
{	
	//状態遷移の際は一度x回転をストップ
	_enemy->OnRotateXStop();
}
